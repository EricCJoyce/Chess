const STATUS_UNSEARCHED = 0;
const STATUS_SEARCHING = 1;
const STATUS_DONE_BOOK = 2;
const STATUS_DONE_SEARCH = 3;

/* A.I. Player class for CHESS. */
class Player
  {
    constructor()
      {
        this.team = 'Black';                                        //  In {'White', 'Black'}. Default to Black.
        this.ply = 3;                                               //  Depth to which this A.I. should search.

        this.searchId = 0;                                          //  Each negamax run gets a unique ID.

        this.branches = [];                                         //  Array of Objects, each {ByteArray(GameState), uchar(depth), ByteArray(Move)}.
        this.branchIterator = 0;                                    //                          The state in which    Depth to      The agent's reply.
                                                                    //                          the agent may act.    which agent   (Having searched.)
                                                                    //                                                has searched.
        //////////////////////////////////////////////////////////////  The evaluation engine.
        this.evaluationEngine = null;                               //  WebAssembly Module containing evaluation functions.

        this.evaluationInputGameStateOffset = null;                 //  (Offset into Module memory.)
        this.evaluationInputGameStateBuffer = null;                 //  ByteArray: Input buffer for an encoded query-gamestate.

        this.evaluationInputMoveOffset = null;                      //  (Offset into Module memory.)
        this.evaluationInputMoveBuffer = null;                      //  ByteArray: Input buffer for an encoded query-move.

        this.evaluationOutputGameStateOffset = null;                //  (Offset into Module memory.)
        this.evaluationOutputGameStateBuffer = null;                //  ByteArray: Output buffer for an encoded answer-gamestate.
                                                                    //  (You would obtain a single gamestate, for instance, after calling makeMove().)

        this.evaluationOutputMovesOffset = null;                    //  (Offset into Module memory.)
        this.evaluationOutputMovesBuffer = null;                    //  ByteArray: Output buffer for encoded answer-moves.
                                                                    //  (You would obtain several moves, for instance, after calling getMoves().)

        //////////////////////////////////////////////////////////////  The negamax engine.
        this.negamaxEngine = null;                                  //  WebAssembly Module containing the negamax search engine.

        this.negamaxInputOffset = null;                             //  (Offset into Module memory.)
        this.negamaxInputBuffer = null;                             //  ByteArray: Input buffer for tree search. Encode a query-gamestate here.
                                                                    //             When Player asks negamax something.
        this.negamaxParamsOffset = null;                            //  (Offset into Module memory.)
        this.negamaxParamsBuffer = null;                            //  ByteArray: Input buffer for search parameters.

        this.negamaxOutputOffset = null;                            //  (Offset into Module memory.)
        this.negamaxOutputBuffer = null;                            //  ByteArray: Output buffer for tree search. Decode an answer from here.
                                                                    //             When negamax answers Player.
        this.negamaxQueryGameStateOffset = null;                    //  (Offset into Module memory.)
        this.negamaxQueryGameStateBuffer = null;                    //  ByteArray: Used by the negamax module to pass encoded game states to the evaluation module.

        this.negamaxQueryMoveOffset = null;                         //  (Offset into Module memory.)
        this.negamaxQueryMoveBuffer = null;                         //  ByteArray: Used by the negamax module to pass encoded moves to the evaluation module.

        this.negamaxAnswerGameStateOffset = null;                   //  (Offset into Module memory.)
        this.negamaxAnswerGameStateBuffer = null;                   //  ByteArray: Used by the negamax module to receive encoded game states from the evaluation module.

        this.negamaxAnswerMovesOffset = null;                       //  (Offset into Module memory.)
        this.negamaxAnswerMovesBuffer = null;                       //  ByteArray: Used by the negamax module to receive encoded moves from the evaluation module.

        this.ZobristHashOffset = null;                              //  (Offset into Module memory.)
        this.ZobristHashBuffer = null;                              //  ByteArray representation of the Zobrist hasher.

        this.TranspositionTableOffset = null;                       //  (Offset into Module memory.)
        this.TranspositionTableBuffer = null;                       //  ByteArray representation of the transposition table.

        this.negamaxSearchOffset = null;                            //  (Offset into Module memory.)
        this.negamaxSearchBuffer = null;                            //  ByteArray: Working node buffer for tree-search.

        this.negamaxMovesOffset = null;                             //  (Offset into Module memory.)
        this.negamaxMovesBuffer = null;                             //  ByteArray: Working move buffer for tree-search.

        this.negamaxKillerMovesOffset = null;                       //  (Offset into Module memory.)
        this.negamaxKillerMovesBuffer = null;                       //  ByteArray representation of the killer-moves table.

        this.negamaxHistoryHeuristicOffset = null;                  //  (Offset into Module memory.)
        this.negamaxHistoryHeuristicBuffer = null;                  //  ByteArray representation of the history-heuristic table.

        //////////////////////////////////////////////////////////////  Book lookup.
        this.bookLookup = new XMLHttpRequest();                     //  IE 7+, Firefox, Chrome, Opera, Safari
        this.callOut = false;                                       //  True while AJAX call has yet to return.

        //////////////////////////////////////////////////////////////  Fetch, instantiate, and connect the Evaluation Module.
        fetch('obj/wasm/eval.wasm', {headers: {'Content-Type': 'application/wasm'} })
        .then(response => response.arrayBuffer())
        .then(bytes =>
          {
            WebAssembly.instantiate(bytes,
              {
                env: {
                       memoryBase: 0,
                       tableBase: 0,
                                                                    //  Malloc 2 pages for 71.8 KB file.
                       memory: new WebAssembly.Memory({initial: 2, maximum: 2}),
                       table: new WebAssembly.Table({initial: 0, element: 'anyfunc'})
                     }
              })
            .then(instance =>
              {
                this.evaluationEngine = instance;
                                                                    //  Assign offset to input-gamestate buffer.
                this.evaluationInputGameStateOffset = this.evaluationEngine.instance.exports.getInputGameStateBuffer();
                this.evaluationInputGameStateBuffer = new Uint8Array(this.evaluationEngine.instance.exports.memory.buffer, this.evaluationInputGameStateOffset, _GAMESTATE_BYTE_SIZE);
                                                                    //  Assign offset to input-move buffer.
                this.evaluationInputMoveOffset = this.evaluationEngine.instance.exports.getInputMoveBuffer();
                this.evaluationInputMoveBuffer = new Uint8Array(this.evaluationEngine.instance.exports.memory.buffer, this.evaluationInputMoveOffset, _MOVE_BYTE_SIZE);
                                                                    //  Assign offset to output-gamestate buffer.
                this.evaluationOutputGameStateOffset = this.evaluationEngine.instance.exports.getOutputGameStateBuffer();
                this.evaluationOutputGameStateBuffer = new Uint8Array(this.evaluationEngine.instance.exports.memory.buffer, this.evaluationOutputGameStateOffset, _GAMESTATE_BYTE_SIZE);
                                                                    //  Assign offset to output-moves buffer.
                this.evaluationOutputMovesOffset = this.evaluationEngine.instance.exports.getOutputMovesBuffer();
                this.evaluationOutputMovesBuffer = new Uint8Array(this.evaluationEngine.instance.exports.memory.buffer, this.evaluationOutputMovesOffset, _MAX_MOVES * (_MOVE_BYTE_SIZE + 5));

                elementsLoaded++;                                   //  Check evaluationEngine off our list.
                                                                    //  Load the tree-search module AFTER the evaluation module is complete.
                                                                    //  Fetch, instantiate, and connect the Negamax Module.
                fetch('obj/wasm/negamax.wasm', {headers: {'Content-Type': 'application/wasm'} })
                .then(response => response.arrayBuffer())
                .then(bytes =>
                  {
                    WebAssembly.instantiate(bytes,
                      {
                        env: {
                               memoryBase: 0,
                               tableBase: 0,
                                                                    //  Compiled with -s INITIAL_MEMORY=16121856 = 246 pages.
                               memory: new WebAssembly.Memory({initial: 246}),
                               table: new WebAssembly.Table({initial: 1, element: 'anyfunc'}),
                               _copyQuery2EvalGSInput: function()
                                 {
                                   var i;                           //  Copy negamaxEngine's query buffer contents
                                                                    //  to evaluationEngine's input-gamestate buffer.
                                   for(i = 0; i < _GAMESTATE_BYTE_SIZE; i++)
                                     this.evaluationInputGameStateBuffer[i] = this.negamaxQueryGameStateBuffer[i];
                                   return;
                                 }.bind(this),
                               _copyQuery2EvalMoveInput: function()
                                 {
                                   var i;                           //  Copy negamaxEngine's query buffer contents
                                                                    //  to evaluationEngine's input-move buffer.
                                   for(i = 0; i < _MOVE_BYTE_SIZE; i++)
                                     this.evaluationInputMoveBuffer[i] = this.negamaxQueryMoveBuffer[i];
                                   return;
                                 }.bind(this),
                               _copyEvalOutput2AnswerGSBuffer: function()
                                 {
                                   var i;                           //  Copy evaluationEngine's output buffer contents
                                                                    //  to negamaxEngine's answer-gamestate buffer.
                                   for(i = 0; i < _GAMESTATE_BYTE_SIZE; i++)
                                     this.negamaxAnswerGameStateBuffer[i] = this.evaluationOutputGameStateBuffer[i];
                                   return;
                                 }.bind(this),
                               _copyEvalOutput2AnswerMovesBuffer: function(len)
                                 {
                                   var i;
                                   const size = _MOVE_BYTE_SIZE + 5;//  Copy evaluationEngine's output buffer contents
                                   for(i = 0; i < len * size; i++)  //  to negamaxEngine's answer-moves buffer.
                                     this.negamaxAnswerMovesBuffer[i] = this.evaluationOutputMovesBuffer[i];
                                   return;
                                 }.bind(this),
                               _sideToMove: function()              //  Returns unsigned char.
                                 {
                                   return this.evaluationEngine.instance.exports.sideToMove_eval();
                                 }.bind(this),
                               _isQuiet: function()                 //  Returns bool.
                                 {
                                   return this.evaluationEngine.instance.exports.isQuiet_eval();
                                 }.bind(this),
                               _isTerminal: function()              //  Returns bool.
                                 {
                                   return this.evaluationEngine.instance.exports.isTerminal_eval();
                                 }.bind(this),
                               _isSideToMoveInCheck: function()     //  Returns bool.
                                 {
                                   return this.evaluationEngine.instance.exports.isSideToMoveInCheck_eval();
                                 }.bind(this),
                               _nonPawnMaterial: function()         //  Returns unsigned char.
                                 {
                                   return this.evaluationEngine.instance.exports.nonPawnMaterial_eval();
                                 }.bind(this),
                               _makeMove: function()
                                 {
                                   this.evaluationEngine.instance.exports.makeMove_eval();
                                   return;
                                 }.bind(this),
                               _makeNullMove: function()
                                 {
                                   this.evaluationEngine.instance.exports.makeNullMove_eval();
                                   return;
                                 }.bind(this),
                               _evaluate: function()                //  Returns float.
                                 {
                                   if(this.team == 'Black')
                                     return this.evaluationEngine.instance.exports.evaluate_eval(false);
                                   else
                                     return this.evaluationEngine.instance.exports.evaluate_eval(true);
                                 }.bind(this),
                               _getMoves: function()                //  Returns unsigned int.
                                 {
                                   return this.evaluationEngine.instance.exports.getMoves_eval();
                                 }.bind(this)
                             }
                      })
                    .then(instance =>
                      {
                        var i;

                        this.negamaxEngine = instance;
                                                                    //  Assign offset to input buffer. This receives a game state as a byte array.
                        this.negamaxInputOffset = this.negamaxEngine.instance.exports.getInputBuffer();
                        this.negamaxInputBuffer = new Uint8Array(this.negamaxEngine.instance.exports.memory.buffer, this.negamaxInputOffset, _GAMESTATE_BYTE_SIZE);
                                                                    //  Assign offset to parameters buffer.
                        this.negamaxParamsOffset = this.negamaxEngine.instance.exports.getParametersBuffer();
                        this.negamaxParamsBuffer = new Uint8Array(this.negamaxEngine.instance.exports.memory.buffer, this.negamaxParamsOffset, _PARAMETER_ARRAY_SIZE);
                                                                    //  Assign offset to output buffer.
                                                                    //  This receives a game state, a 1-byte uchar (depth achieved), a move, a 4-byte float (score).
                        this.negamaxOutputOffset = this.negamaxEngine.instance.exports.getOutputBuffer();
                        this.negamaxOutputBuffer = new Uint8Array(this.negamaxEngine.instance.exports.memory.buffer, this.negamaxOutputOffset, _GAMESTATE_BYTE_SIZE + 1 + _MOVE_BYTE_SIZE + 4);
                                                                    //  Assign offset to input buffer. This receives a game state as a byte array.
                        this.negamaxQueryGameStateOffset = this.negamaxEngine.instance.exports.getQueryGameStateBuffer();
                        this.negamaxQueryGameStateBuffer = new Uint8Array(this.negamaxEngine.instance.exports.memory.buffer, this.negamaxQueryGameStateOffset, _GAMESTATE_BYTE_SIZE);
                                                                    //  Assign offset to input buffer. This receives a game state as a byte array.
                        this.negamaxQueryMoveOffset = this.negamaxEngine.instance.exports.getQueryMoveBuffer();
                        this.negamaxQueryMoveBuffer = new Uint8Array(this.negamaxEngine.instance.exports.memory.buffer, this.negamaxQueryMoveOffset, _MOVE_BYTE_SIZE);
                                                                    //  Assign offset to auxiliary buffer.
                                                                    //  This is a receiving buffer, temporarily holding output from the evaluation module before converting these data to negamax nodes.
                        this.negamaxAnswerGameStateOffset = this.negamaxEngine.instance.exports.getAnswerGameStateBuffer();
                        this.negamaxAnswerGameStateBuffer = new Uint8Array(this.negamaxEngine.instance.exports.memory.buffer, this.negamaxAnswerGameStateOffset, _GAMESTATE_BYTE_SIZE);
                                                                    //  Assign offset to auxiliary buffer.
                                                                    //  This is a receiving buffer, temporarily holding output from the evaluation module before converting these data to negamax nodes.
                        this.negamaxAnswerMovesOffset = this.negamaxEngine.instance.exports.getAnswerMovesBuffer();
                        this.negamaxAnswerMovesBuffer = new Uint8Array(this.negamaxEngine.instance.exports.memory.buffer, this.negamaxAnswerMovesOffset, _MAX_MOVES * (_MOVE_BYTE_SIZE + 5));
                                                                    //  Assign offset to Zobrist hash buffer.
                                                                    //  This receives 8 * _ZHASH_TABLE_SIZE bytes. Sections of 8 bytes treated as unsigned long longs.
                        this.ZobristHashOffset = this.negamaxEngine.instance.exports.getZobristHashBuffer();
                        this.ZobristHashBuffer = new Uint8Array(this.negamaxEngine.instance.exports.memory.buffer, this.ZobristHashOffset, _HASH_VALUE_BYTE_SIZE * _ZHASH_TABLE_SIZE);
                                                                    //  Assign offset to Transposition Table buffer.
                        this.TranspositionTableOffset = this.negamaxEngine.instance.exports.getTranspositionTableBuffer();
                        this.TranspositionTableBuffer = new Uint8Array(this.negamaxEngine.instance.exports.memory.buffer, this.TranspositionTableOffset, 1 + _TRANSPO_TABLE_SIZE * _TRANSPO_RECORD_BYTE_SIZE);
                                                                    //  Assign offset to tree-search buffer.
                                                                    //  This is a working buffer that receives bytes from the evaluation engine.
                        this.negamaxSearchOffset = this.negamaxEngine.instance.exports.getNegamaxSearchBuffer();
                        this.negamaxSearchBuffer = new Uint8Array(this.negamaxEngine.instance.exports.memory.buffer, this.negamaxSearchOffset, 4 + _TREE_SEARCH_ARRAY_SIZE * _NEGAMAX_NODE_BYTE_SIZE);
                                                                    //  Assign offset to tree-search buffer.
                                                                    //  This is a working buffer that receives bytes from the evaluation engine.
                        this.negamaxMovesOffset = this.negamaxEngine.instance.exports.getNegamaxMovesBuffer();
                        this.negamaxMovesBuffer = new Uint8Array(this.negamaxEngine.instance.exports.memory.buffer, this.negamaxMovesOffset, 4 + _TREE_SEARCH_ARRAY_SIZE * _NEGAMAX_MOVE_BYTE_SIZE);
                                                                    //  Assign offset to auxiliary buffer.
                                                                    //  This is a working buffer that holds the killer-moves table.
                        this.negamaxKillerMovesOffset = this.negamaxEngine.instance.exports.getKillerMovesBuffer();
                        this.negamaxKillerMovesBuffer = new Uint8Array(this.negamaxEngine.instance.exports.memory.buffer, this.negamaxKillerMovesOffset, _KILLER_MOVE_PER_PLY * 2 * _KILLER_MOVE_MAX_DEPTH);
                                                                    //  Assign offset to auxiliary buffer.
                                                                    //  This is a working buffer that holds the history-heuristic table.
                        this.negamaxHistoryHeuristicOffset = this.negamaxEngine.instance.exports.getHistoryTableBuffer();
                        this.negamaxHistoryHeuristicBuffer = new Uint8Array(this.negamaxEngine.instance.exports.memory.buffer, this.negamaxHistoryHeuristicOffset, 2 * (_NOTHING + _NOTHING));

                        for(i = 0; i < 4; i++)                      //  Blank out the first four bytes; let the rest be trash.
                          this.TranspositionTableBuffer[i] = 0;

                        this.TranspositionTableBuffer[0] = 1;       //  Set "generation byte" to 1.

                        elementsLoaded++;                           //  Check negaMaxEngine off our list.
                                                                    //  Load a Zobrist Hasher.
                        var ReqXML = new XMLHttpRequest();          //  IE 7+, Firefox, Chrome, Opera, Safari.
                        var params = 'sendRequest=eggsnhash';
                        ReqXML.parent = this;                       //  Add a reference to the Player object inside this callback function.

                        ReqXML.open("POST", 'obj/sess/reqzhash.php', true);
                        ReqXML.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
                        ReqXML.onreadystatechange = function()
                          {
                            if(ReqXML.readyState == 4 && ReqXML.status == 200)
                              {
                                if(ReqXML.responseText == "")       //  Null return: unknown error.
                                  {
                                    switch(currentLang)
                                      {
                                        case "Spanish": alert(alertStringScrub("Error"));  break;
                                        case "German": alert(alertStringScrub("Fehler"));  break;
                                        case "Polish": alert(alertStringScrub("B&#322;&#261;d na stronie"));  break;
                                        default: alert(alertStringScrub("Error"));
                                      }
                                  }
                                else
                                  {
                                    var parse = ReqXML.responseText.split('|');
                                    var arr;
                                    var i;
                                    if(parse[0] == 'chess' && parse[1] == 'ok')
                                      {
                                        parse = parse[2].split(',');//  Repurpose "parse".
                                        console.log(parse[0] + ' Zobrist keys.');
                                                                    //  Load into Zobrist-hasher's buffer.
                                        for(i = 1; i < parse.length; i++)
                                          {
                                            arr = new Uint8Array(1);//  Force byte type.
                                            arr[0] = parseInt(parse[i]);
                                            this.parent.ZobristHashBuffer[i] = arr[0];
                                          }

                                        elementsLoaded++;           //  Check Zobrist hasher off our list.
                                        loadTotalReached();         //  Check the total.
                                      }
                                    else                            //  Error-label or garbage.
                                      {
                                        switch(currentLang)
                                          {
                                            case "Spanish": alert(alertStringScrub("Error"));  break;
                                            case "German": alert(alertStringScrub("Fehler"));  break;
                                            case "Polish": alert(alertStringScrub("B&#322;&#261;d na stronie"));  break;
                                            default: alert(alertStringScrub("Error"));
                                          }
                                      }
                                  }
                              }
                          };
                        ReqXML.send(params);
                      });
                  });
              });
          });
      }

    /* Collect all moves the opponent might make, and prepare for each, an Object for the negamax search routine. */
    branch()
      {
        var i, j, len;
        var moves;
        var buffer4 = new Uint8Array(4);

        for(i = 0; i < _GAMESTATE_BYTE_SIZE; i++)                   //  Copy the current game-state byte-array to the player's evaluation engine's input buffer.
          this.evaluationInputGameStateBuffer[i] = gameStateBuffer[i];
                                                                    //  Tell the evaluation engine to get a list of possible moves.
        len = this.evaluationEngine.instance.exports.getMoves_eval();

        moves = [];                                                 //  Parse encoded moves and metadata.
        for(i = 0; i < len; i++)
          {
            moves.push( {move: new Uint8Array(_MOVE_BYTE_SIZE), score: 0} );
            for(j = 0; j < _MOVE_BYTE_SIZE; j++)
              moves[moves.length - 1].move[j] = this.evaluationOutputMovesBuffer[i * (_MOVE_BYTE_SIZE + 5) + j];
            for(j = 0; j < 4; j++)
              buffer4[j] = this.evaluationOutputMovesBuffer[i * (_MOVE_BYTE_SIZE + 5) + _MOVE_BYTE_SIZE + j];
            const dv = new DataView(buffer4.buffer, buffer4.byteOffset, 4);
            moves[moves.length - 1].score = dv.getUint32(0, true);  //  Little-endian.
          }

        moves.sort((a, b) => b.score - a.score);                    //  Sort descending by score.

        this.branches = [];                                         //  Reset the array.
        for(i = 0; i < len; i++)                                    //  Transfer results from the evaluation engine to the array of objects.
          {
                                                                    //  Game state.
                                                                    //  Best move found for this state.
                                                                    //  Depth searched/achieved.
                                                                    //  Whether we already tried the DB lookup.
            this.branches.push( {gamestate: new Uint8Array(_GAMESTATE_BYTE_SIZE),
                                 bestMove:  new Uint8Array(_MOVE_BYTE_SIZE),
                                 depth:     0,
                                 status:    STATUS_UNSEARCHED,
                                 score:     0.0} );

            for(j = 0; j < _MOVE_BYTE_SIZE; j++)                    //  Copy the candidate move to the evaluation module's move-input.
              this.evaluationInputMoveBuffer[j] = moves[i].move[j];

            this.evaluationEngine.instance.exports.makeMove_eval(); //  Apply the candidate move to the game state still in Evaluation's input buffer.

            for(j = 0; j < _GAMESTATE_BYTE_SIZE; j++)               //  Copy the encoded, resultant game state from Evaluation's output to the branch object.
              this.branches[this.branches.length - 1].gamestate[j] = this.evaluationOutputGameStateBuffer[j];
                                                                    //  Write a blank Best-Move-Found-So-Far to the branch object.
            this.branches[this.branches.length - 1].bestMove[0] = _NOTHING;
            this.branches[this.branches.length - 1].bestMove[1] = _NOTHING;
            this.branches[this.branches.length - 1].bestMove[2] = 0;
          }

        this.branchIterator = 0;                                    //  Reset the branch iterator, point to the first (assumed best) move.
        console.log(len+' branches');

        return;
      }

    /* Set up the search routine for the object in "this.branches" under "this.branchIterator". */
    initSearch()
      {
        var i;

        if(this.branches[ this.branchIterator ].depth < this.ply)   //  Is search necessary?
          {
                                                                    //  First, copy the byte array from this.branches[ this.branchIterator ]
            for(i = 0; i < _GAMESTATE_BYTE_SIZE; i++)               //  to "this.negamaxInputBuffer".
              this.negamaxInputBuffer[i] = this.branches[ this.branchIterator ].gamestate[i];

            //  Next, attempt to look up this game state in the server-side opening book.
            /*
            if()
              {
                console.log('Retrieved lookup for '+this.branchIterator);
              }
            else                                                    //  If lookup failed, proceed with tree-search.
              {
                this.negamaxEngine.instance.exports.initSearch( this.ply );
                console.log('Loaded '+this.branchIterator+' for search @ '+this.ply);
              }
            */
          }

        return;
      }

    /* Advance the search routine. */
    step()
      {
        var i;

        if(this.branches[ this.branchIterator ].depth < this.ply)   //  Is search necessary?
          {
            if(this.negamaxEngine.instance.exports.negamax())       //  Returns true when search is complete.
              {
                console.log('Search is complete.');

                //  Result of search is stored in this.negamaxOutputBuffer.
                //for(i = 0; i < ; i++)
                //  this.branches[ this.branchIterator ]

                if(++this.branchIterator == this.branches.length)   //  Wrap around.
                  this.branchIterator = 0;
              }
          }


        return;
      }

    /* Attempt to look up an advantageous move to make for the game state currently held as a byte array in "this.negamaxInputBuffer". */
    lookup()
      {
        if(!this.callOut)
          {
            var params = 'sendRequest=philadelphia';
            var i;
            for(i = 0; i < _GAMESTATE_BYTE_SIZE; i++)
              params += '&b'+i+'='+this.negamaxInputBuffer[i];

            this.bookLookup.open("POST", 'obj/sess/lookup.php', true);
            this.bookLookup.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
            this.bookLookup.onreadystatechange = function()
              {
                if(this.bookLookup.readyState == 4 && this.bookLookup.status == 200)
                  {
                    if(this.bookLookup.responseText == "")          //  Null return: unknown error
                      {
/*
                        switch(currentLang)
                          {
                            case "Spanish": alert(alertStringScrub("Error"));  break;
                            case "German": alert(alertStringScrub("Fehler"));  break;
                            case "Polish": alert(alertStringScrub("B&#322;&#261;d na stronie"));  break;
                            default: alert(alertStringScrub("Error"));
                          }
*/
                      }
                    else
                      {
                        var parse = this.bookLookup.responseText.split('|');
                        var arr;
                        var i;
                        if(parse[0] == 'chess' && parse[1] == 'ok')
                          {
/*
                            parse = parse[2].split(',');            //  Repurpose "parse".
                            for(i = 0; i < parse.length; i++)       //  Load buffer.
                              {
                                arr = new Uint8Array(1);            //  Force byte type.
                                arr[0] = parseInt(parse[i]);
                                gameStateBuffer[i] = arr[0];
                              }
*/
                          }
                        else                                        //  Error-label or garbage
                          {
/*
                            switch(currentLang)
                              {
                                case "Spanish": alert(alertStringScrub("Error"));  break;
                                case "German": alert(alertStringScrub("Fehler"));  break;
                                case "Polish": alert(alertStringScrub("B&#322;&#261;d na stronie"));  break;
                                default: alert(alertStringScrub("Error"));
                              }
*/
                          }
                      }

                    this.callOut = false;
                  }
              };
            this.callOut = true;
            this.bookLookup.send(params);
          }

        return;
      }
/*
    someOtherFunctionOfThisClass()
      {
      }
*/
  }