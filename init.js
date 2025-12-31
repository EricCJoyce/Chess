if(!Detector.webgl)
  Detector.addGetWebGLMessage();

var gameEngine;                                                     //  Compiled WebASM Module.
var gameStateOffset;                                                //  Address of read/write memory in gameEngine.
var gameStateBuffer;                                                //  Byte buffer
var gameOutputOffset;                                               //  Address of read/write memory in gameEngine.
var gameOutputBuffer;                                               //  Byte buffer
const _GAMESTATE_BYTE_SIZE = 81;                                    //  Size (see C code).
const _MOVE_BYTE_SIZE = 3;                                          //  Size (see C code).
const _MOVEBUFFER_BYTE_SIZE = 32;                                   //  Size (see C code).
const _MAX_MOVES = 64;                                              //  Size (see C code).
const _ZHASH_TABLE_SIZE = 751;                                      //  Size (see C code).
const _HASH_VALUE_BYTE_SIZE = 8;                                    //  Size of long long.
const _TRANSPO_TABLE_SIZE = 524288;                                 //  Size (see C code).
const _TRANSPO_RECORD_BYTE_SIZE = 18;                               //  Size (see C code).
const _PARAMETER_ARRAY_SIZE = 16;                                   //  Size (see C++ code).
const _TREE_SEARCH_ARRAY_SIZE = 65536;                              //  Size (see C++ code).
const _NEGAMAX_NODE_BYTE_SIZE = 139;                                //  Size (see C++ code).
const _NEGAMAX_MOVE_BYTE_SIZE = 4;                                  //  Size (see C++ code).
const _KILLER_MOVE_PER_PLY = 2;                                     //  Size (see C++ code).
const _KILLER_MOVE_MAX_DEPTH = 64;                                  //  Size (see C++ code).

var philadelphia = new Player();                                    //  Create the A.I. agent.

//////////////////////////////////////////////////////////////////////  Controls
var gameStarted = false;                                            //  When the clock starts running, if there's a clock.

//////////////////////////////////////////////////////////////////////  Window dimensions
var screenWidth = window.innerWidth;
var screenHeight = window.innerHeight;

//////////////////////////////////////////////////////////////////////  CONSTANTS
const COLS = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'];
const ROWS = ['1', '2', '3', '4', '5', '6', '7', '8'];
const _A1 = 0,  _B1 = 1,  _C1 = 2,  _D1 = 3,  _E1 = 4,  _F1 = 5,  _G1 = 6,  _H1 = 7;
const _A2 = 8,  _B2 = 9,  _C2 = 10, _D2 = 11, _E2 = 12, _F2 = 13, _G2 = 14, _H2 = 15;
const _A3 = 16, _B3 = 17, _C3 = 18, _D3 = 19, _E3 = 20, _F3 = 21, _G3 = 22, _H3 = 23;
const _A4 = 24, _B4 = 25, _C4 = 26, _D4 = 27, _E4 = 28, _F4 = 29, _G4 = 30, _H4 = 31;
const _A5 = 32, _B5 = 33, _C5 = 34, _D5 = 35, _E5 = 36, _F5 = 37, _G5 = 38, _H5 = 39;
const _A6 = 40, _B6 = 41, _C6 = 42, _D6 = 43, _E6 = 44, _F6 = 45, _G6 = 46, _H6 = 47;
const _A7 = 48, _B7 = 49, _C7 = 50, _D7 = 51, _E7 = 52, _F7 = 53, _G7 = 54, _H7 = 55;
const _A8 = 56, _B8 = 57, _C8 = 58, _D8 = 59, _E8 = 60, _F8 = 61, _G8 = 62, _H8 = 63;
const _NOTHING = 64;

//////////////////////////////////////////////////////////////////////  Three.js
var VIEW_ANGLE = 45;
var ASPECT = screenWidth / screenHeight;
var NEAR = 0.1;
var FAR = 10000;
var container = document.getElementById('container');               //  Contains the game
var renderer, camera, scene;
var CAMERA_X, CAMERA_Y, CAMERA_Z;
const PREFERRED_M = -0.3079928;                                     //  IMPLEMENTATION-SPECIFIC:
const PREFERRED_C = 617.1691;                                       //  CAMERA_Z = M(minimum-screen-dimension) + C
                                                                    //  Keep the board at a comfortable but ample visual distance.
