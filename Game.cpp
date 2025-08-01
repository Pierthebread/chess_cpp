#include "Game.hpp"

#include <iostream>

Game::Game(std::string nameWhite, std::string nameBlack,
           sf::RenderWindow& window)
    : window_(window),
      board_(Board()),
      whitePlayer__(nameWhite, White),
      blackPlayer__(nameBlack, Black),
      playerTurn__{White},
      gameOver_(false) {}

// metodi per accedere alle variabili private
Board& Game::getBoard() { return board_; }
bool Game::getGameOver() { return gameOver_; }
Color Game::getPlayerTurn() { return playerTurn_; }

// metodi per modificare variabili private
void Game::setPlayerTurn_(Color newTurn) { playerTurn_ = newTurn; };
void Game::setGameOver(bool the_end) { gameOver_ = the_end; }

// funzioni per il movimento dei pezzi
void Game::playMove(Point from, Point to){}

bool Game::rightStarting(Point from){
  Piece* piece_from = board_.selectPiece(from);
  if (!piece_from){return false;}
  if (piece_from->getColor() != playerTurn_){return false;}
  return true;
}

bool Game::rightArrival(Point to){
  Piece* piece_to = board_.selectPiece(to);
  if(!piece_to){return true;}
  if(piece_to->getColor() != playerTurn_){return true;}
  return false;
}

bool Game::validMove(Point from, Point to) {
  Piece* piece = board_.selectPiece(from);

  if (piece->getName() == pawn) {
    if (from.c == to.c && board_.selectPiece(to) != nullptr) {
      std::cout << "5" << '\n';
      return false;
    }

    if (from.c != to.c && board_.selectPiece(to) == nullptr) {
      std::cout << "6" << '\n';
      return false;
    }

    if (from.c == to.c && abs(from.r - to.r) == 2 &&
        piece->getMoved() == true) {
      std::cout << "7" << '\n';
      return false;
    }
  }

  if (!rightStarting(from)) {
    std::cout << "1" << '\n';
    return false;  // controlla anche che non sia nullptr !
  } else if (!rightArrival(to)) {
    std::cout << "2" << '\n';
    return false;  // controlla che non mangio i miei pezzi
  } else if (!piece->validPieceMove(to)) {
    std::cout << "3" << '\n';
    return false;
  } else if (!board_.clearPath(from, to)) {
    std::cout << "4" << '\n';
    return false;
  } else if (createCheck(from, to)) {
    std::cout << "in questo modo ti poni in scacco" << '\n';
    return false;
  }
  return true;
}


// metodi per accedere alle variabili private
Board& Game::getBoard() { return board_; }
bool Game::getGameOver() { return gameOver_; }
Color Game::getPlayerTurn() { return playerTurn_; }

// metodi per modificare variabili private
void Game::setPlayerTurn_(Color newTurn) { playerTurn_ = newTurn; };
void Game::setGameOver(bool the_end) { gameOver_ = the_end; }

// funzioni per il movimento dei pezzi
void Game::playMove(Point from, Point to){}

bool Game::rightStarting(Point from){
  Piece* piece_from = board_.selectPiece(from);
  if (!piece_from){return false;}
  if (piece_from->getColor() != playerTurn_){return false;}
  return true;
}

bool Game::rightArrival(Point to){
  Piece* piece_to = board_.selectPiece(to);
  if(!piece_to){return true;}
  if(piece_to->getColor() != playerTurn_){return true;}
  return false;
}

bool Game::validMove(Point from, Point to) {
  Piece* piece = board_.selectPiece(from);

  if (piece->getName() == pawn) {
    if (from.c == to.c && board_.selectPiece(to) != nullptr) {
      std::cout << "5" << '\n';
      return false;
    }

    if (from.c != to.c && board_.selectPiece(to) == nullptr) {
      std::cout << "6" << '\n';
      return false;
    }

    if (from.c == to.c && abs(from.r - to.r) == 2 &&
        piece->getMoved() == true) {
      std::cout << "7" << '\n';
      return false;
    }
  }

  if (!rightStarting(from)) {
    std::cout << "1" << '\n';
    return false;  // controlla anche che non sia nullptr !
  } else if (!rightArrival(to)) {
    std::cout << "2" << '\n';
    return false;  // controlla che non mangio i miei pezzi
  } else if (!piece->validPieceMove(to)) {
    std::cout << "3" << '\n';
    return false;
  } else if (!board_.clearPath(from, to)) {
    std::cout << "4" << '\n';
    return false;
  } else if (createCheck(from, to)) {
    std::cout << "in questo modo ti poni in scacco" << '\n';
    return false;
  }
  return true;
}


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