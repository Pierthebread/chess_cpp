# chess_cpp
#COMANDI PER CONFIGURARE E BUILDARE:

cmake -S . -B build -G"Ninja Multi-Config"

cmake --build build --config Debug

cmake --build build --config Debug --target test

cmake --build build --config Release

cmake --build build --config Release --target test