//////////////////////////////////////////////////////////////////////  Board squares
var boardSquares = [];                                              //  Array of PlaneGeometry objects
var normalMaterials = [];                                           //  Array of texture maps
var selectedMaterials = [];                                         //  Array of texture maps
var targetedMaterials = [];                                         //  Array of texture maps

const BOARD_SQ_WIDTH = 8, BOARD_SQ_HEIGHT = 8;                      //  IMPLEMENTATION-SPECIFIC
const SQ_WIDTH = 22, SQ_HEIGHT = 22, SQ_W_SEG = 1, SQ_H_SEG = 1;
const SQ_OFFSET = 23;

//////////////////////////////////////////////////////////////////////  Board pieces
var pawnModelLoader, pawnGeometry;                                  //  Template loaders
var knightModelLoader, knightGeometry;
var bishopModelLoader, bishopGeometry;
var rookModelLoader, rookGeometry;
var queenModelLoader, queenGeometry;
var kingModelLoader, kingGeometry;
var gamePieces = [];

//////////////////////////////////////////////////////////////////////  Piece materials
var whiteMaterial, blackMaterial;

//////////////////////////////////////////////////////////////////////  Sounds
var select_mp3, deselect_mp3, commit_mp3, promote_mp3, chime_mp3, error_mp3, switch_mp3;

//////////////////////////////////////////////////////////////////////  Lights
var ambientLight;
var directionalLight1, directionalLight2;

//////////////////////////////////////////////////////////////////////  Game control
var MasterControl = false;                                          //  Shuts on/off all interactivity
var cuedToAnimate = false;
var animating = false;
var animationInstruction = null;

var gameOver = false;                                               //  Has it ended?

var HumansTurn = true;
var CurrentTurn = 'White';                                          //  Whose turn to play
var Select_A = _NOTHING;
var Select_B = _NOTHING;
var Castle_C = _NOTHING;                                            //  IMPLEMENTATION-SPECIFIC
var Castle_D = _NOTHING;                                            //  Used to store the castling A, B after the Tween
var PromotionTarget = 'x';                                          //  Tracks the choice of pawn promotion

//////////////////////////////////////////////////////////////////////  Game clock
var previousSecond = Date.now();                                    //  Track the last millisecond

//////////////////////////////////////////////////////////////////////  Game logic
var WhiteTeam = [], BlackTeam = [];                                 //  Arrays of indices
var Options = [];

//////////////////////////////////////////////////////////////////////  Game piece animation control
const GAMEPIECE_MOVEMENT_ZENITH = 100;
var animationTarget;                                                //  Maintains Mesh index when tweening
var animate_startPos;
var animate_midPos;
var animate_endPos;
var animate_startScale;
var animate_endScale;

//////////////////////////////////////////////////////////////////////  Load-targets
var elementsLoaded = 0;                                             //  Track objects to load
const ELEMENTS_TO_LOAD = 210;                                       //  64 squares: normal, selected, targeted + 6 meshes
                                                                    //  (DefaultLoadingManager excludes JSON meshes)
                                                                    //  +1 game logic WebASM module
                                                                    //  +1 evaluation WebASM module
                                                                    //  +1 tree-search WebASM module
                                                                    //  +1 Zobrist hasher
                                                                    //  +4 tech details (en, pl, es, de)
                                                                    //  +4 control panels (en, pl, es, de)
THREE.DefaultLoadingManager.onProgress = function(item, loaded, total)
  {
    elementsLoaded++;
    loadTotalReached();                                             //  Test for load complete
  };

init3D();
initScene();
initEvents();
initSounds();

//////////////////////////////////////////////////////////////////////
//   I N I T s
function init3D()
  {
    scene = new THREE.Scene();
    camera = new THREE.PerspectiveCamera(VIEW_ANGLE, ASPECT, NEAR, FAR);
    renderer = new THREE.WebGLRenderer( {alpha: true} );

    CAMERA_X = 0;
    CAMERA_Y = 0;
    CAMERA_Z = PREFERRED_M * Math.min(screenWidth, screenHeight) + PREFERRED_C;

    scene.add(camera);
    camera.position.set(CAMERA_X, CAMERA_Y, CAMERA_Z);
    renderer.setSize(screenWidth, screenHeight);
    container.append(renderer.domElement);

    document.querySelectorAll('canvas')[0].id = "interactivelayer"; //  Distinguish this canvas from the "flowfield" canvas

    var canvas = document.getElementById('interactivelayer');       //  Add event listener for context loss:
                                                                    //  put yourself back together
    canvas.addEventListener("webglcontextlost", restoreLostContext, false)

    renderer.render(scene, camera);
  }

