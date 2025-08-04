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

bool Game::validMove(Point from, Point to, Board& board) {
  Piece* piece = board.selectPiece(from);
  if (!rightArrival(to) && !rightStarting(from)) {
    return false;
  } else if (!piece->validPieceMove(from, to)) {
    return false;
  } else if (!board.clearPath(from, to)) {
    std::cout << "4" << '\n';
    return false;
  } else if (isCheck(playerTurn_, board) == true) {
    std::cout << "in questo modo ti poni in scacco" << '\n';
    return false;
  }
  if (piece->getName() == pawn) {
    if (from.c == to.c && board.selectPiece(to)) {
      std::cout << "5" << '\n';
      return false;
    }
    if (from.c != to.c && board.selectPiece(to)) {
      return false;
    }
  }
  return true;
}

//// funzioni per lo scacco
// è scacco per il colore selezionato?
bool Game::isCheck(Color color, Board& board) {
  Point king_pos = board.kingPosition(color);
  for (int c = 0; c < 8; ++c) {
    for (int r = 0; r < 8; ++r) {
      Piece* piece = board.selectPiece({c, r});
      if (piece && piece->getColor() != color) {
        if (!rightArrival({c,r}) && !rightStarting(king_pos) && !piece->validPieceMove({c,r}, king_pos), !board.clearPath({c,r}, king_pos)) {
          return true;
        }
      }
    }
  }
  return false;
}

// la mossa genera uno scacco a se stessi?
bool Game::createCheck(Point from, Point to) {
  Piece* piece = board_.selectPiece(from);  // pezzo che voglio spostare
  if (!piece) return false;
  Board temporary_board = board_.cloneBoard(board_);
  temporary_board.movePiece(from, to);
  bool createCheck = isCheck(piece->getColor(), temporary_board);
  return createCheck;
}

//// funzioni per arrocco
bool Game::isCastlingValid(Point from, Point to) {
  Piece* king_piece = board_.selectPiece(from);
  Piece* rook_piece = board_.selectPiece(to);
  if (king_piece->getName() == king && !king_piece->getMoved() &&
      rook_piece->getName() == rook && !rook_piece->getMoved() &&
      king_piece->getColor() == rook_piece->getColor()) {
    if ((from.r == 0 or from.r == 7) && (to.c == 7 or to.c == 0) &&
        board_.clearPath(from, to)) {
      return true;
    }
  }
  return false;
}

