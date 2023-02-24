#! /bin/sh

# Make sure to have raylib recompiled for the WEB version.
# Also make sure to have emcc toolchain in your path
# And update the path to your raylib location
RAYLIB_LOCATION="../raylib"
emcc -o game.html  -Os -Wall $RAYLIB_LOCATION/src/libraylib.a -I$RAYLIB_LOCATION/src/ *.c -s USE_GLFW=3 -s ASYNCIFY  -DPLATFORM_WEB --preload-file assets
