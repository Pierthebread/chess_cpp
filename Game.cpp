#include "Game.hpp"

#include <iostream>

Game::Game(std::string nameWhite, std::string nameBlack,
           sf::RenderWindow& window)
    : window_(window),
      board_(window_),
      whitePlayer(nameWhite, White),
      blackPlayer(nameBlack, Black),
      playerTurn{White},
      gameOver_(false) {}
