# chess_cpp

-------------- EXTERNAL LIBRARIES TO INSTALL --------------
sudo apt update
sudo apt install ninja-build
sudo apt install libsfml-dev

-------------- COMMANDS TO START THE PROGRAM --------------
cd chess_cpp
cmake --build build --config Release
./build/Release/chess

#COMANDI PER CONFIGURARE E BUILDARE:

cmake -S . -B build -G"Ninja Multi-Config"

cmake --build build --config Debug

cmake --build build --config Debug --target test

cmake --build build --config Release

cmake --build build --config Release --target test
