src="."
obj="$src/obj"

mkdir -p $obj

C_FLAGS="\
    -O3 \
    -march=native \
    -ansi \
    -Wall \
    -pedantic"

echo Compiling C source code...
cc -S $C_FLAGS -o $obj/main.s    $src/main.c
cc -S $C_FLAGS -o $obj/catch.s   $src/catch.c

echo Assembling compiled sources...
as -o $obj/main.o    $obj/main.s
as -o $obj/catch.o   $obj/catch.s

echo Linking assembled object files...
gcc -o $obj/sig-test -s $obj/main.o $obj/catch.o
