
# Ludo

To compile, first compile and install [raylib](https://github.com/raysan5/raylib) as a shared library, and afterwards, simply run
`gcc *.c -lraylib`


## Compile for web

  Make sure to recompile raylib for web.
  This means doing `make clean`, and then following their instructions on how to compile the library for the web platform
  After you've setup raylib and emcc run the ./compile_for_web.sh (edit it to point to raylib)
