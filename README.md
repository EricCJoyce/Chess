# [Chess](https://www.ericjoycefilm.com/wastesoftime/boardgames/chess/index.php?lang=en)
Notes on the creation of Chess

## Docker container to compile C to WebAssembly
Create the container.
```
sudo docker build -t emscripten-c .
```

Confirm its existence.
```
sudo docker images
```

Kill the container.
```
sudo docker image rm emscripten-c
```

## Zobrist hash generator
This executable lives on the back-end. Call it when the page loads to generate a Zobrist hash for every game.
```
gcc -Wall zgenerate.c -lm -o zgenerate
```

## Client-facing game logic module
![Game Logic Schema](Game_Logic_Schema.png)

The **game-logic module** has *two* outward-facing buffers:
- `currentState` is `_GAMESTATE_BYTE_SIZE` bytes long. It encodes the current state of the game.
- `movesBuffer` is `4 + _MAX_NUM_TARGETS` bytes long. Its first four bytes encode `n`, the number (unsigned int) of legal targets, then the subsequent `n` bytes are those indices on the chess board.

Compile the front-end, client-facing game-logic module. This WebAssembly module answers queries from the client-side like getting data about which pieces can move where.
```
sudo docker run --rm -v $(pwd):/src -u $(id -u):$(id -g) --mount type=bind,source=$(pwd),target=/home/src emscripten-c emcc -Os -s STANDALONE_WASM -s EXPORTED_FUNCTIONS="['_getCurrentState','_getMovesBuffer','_sideToMove_client','_isWhite_client','_isBlack_client','_isEmpty_client','_isPawn_client','_isKnight_client','_isBishop_client','_isRook_client','_isQueen_client','_isKing_client','_getMovesIndex_client','_makeMove_client','_isTerminal_client','_isWin_client','_draw']" -Wl,--no-entry "gamelogic.c" -o "gamelogic.wasm"
```

This produces a `.wasm` with functions you can call like this:
- `gameEngine.instance.exports.getCurrentState();` returns the address of the game-logic module's current-gamestate buffer.
- `gameEngine.instance.exports.getMovesBuffer();` returns the address of the game-logic module's move-targets buffer.
- `gameEngine.instance.exports.sideToMove_client();` returns `_WHITE_TO_MOVE` or `_BLACK_TO_MOVE`.
- `gameEngine.instance.exports.isWhite_client(unsigned char);` returns a Boolean value indicating whether the piece at the given index belongs to the white team.
- `gameEngine.instance.exports.isBlack_client(unsigned char);` returns a Boolean value indicating whether the piece at the given index belongs to the black team.
- `gameEngine.instance.exports.isEmpty_client(unsigned char);` returns a Boolean value indicating whether the given index is empty.
- `gameEngine.instance.exports.isPawn_client(unsigned char);` returns a Boolean value indicating whether the piece at the given index is a pawn.
- `gameEngine.instance.exports.isKnight_client(unsigned char);` returns a Boolean value indicating whether the piece at the given index is a knight.
- `gameEngine.instance.exports.isBishop_client(unsigned char);` returns a Boolean value indicating whether the piece at the given index is a bishop.
- `gameEngine.instance.exports.isRook_client(unsigned char);` returns a Boolean value indicating whether the piece at the given index is a rook.
- `gameEngine.instance.exports.isQueen_client(unsigned char);` returns a Boolean value indicating whether the piece at the given index is a queen.
- `gameEngine.instance.exports.isKing_client(unsigned char);` returns a Boolean value indicating whether the piece at the given index is a king.
- `gameEngine.instance.exports.getMovesIndex_client(unsigned char);` writes the number of legal targets `n` and then a run of `n` indices for the current state of the board and the piece at the given index.
- `gameEngine.instance.exports.makeMove_client(unsigned char, unsigned char, unsigned char);` .
- `gameEngine.instance.exports.isTerminal_client();` returns a Boolean value indicating whether the current game state is terminal.
- `gameEngine.instance.exports.isWin_client();` returns an unsigned char indicating whether white has won, black has won, the game has reached stalemate, or the game is ongoing.
- `gameEngine.instance.exports.draw();` prints the board to the browser console.

## Negamax & evaluation engines
I have separated game logic and node evaluation from tree-search. This allows me to have a single, game-agnostic negamax engine (in C++) for two-player, non-stochastic, perfect-information games (written in whatever language). The JavaScript class `player.js` glues together and coordinates these components.

![Negamax Schema](Negamax_Engine_Schema.png)

For Chess:
| Name  | Bytes  | Description |
| :---:	| :----: | :---------: |
| _GAMESTATE_BYTE_SIZE | 81 | Number of bytes needed to encode a game state |
| _MOVE_BYTE_SIZE | 3 | Number of bytes needed to describe a move in Chess |
| _MAX_NUM_TARGETS | 32 | A (generous) upper bound on how many distinct destinations (not distinct moves) may be available to a player from a single index |
| _MAX_MOVES | 64 | A (generous) upper bound on how many moves may be made by a team in a single turn |
| _KILLER_MOVE_PER_PLY | 2 | Chess engines typically store 2 killer moves per ply |
| _KILLER_MOVE_MAX_DEPTH | 64 | Not to say that we actually search to depth 64! This is just comfortably large. |
| _TRANSPO_RECORD_BYTE_SIZE | 91 | Number of bytes needed to store a TranspoRecord object |
| _TRANSPO_TABLE_SIZE | 65536 | Number of TranspoRecords, each 91 bytes |
| _TREE_SEARCH_ARRAY_SIZE | 65536 | Number of (game-state bytes, move-bytes) |
| _NEGAMAX_NODE_BYTE_SIZE | 130 | Number of bytes needed to encode a negamax node |
| ZHASH_TABLE_SIZE | 751 | Number of Zobrist keys |
| _WHITE_TO_MOVE | 0 | Indication that white is to move in the current game state |
| _BLACK_TO_MOVE | 1 | Indication that black is to move in the current game state |

