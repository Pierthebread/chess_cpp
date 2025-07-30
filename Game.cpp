#include "Game.hpp"

#include <iostream>

Game::Game(std::string nameWhite, std::string nameBlack,
           sf::RenderWindow& window)
    : window_(window),
      board_(Board()),
      whitePlayer_(nameWhite, White),
      blackPlayer_(nameBlack, Black),
      playerTurn_{White},
      gameOver_(false) {}

// metodi per accedere alle variabili private
Color Game::getPlayerTurn() { return playerTurn_; }
bool Game::getGameOver() { return gameOver_; }

// metodi per modificare le variabili private
void Game::setPlayerTurn(Color color) { playerTurn_ = color; }

void Game::setGameOver(bool p) { gameOver_ = p; }

// funzioni validMove
bool Game::rightStarting(Point from) {
  Piece* piece{board_.selectPiece(from)};
  if (piece == nullptr) {
    return false;
  }
  if (piece->getColor() != playerTurn_) {
    return false;
  }
  return true;
}

bool Game::rightArrival(Point to) {
  Piece* piece{board_.selectPiece(to)};
  if (piece->getColor() == playerTurn_) {
    return false;
  }
  return true;
}

bool Game::validMove(Point from, Point to) {
  Piece* piece_from{board_.selectPiece(from)};
  Piece* piece_to{board_.selectPiece(to)};

  if (rightStarting(from) != true or rightArrival(to) != true) {
    return false;
  }
  if (rightArrival)
  }
}

// funzioni per lo scacco
bool Game::isCheck(Color color) {
  for (int c = 0; c < 8; ++c) {
    for (int r = 0; r < 8; ++r) {
      Piece* piece{board_.selectPiece({static_cast<Column>(c), r})};
      if (piece != nullptr &&
          isChecking({static_cast<Column>(c), r}, color) == true) {
        return true;
      }
    }
  }
  return false;
}

bool Game::isChecking(Point p, Color color) {}