function initScene()
  {
    initLights();
    initBoard();
    initPieces();
  }

function initLights()
  {
    ambientLight = new THREE.AmbientLight(0xc0c0c0);

    directionalLight1 = new THREE.DirectionalLight(0xffffff, 0.3);
    directionalLight1.position.set(1, -1, 1).normalize();

    directionalLight2 = new THREE.DirectionalLight(0xffffff, 0.1);
    directionalLight2.position.set(1, 1, -1).normalize();

    scene.add(ambientLight);
    scene.add(directionalLight1);
    scene.add(directionalLight2);
  }

function initBoard()
  {
    var i;
    var row_ctr = 0;
    var col_ctr = 0;

    for(i = _A1; i < _NOTHING; i++)
      {
        var normalmaterial = new THREE.MeshLambertMaterial({
            map: THREE.ImageUtils.loadTexture('obj/img/board/' + COLS[col_ctr] + ROWS[row_ctr] + '.png')
          });
        var selectedmaterial = new THREE.MeshLambertMaterial({
            map: THREE.ImageUtils.loadTexture('obj/img/board/' + COLS[col_ctr] + ROWS[row_ctr] + 's.png')
          });
        var targetedmaterial = new THREE.MeshLambertMaterial({
            map: THREE.ImageUtils.loadTexture('obj/img/board/' + COLS[col_ctr] + ROWS[row_ctr] + 't.png')
          });
        normalMaterials.push(normalmaterial);
        selectedMaterials.push(selectedmaterial);
        targetedMaterials.push(targetedmaterial);

        col_ctr++;
        if(col_ctr > BOARD_SQ_WIDTH - 1)
          {
            col_ctr = 0;
            row_ctr++;
          }
      }
  }

function buildBoard()
  {
    var i;
    var x_offset = -80;
    var y_offset = -80;
    var target_x = 0;
    var target_y = 0;
    var row_ctr = 0;
    var col_ctr = 0;

    var planegeom = new THREE.PlaneGeometry(SQ_WIDTH, SQ_HEIGHT, SQ_W_SEG, SQ_H_SEG);

    for(i = _A1; i < _NOTHING; i++)
      {
        var plane = new THREE.Mesh(planegeom, normalMaterials[i]);
        plane.name = i;
        plane.position.set(target_x + x_offset, target_y + y_offset, 0);
        boardSquares.push(plane);
        scene.add(boardSquares[boardSquares.length - 1]);
        target_x += SQ_OFFSET;
        col_ctr++;
        if(target_x > (SQ_OFFSET * (BOARD_SQ_WIDTH - 1)))
          {
            target_x = 0;
            target_y += SQ_OFFSET;
            col_ctr = 0;
            row_ctr++;
          }
      }
  }

function initPieces()
  {
    whiteMaterial = new THREE.MeshLambertMaterial(
      {
        color: 0xEDF5FF,
        emissive: 0x000000
      });
    blackMaterial = new THREE.MeshPhongMaterial(
      {
        color: 0x383838,
        emissive: 0x000000,
        specular: 0xB9B9B9
      });

    pawnModelLoader = new THREE.JSONLoader();
    pawnModelLoader.load('obj/collada/Pawn.json', function(geometry)
      {
        pawnGeometry = geometry;
        elementsLoaded++;
        loadTotalReached();
      });

    knightModelLoader = new THREE.JSONLoader();
    knightModelLoader.load('obj/collada/Knight.json', function(geometry)
      {
        knightGeometry = geometry;
        elementsLoaded++;
        loadTotalReached();
      });

    bishopModelLoader = new THREE.JSONLoader();
    bishopModelLoader.load('obj/collada/Bishop.json', function(geometry)
      {
        bishopGeometry = geometry;
        elementsLoaded++;
        loadTotalReached();
      });

    rookModelLoader = new THREE.JSONLoader();
    rookModelLoader.load('obj/collada/Rook.json', function(geometry)
      {
        rookGeometry = geometry;
        elementsLoaded++;
        loadTotalReached();
      });

    queenModelLoader = new THREE.JSONLoader();
    queenModelLoader.load('obj/collada/Queen.json', function(geometry)
      {
        queenGeometry = geometry;
        elementsLoaded++;
        loadTotalReached();
      });

    kingModelLoader = new THREE.JSONLoader();
    kingModelLoader.load('obj/collada/King.json', function(geometry)
      {
        kingGeometry = geometry;
        elementsLoaded++;
        loadTotalReached();
      });
  }

