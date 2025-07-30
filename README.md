# chess_cpp

#scaricare la repository:

$ git clone git@github.com:Pierthebread/chess_cpp.git

$ cd chess_cpp 

#CREARE BRANCH

$ git branch <nome>

$ git checkout <nome>

#CARICARE I SALVATAGGI (la mia filosofia):

$ git add "nome file" (oppure git add .)

$ git commit -m "commento"

$ git push 

$ git checkout main

$ git merge "nome branch su cui hai lavorato"

$ git push

#COMANDI PER CONFIGURARE E BUILDARE:

cmake -S . -B build -G"Ninja Multi-Config"

cmake --build build --config Debug

cmake --build build --config Debug --target test

cmake --build build --config Release

cmake --build build --config Release --target test