The **evaluation engine** has *four* outward-facing buffers:
- `inputGameStateBuffer` is `_GAMESTATE_BYTE_SIZE` bytes long. The negamax module writes bytes here and can then ask the evaluation module things like, "What moves are available from this state?"
- `inputMoveBuffer` is `_MOVE_BYTE_SIZE` bytes long. The negamax module writes bytes here and writes game-state bytes to `inputGameStateBuffer` and can then ask, "What game state results from making this move from this state?"
- `outputGameStateBuffer` is `_GAMESTATE_BYTE_SIZE` bytes long and encodes a chess state after an operation. The negamax module reads these bytes from the evaluation module.
- `outputMovesBuffer` is 4 + `_MAX_MOVES` * (`_MOVE_BYTE_SIZE` + 4) bytes long. The first 4 bytes decode to an unsigned integer indicating how many moves are in the array. Subsequent bytes encode the move (as from, to, promo) and the rough score computed using SEE, promotions, and checks. The negamax module reads these, updates their scores for better move-ordering, and turns them into search nodes.

Compile the evaluation engine:
```
sudo docker run --rm -v $(pwd):/src -u $(id -u):$(id -g) --mount type=bind,source=$(pwd),target=/home/src emscripten-c emcc -Os -s STANDALONE_WASM -s EXPORTED_FUNCTIONS="['_getInputGameStateBuffer','_getInputMoveBuffer','_getOutputGameStateBuffer','_getOutputMovesBuffer','_isQuiet','_isTerminal','_isSideToMoveInCheck','_nonPawnMaterial','_makeMove','_makeNullMove','_evaluate','_getMoves']" -Wl,--no-entry "philadelphia.c" -o "eval.wasm"
```
This produces a `.wasm` with functions you can load into the JavaScript Player class and call like this:
- `this.evaluationEngine.instance.exports.getInputGameStateBuffer();` returns the address of the evaluation module's input-gamestate buffer.
- `this.evaluationEngine.instance.exports.getInputMoveBuffer();` returns the address of the evaluation module's input-move buffer.
- `this.evaluationEngine.instance.exports.getOutputGameStateBuffer();` returns the address of the evaluation module's output-gamestate buffer.
- `this.evaluationEngine.instance.exports.getOutputMovesBuffer();` returns the address of the evaluation module's output-moves buffer.
- `this.evaluationEngine.instance.exports.isQuiet();` returns a Boolean value, according to the game state bytes previously written to the evaluation module's input-gamestate buffer.
- `this.evaluationEngine.instance.exports.isTerminal();` returns a Boolean value, according to the game state bytes previously written to the evaluation module's input-gamestate buffer.
- `this.evaluationEngine.instance.exports.isSideToMoveInCheck();` returns a Boolean value, according to the game state bytes previously written to the evaluation module's input-gamestate buffer.
- `this.evaluationEngine.instance.exports.nonPawnMaterial();` returns an unsigned integer value, according to the game state bytes previously written to the evaluation module's input-gamestate buffer. This function is used by the negamax module to test whether there is enough material to try null-move pruning.
- `this.evaluationEngine.instance.exports.makeMove();` .
- `this.evaluationEngine.instance.exports.makeNullMove();` .
- `this.evaluationEngine.instance.exports.evaluate(bool);` .
- `this.evaluationEngine.instance.exports.getMoves();` .

The **negamax engine** has *eleven* outward-facing buffers:
- `inputGameStateBuffer`
- `outputBuffer`
- `queryGameStateBuffer`
- `queryMoveBuffer`
- `answerGameStateBuffer`
- `answerMovesBuffer`
- `zobristHashBuffer`
- `transpositionTableBuffer`
- `negamaxSearchBuffer`
- `killerMovesTableBuffer`
- `historyTableBuffer`

Compile the negamax engine:
```
sudo docker run --rm -v $(pwd):/src -u $(id -u):$(id -g) --mount type=bind,source=$(pwd),target=/home/src emscripten-c em++ -I ./ -Os -s STANDALONE_WASM -s INITIAL_MEMORY=16121856 -s STACK_SIZE=1048576 -s EXPORTED_FUNCTIONS="['_getInputBuffer','_getQueryBuffer','_getOutputBuffer','_getZobristHashBuffer','_getTranspositionTableBuffer','_getNegamaxSearchBuffer','_getAuxiliaryBuffer','_getKillerMovesBuffer','_getHistoryBuffer','_initSearch','_negamax']" -Wl,--no-entry "negamax.cpp" -o "negamax.wasm"
```
This produces a `.wasm` with functions you can load into the JavaScript Player class and call like this:
- `this.negamaxEngine.instance.exports.getInputBuffer();` returns the address of the negamax module's input-gamestate buffer.

## Citation
If this code was helpful to you, please cite this repository.

```
@misc{chess,
  title={Chess in C},
  author={Eric C. Joyce},
  year={2025},
  publisher={Github},
  journal={GitHub repository},
  howpublished={\url{https://github.com/EricCJoyce/Chess}}
}
```
