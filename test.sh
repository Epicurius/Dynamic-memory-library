#!/bin/sh

### Using rpath.
make fclean && make -j6
gcc ./tests/main.c -L . -Wl,-rpath=$PWD -Wall -ldm_x86_64_Linux -I include -pthread
./a.out && rm ./a.out


### Using LD_LIBRARY_PATH.
# make fclean && make -j6
# echo "Before: $LD_LIBRARY_PATH"
# export LD_LIBRARY_PATH=$PWD
# echo "During: $LD_LIBRARY_PATH"
# gcc ./tests/main.c -L . -Wall -ldm_x86_64_Linux -I include -pthread
# ./a.out && rm ./a.out
# unset "LD_LIBRARY_PATH"
# echo "After: $LD_LIBRARY_PATH"

### https://www.cprogramming.com/tutorial/shared-libraries-linux-gcc.html
