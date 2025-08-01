#include "Game.hpp"

#include <iostream>

Game::Game(std::string nameWhite, std::string nameBlack,
           sf::RenderWindow& window)
    : window_(window),
      board_(Board(window)),
      whitePlayer_(nameWhite, White),
      blackPlayer_(nameBlack, Black),
      playerTurn_{White},
      gameOver_(false) {}

// metodi per accedere alle variabili private
Board& Game::getBoard() { return board_; }
bool Game::getGameOver() { return gameOver_; }
Color Game::getPlayerTurn() { return playerTurn_; }

// metodi per modificare variabili private
void Game::setPlayerTurn(Color newTurn) { playerTurn_ = newTurn; };
void Game::setGameOver(bool the_end) { gameOver_ = the_end; }

// funzioni per il movimento pezzi
bool Game::rightStarting(Point from) {
  Piece* piece_from = board_.selectPiece(from);
  if (piece_from == nullptr) {
    return false;
  }
  if (piece_from->getColor() != playerTurn_) {
    return false;
  }
  return true;
}

bool Game::rightArrival(Point to) {
  Piece* piece_to = board_.selectPiece(to);
  if (piece_to == nullptr) {
    return true;
  }
  if (piece_to->getColor() != playerTurn_) {
    return true;
  }
  return false;
}

bool Game::validMove(Point from, Point to) {  // non specifico il tipo di pezzo
  Piece* piece = board_.selectPiece(from);
  if (!rightArrival(to) && !rightStarting(from)) {
    std::cout << "2" << '\n';
    return false;
  } else if (!piece->validPieceMove(to)) {
    std::cout << "3" << '\n';
    return false;
  } else if (!board_.clearPath(from, to)) {
    std::cout << "4" << '\n';
    return false;
  } else if (isCheck(playerTurn_) == true) {
    std::cout << "in questo modo ti poni in scacco" << '\n';
    return false;
  }
  if (piece->getName() == pawn) {
    if (from.c == to.c && board_.selectPiece(to) != nullptr) {
      std::cout << "5" << '\n';
      return false;
    }
    if (from.c != to.c && board_.selectPiece(to) == nullptr) {
      return false;
    }
  }
  return true;
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

void Game::setGameOver(bool p) { gameOver_ = p; }

// funzioni per arrocco
bool Game::validCastling(Point from, Point to) {
  Piece* piece = board_.selectPiece(from);
  if (piece->getName() == king) {
    if (from.r == 0) {
      if () }
    if (from.r == 7) {
    }
  }

  return false;
}
// funzioni per enPassant

// funzioni per il movimento dei pezzi
void Game::playMove(Point from, Point to) {}