clang -Wall -Wextra --target=wasm32 -o  wasm.o -c ./wasm.c
wasm-ld --no-entry -export-all --allow-undefined -o ../webpack/wasm.wasm wasm.o
