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

## Generate a Zobrist hash
This executable lives on the back-end. Call it when the page loads to generate a Zobrist hash for every game.
```
gcc -Wall zgenerate.c -lm -o zgenerate
```

## Client-facing game logic module
Compile the front-end, client-facing game-logic module. This WebAssembly module answers queries from the client-side like getting data about which pieces can move where.
```
sudo docker run --rm -v $(pwd):/src -u $(id -u):$(id -g) --mount type=bind,source=$(pwd),target=/home/src emscripten-c emcc -Os -s STANDALONE_WASM -s EXPORTED_FUNCTIONS="['_getCurrentState','_getMovesBuffer','_sideToMove_client','_isWhite_client','_isBlack_client','_isEmpty_client','_isPawn_client','_isKnight_client','_isBishop_client','_isRook_client','_isQueen_client','_isKing_client','_getMovesIndex_client','_makeMove_client','_isTerminal_client','_isWin_client','_draw']" -Wl,--no-entry "gamelogic.c" -o "gamelogic.wasm"
```

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