function initPiece(symbol, index, geometry)
  {
    if(symbol == symbol.toUpperCase())                              //  White team
      {
        WhiteTeam.push(index);
        switch(symbol)
          {
            case 'P': gamePieces.push(new THREE.Mesh(geometry, whiteMaterial));
                      gamePieces[gamePieces.length - 1].chessteam = 'White';
                      gamePieces[gamePieces.length - 1].chessrank = 'Pawn';
                      break;
            case 'N': gamePieces.push(new THREE.Mesh(geometry, whiteMaterial));
                      gamePieces[gamePieces.length - 1].rotation.y = 90 * (Math.PI/180);
                      gamePieces[gamePieces.length - 1].chessteam = 'White';
                      gamePieces[gamePieces.length - 1].chessrank = 'Knight';
                      break;
            case 'B': gamePieces.push(new THREE.Mesh(geometry, whiteMaterial));
                      gamePieces[gamePieces.length - 1].chessteam = 'White';
                      gamePieces[gamePieces.length - 1].chessrank = 'Bishop';
                      break;
            case 'R': gamePieces.push(new THREE.Mesh(geometry, whiteMaterial));
                      gamePieces[gamePieces.length - 1].chessteam = 'White';
                      if(index == _A1)
                        gamePieces[gamePieces.length - 1].chessrank = 'QRook';
                      else
                        gamePieces[gamePieces.length - 1].chessrank = 'KRook';
                      break;
            case 'Q': gamePieces.push(new THREE.Mesh(geometry, whiteMaterial));
                      gamePieces[gamePieces.length - 1].chessteam = 'White';
                      gamePieces[gamePieces.length - 1].chessrank = 'Queen';
                      break;
            case 'K': gamePieces.push(new THREE.Mesh(geometry, whiteMaterial));
                      gamePieces[gamePieces.length - 1].chessteam = 'White';
                      gamePieces[gamePieces.length - 1].chessrank = 'King';
                      break;
          }
      }
    else                                                            //  Black team
      {
        BlackTeam.push(index);
        switch(symbol)
          {
            case 'p': gamePieces.push(new THREE.Mesh(geometry, blackMaterial));
                      gamePieces[gamePieces.length - 1].chessteam = 'Black';
                      gamePieces[gamePieces.length - 1].chessrank = 'Pawn';
                      break;
            case 'n': gamePieces.push(new THREE.Mesh(geometry, blackMaterial));
                      gamePieces[gamePieces.length - 1].rotation.y = -90 * (Math.PI/180);
                      gamePieces[gamePieces.length - 1].chessteam = 'Black';
                      gamePieces[gamePieces.length - 1].chessrank = 'Knight';
                      break;
            case 'b': gamePieces.push(new THREE.Mesh(geometry, blackMaterial));
                      gamePieces[gamePieces.length - 1].chessteam = 'Black';
                      gamePieces[gamePieces.length - 1].chessrank = 'Bishop';
                      break;
            case 'r': gamePieces.push(new THREE.Mesh(geometry, blackMaterial));
                      gamePieces[gamePieces.length - 1].chessteam = 'Black';
                      if(index == _A8)
                        gamePieces[gamePieces.length - 1].chessrank = 'QRook';
                      else
                        gamePieces[gamePieces.length - 1].chessrank = 'KRook';
                      break;
            case 'q': gamePieces.push(new THREE.Mesh(geometry, blackMaterial));
                      gamePieces[gamePieces.length - 1].chessteam = 'Black';
                      gamePieces[gamePieces.length - 1].chessrank = 'Queen';
                      break;
            case 'k': gamePieces.push(new THREE.Mesh(geometry, blackMaterial));
                      gamePieces[gamePieces.length - 1].chessteam = 'Black';
                      gamePieces[gamePieces.length - 1].chessrank = 'King';
                      break;
          }
      }
    gamePieces[gamePieces.length - 1].scale.x = 25;
    gamePieces[gamePieces.length - 1].scale.y = 25;
    gamePieces[gamePieces.length - 1].scale.z = 25;
    gamePieces[gamePieces.length - 1].rotation.x = 90 * (Math.PI/180);
    gamePieces[gamePieces.length - 1].position.x = convIndexToX(index);
    gamePieces[gamePieces.length - 1].position.y = convIndexToY(index);
    gamePieces[gamePieces.length - 1].chessposition = index;

    scene.add(gamePieces[gamePieces.length - 1]);
  }

