emcc -o game.html  -Os -Wall ../raylib/src/libraylib.a -I../raylib/src/ *.c -s USE_GLFW=3 -s ASYNCIFY  -DPLATFORM_WEB --preload-file assets
