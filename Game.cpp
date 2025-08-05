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
  if (!piece_from) {
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
  } else if (createCheck(from, to) == true) {
    std::cout << "in questo modo ti poni in scacco" << '\n';
    return false;
  }
  if (piece->getName() == pawn) {
    if (to == enPassantTarget_) {
      return true;
    }
    if (from.c == to.c && board.selectPiece(to)) {
      std::cout << "5" << '\n';
      return false;
    }
    if (from.c != to.c && !board.selectPiece(to)) {
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
        if (piece->validPieceMove(
                {c, r}, king_pos) &&  // probabilmente altri casi da considerare
            board.clearPath({c, r}, king_pos)) {
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
  if (!piece) {
    return false;
  }  // potrebbe essere tolto forse
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

void Game::executeCastling(Point from, Point to) {
  if (to.r == 0) {  // caso bianco
    switch (to.c) {
      case 0:
        board_.movePiece(from, {2, 0});
        board_.movePiece(to, {3, 0});
        return;
      case 7:
        board_.movePiece(from, {6, 0});
        board_.movePiece(to, {5, 0});
        return;
    }
  } else {  // caso nero
    switch (to.c) {
      case 0:
        board_.movePiece(from, {2, 7});
        board_.movePiece(to, {3, 7});
        return;
      case 7:
        board_.movePiece(from, {6, 7});
        board_.movePiece(to, {5, 7});
        return;
    }
  }
}

// funzioni per enPassant
void Game::setEnPassantTarget(Point from, Point to) {
  Piece* piece = board_.selectPiece(from);
  if (piece && piece->getName() == pawn && abs(to.r - from.r) == 2) {
    enPassantTarget_ = {to.c, (to.r + from.r) / 2};
  } else {
    enPassantTarget_ = {8, 8};
  }
}

bool Game::isEnPassantValid(Point to) {
  if (to == enPassantTarget_ && !board_.selectPiece(to)) {
    return true;
  }
  return false;
}

void Game::executeEnPassant(Point from, Point to) {  // serve davvero?
  board_.movePiece(to, {to.c, from.r});
  board_.movePiece(from, to);
}

// funzioni per il movimento dei pezzi
void Game::playMove(Point from, Point to) {
  Piece* piece = board_.selectPiece(from);
  bool moveExecuted{false};
  // verifichiamo che la mossia sia valida
  if (!validMove(from, to, board_)) {
    return;
  }
  // casi particolari
  switch (piece->getName()) {
    case king: {
      if (isCastlingValid(from, to) == true) {
        executeCastling(from, to);
        moveExecuted = true;
        // cambio turno
      }
      break;
    }
    case pawn: {
      if (isEnPassantValid(to)) {
        executeEnPassant(from, to);
        moveExecuted = true;
      }
      setEnPassantTarget(from, to);

      if (board_.isPromotion(to, from) == true) {
        // qualcosa per scegliere a quale pezzo sostituire il pedone
        Name promotePiece;
        board_.promote(to, promotePiece);
        moveExecuted = true;
      }
      break;
    }
    default:
      break;
  }
  // caso generale
  if (!moveExecuted) {
    board_.movePiece(from, to);
  }

  piece->setMoved(true);
  playerTurn_ = (playerTurn_ == White) ? Black : White;  // cambio turno
}