function initSounds()
  {
    select_mp3 = new Audio('obj/mp3/select.mp3');
    deselect_mp3 = new Audio('obj/mp3/deselect.mp3');
    commit_mp3 = new Audio('obj/mp3/commit.mp3');
    promote_mp3 = new Audio('obj/mp3/promote.mp3');
    chime_mp3 = new Audio('obj/mp3/chime.mp3');
    error_mp3 = new Audio('obj/mp3/error.mp3');
    switch_mp3 = new Audio('obj/mp3/switch.mp3');
  }

function initEvents()
  {
    window.addEventListener('resize', onWindowResize, false);
    window.addEventListener('mousedown', onClick, false);
    document.addEventListener('touchstart', onTouch, false);
  }

//////////////////////////////////////////////////////////////////////
//   L O A D I N G
function loadTotalReached()
  {
    var element = document.getElementById('loadingbanner');
    var ldBanner = document.getElementById('percentLoaded');

    if(elementsLoaded == ELEMENTS_TO_LOAD)
      {
        element.parentNode.removeChild(element);                    //  Remove "Loading . . ." banner

        buildBoard();
        begin();

        reqSess();                                                  //  Load initial byte array into GameState buffer.
      }
    else
      ldBanner.innerHTML = Math.round(elementsLoaded / ELEMENTS_TO_LOAD * 100) + ' %';
  }

//////////////////////////////////////////////////////////////////////
//   L A U N C H
function begin()
  {
    var tech = document.getElementById('tech-details');
    var theGui = document.getElementById('control-panel');

    resetCameraPositionAngle(36);                                   //  Default camera angle (the nicest in my opinion)
    render();                                                       //  Begin animation

    switch(currentLang)                                             //  Load interface components
      {
        case 'Polish':  tech.innerHTML = techDetails_pl;
                        theGui.innerHTML = gui_pl;
                        break;
        case 'Spanish': tech.innerHTML = techDetails_es;
                        theGui.innerHTML = gui_es;
                        break;
        case 'German':  tech.innerHTML = techDetails_de;
                        theGui.innerHTML = gui_de;
                        break;
        default:        tech.innerHTML = techDetails_en;
                        theGui.innerHTML = gui_en;
                        break;
      }
                                                                    //  If fullscreen is not available on this
    if(!fullscreenAvailable)                                        //  device, then hide the switch
      document.getElementById('fullscreen-row').style.display = "none";

    setPanelToggleReveal(true);                                     //  Enable panel toggle

    MasterControl = true;                                           //  BEGIN !!
  }

//////////////////////////////////////////////////////////////////////
//   R E S T O R E   W E B G L   C O N T E X T
function restoreLostContext(e)
  {
    e.preventDefault();                                             //  Prevent default action

    if(DEBUG_VERBOSE)
      console.log('WebGL context crashed');

    //cancelRequestAnimationFrame(requestId);
  }

//////////////////////////////////////////////////////////////////////
//   B O A R D   S Q U A R E   U T I L s
function convIndexToX(i)
  {
    return ((i % BOARD_SQ_WIDTH) * SQ_OFFSET) - 80;
  }

function convIndexToY(i)
  {
    return (((i - (i % BOARD_SQ_WIDTH)) / BOARD_SQ_WIDTH) * SQ_OFFSET) - 80;
  }

