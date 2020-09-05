mkdir ./build
pwd
cd ./data_desk
pwd
./build_gcc.sh
cd -

cd ./data_desk_custom
pwd
gcc -c -fpic data_desk_custom.c
gcc -shared -o data_desk_custom.so data_desk_custom.o
cd -

cd ./src
pwd
../data_desk/build/data_desk --custom ../data_desk_custom/data_desk_custom.so ./ComponentEnums.ds
cd -

cd ./build
pwd
g++ -o TheGreatMachine ../src/TheGreatMachine.cpp ../src/pge/*.cpp ../src/imgui/*.cpp -std=c++17 -lpng -lGL -lX11 -pthread -Wno-write-strings
cd -