function midpoint(a, b)
  {
    return (a + b) / 2;
  }

//////////////////////////////////////////////////////////////////////
//   B O A R D   S Q U A R E   M A T E R I A L S
function normalSq(i)
  {
    i = typeof i !== 'undefined' ? i : _NOTHING;
    var ctr, j;

    if(i == _NOTHING)
      {
        ctr = _A1;
        j = _H8;
      }
    else
      {
        ctr = i;
        j = i;
      }

    for(; ctr <= j; ctr++)
      {
        boardSquares[ctr].material = normalMaterials[ctr];
      }
  }

function selectedSq(i)
  {
    i = typeof i !== 'undefined' ? i : _NOTHING;
    var ctr, j;

    if(i == _NOTHING)
      {
        ctr = _A1;
        j = _H8;
      }
    else
      {
        ctr = i;
        j = i;
      }

    for(; ctr <= j; ctr++)
      {
        boardSquares[ctr].material = selectedMaterials[ctr];
      }
  }

function targetedSq(i)
  {
    i = typeof i !== 'undefined' ? i : _NOTHING;
    var ctr, j;

    if(i == _NOTHING)
      {
        ctr = _A1;
        j = _H8;
      }
    else
      {
        ctr = i;
        j = i;
      }

    for(; ctr <= j; ctr++)
      {
        boardSquares[ctr].material = targetedMaterials[ctr];
      }
  }

//////////////////////////////////////////////////////////////////////
//   C L I C K S
function validTarget(j)
  {
    var i = 0;
    while(i < Options.length && (Options[i].src != Select_A || Options[i].dst != j))
      i++;
    if(i < Options.length)
      return true;
    return false;
  }

function onClick(e)
  {
    if(!gameOver)
      {
        var vector = new THREE.Vector3((e.clientX / screenWidth) * 2 - 1,
                                     - (e.clientY / screenHeight) * 2 + 1,
                                       1);
        vector.unproject(camera);

        var ray = new THREE.Raycaster(camera.position, vector.sub(camera.position).normalize());
        var intersects = ray.intersectObjects(boardSquares);

        selection(intersects);
      }
  }

function onTouch(e)
  {
    if(!gameOver && e.touches.length == 1)
      {
        var vector = new THREE.Vector3((e.touches[0].pageX / screenWidth) * 2 - 1,
                                     - (e.touches[0].pageY / screenHeight) * 2 + 1,
                                       1);
        vector.unproject(camera);

        var ray = new THREE.Raycaster(camera.position, vector.sub(camera.position).normalize());
        var intersects = ray.intersectObjects(boardSquares);

        selection(intersects);
      }
  }

function selection(intersects)
  {
    //  LEFT OFF HERE !!! ***
  }

//////////////////////////////////////////////////////////////////////
//   S C R E E N   E V E N T S
function onWindowResize(e)
  {
    screenWidth = window.innerWidth;
    screenHeight = window.innerHeight;

    ASPECT = screenWidth / screenHeight;
    camera.aspect = ASPECT;
    camera.updateProjectionMatrix();
    renderer.setSize(screenWidth, screenHeight);

    CAMERA_Z = PREFERRED_M * Math.min(screenWidth, screenHeight) + PREFERRED_C;
    resetCameraPositionAngle(angle);                                //  Set this to itself just so that it can redraw
  }

function toggleFullscreen(b)
  {
    if(b)
      THREEx.FullScreen.request();
    else
      THREEx.FullScreen.cancel();
  }

//////////////////////////////////////////////////////////////////////
//   C A M E R A   M O V E M E N T
function resetCameraPositionAngle(a)
  {
    angle = a;

    if(philadelphia.team == 'Black')
      {
        camera.position.z = Math.cos(-a * (Math.PI / 180)) * CAMERA_Z;
        camera.position.y = CAMERA_Y + Math.sin(-a * (Math.PI / 180)) * CAMERA_Z;
        camera.rotation.x = a * (Math.PI / 180);
      }
    else
      {
        camera.position.z = Math.cos(-a * (Math.PI / 180)) * CAMERA_Z;
        camera.position.y = CAMERA_Y + Math.sin(a * (Math.PI / 180)) * CAMERA_Z;
        camera.rotation.x = -a * (Math.PI / 180);
      }
  }
