#include "Game.hpp"

#include <iostream>
#include <limits>

Game::Game(std::string nameWhite, std::string nameBlack,
           sf::RenderWindow& window)
    : window_(window),
      board_(Board(window)),
      whitePlayer_(nameWhite, White),
      blackPlayer_(nameBlack, Black),
      playerTurn_{White},
      gameOver_(false),
      fifty_movescounter_(0) {}

// metodi per accedere alle variabili private
Board& Game::getBoard() { return board_; }
bool Game::getGameOver() { return gameOver_; }
Color Game::getPlayerTurn() { return playerTurn_; }
Player Game::getPlayer(Color color) {
  return (color == White) ? whitePlayer_ : blackPlayer_;
}
int Game::getFiftyMovesCounter() { return fifty_movescounter_; }

// metodi per modificare variabili private
void Game::switchTurn() {
  playerTurn_ = (playerTurn_ == White) ? Black : White;
}
void Game::setGameOver(bool the_end) { gameOver_ = the_end; }
void Game::addMovesCounter() { ++fifty_movescounter_; }
void Game::resetMovesCounter() { fifty_movescounter_ = 0; }

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
  if (!piece_to) {
    return true;
  }
  if (piece_to->getColor() != playerTurn_) {
    return true;
  }
  return false;
}

bool Game::validMove(Point from, Point to, Board& board) {
  Piece* piece = board.selectPiece(from);

  if (!rightArrival(to)) {
    return false;
  } else if (!rightStarting(from)) {
    return false;
  } else if (!piece->validPieceMove(from, to)) {
    return false;
  } else if (!board.clearPath(from, to)) {
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
      return false;
    }
    if (from.c != to.c && !board.selectPiece(to)) {
      return false;
    }
  }
  return true;
}

//////// funzioni per lo scacco
// è scacco per il colore selezionato?
bool Game::isCheck(Color color, Board& board) {
  Point king_pos = board.kingPosition(color);
  return isCellAttached(king_pos, color, board);
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

bool Game::isCellAttached(Point p, Color color, Board& board) {
  for (int c = 0; c < 8; ++c) {
    for (int r = 0; r < 8; ++r) {
      Piece* piece = board.selectPiece({c, r});
      if (piece && piece->getColor() != color) {
        if (piece->getName() != pawn && piece->validPieceMove({c, r}, p) &&
            board.clearPath({c, r}, p)) {
          return true;
        }
        if (piece->getName() == pawn && piece->validPieceMove({c, r}, p) &&
            c != p.c) {
          return true;
        }
      }
    }
  }
  return false;
}

//// funzioni per arrocco
bool Game::isCastlingValid(Point from, Point to) {
  int rook_c = (to.c == 2) ? 0 : 7;
  Piece* rook_pos = board_.selectPiece({rook_c, to.r});

  if (!rook_pos || rook_pos->getName() != rook || rook_pos->getMoved()) {
    return false;
  }
  if (!board_.clearPath(from, {rook_c, to.r})) {
    return false;
  }
  int step = (to.c < from.c) ? -1 : 1;
  for (int x = from.c; x != to.c + step; x += step) {
    Point checkCell{x, from.r};
    if (isCellAttached(checkCell, rook_pos->getColor(), board_)) {
      return false;
    }
  }
  return true;
}

void Game::executeCastling(Point from, Point to) {
  const int row = to.r;
  // Arrocco lungo (sinistra)
  if (to.c == 2) {
    board_.movePiece(from, {2, row});      // Muove il re
    board_.movePiece({0, row}, {3, row});  // Muove la torre sinistra
    board_.selectPiece({3, row})->setMoved(true);
  }
  // Arrocco corto (destra)
  else if (to.c == 6) {
    board_.movePiece(from, {6, row});      // Muove il re
    board_.movePiece({7, row}, {5, row});  // Muove la torre destra
    board_.selectPiece({5, row})->setMoved(true);
  }
  board_.selectPiece(to)->setMoved(true);
}

//////// funzioni per enPassant
void Game::setEnPassantTarget(Point from, Point to) {
  Piece* piece = board_.selectPiece(to);
  if (piece && piece->getName() == pawn && abs(to.r - from.r) == 2) {
    enPassantTarget_ = {to.c, (to.r + from.r) / 2};
  } else {
    enPassantTarget_ = {8, 8};
  }
}

bool Game::isEnPassantValid(Point from, Point to) {
  return (to == enPassantTarget_ && !board_.selectPiece(to) &&
          board_.selectPiece(from)->validPieceMove(from, to));
}

void Game::executeEnPassant(Point from, Point to) {
  board_.clearPieceAt({to.c, from.r});
  board_.movePiece(from, to);
}

// funzioni per la promotion
Name Game::pieceToPromote() {
  int i;
  while (i < 1 || i > 4) {
    std::cout << " Select the piece " << '\n'
              << "1. Queen " << '\n'
              << "2. Bishop " << '\n'
              << "3. Knight " << '\n'
              << "4. Rook" << '\n';
    ;
    std::cin >> i;
    if (std::cin.fail()) {  // bool che verifica tipo input == tipo richiesto
                            // (es. qualcuno scrive una lettera piuttosto che
                            // un numero)
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "Input non valido !!! " << '\n' << "Riprova " << '\n';
    }
  }
  Name promotedPiece;
  switch (i) {
    case 1:
      promotedPiece = queen;
      break;
    case 2:
      promotedPiece = bishop;
      break;
    case 3:
      promotedPiece = knight;
      break;
    case 4:
      promotedPiece = rook;
      break;

    default:
      break;
  }
  return promotedPiece;
}

void Game::executePromotion(Point from, Point to) {
  Name promotedPiece{pieceToPromote()};
  Color color{board_.selectPiece(from)->getColor()};
  board_.promote(to, promotedPiece, color);
  board_.clearPieceAt(from);
}

// funzioni per il movimento dei pezzi
void Game::executeMove(Point from, Point to) {
  Piece* piece{board_.selectPiece(from)};
  Name name_piece{piece->getName()};
  Piece* captured{board_.selectPiece(to)};
  bool ate;

  if (captured) {
    ate = true;
  }

  bool moveExecuted{false};
  // ARROCCO
  if (board_.isCastling(from, to) && isCastlingValid(from, to)) {
    executeCastling(from, to);
    moveExecuted = true;
  }
  // EN PASSANT
  else if (to == enPassantTarget_ && isEnPassantValid(from, to)) {
    executeEnPassant(from, to);
    moveExecuted = true;
  }
  // MOSSA NORMALE e PROMOZIONE
  else if (validMove(from, to, board_)) {
    if (board_.isPromotion(from, to)) {
      executePromotion(from, to);
      moveExecuted = true;
    } else {
      board_.movePiece(from, to);
      moveExecuted = true;
      piece->setMoved(true);
    }
  }
  if (moveExecuted) {
    setEnPassantTarget(from, to);
    if (!ate && piece->getName() != pawn) {
      addMovesCounter();
    } else {
      resetMovesCounter();
    }
    switchTurn();
  }
}

bool Game::canMove(Color color) {
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      Point from{i, j};  // r = row, c = col
      Piece* piece = board_.selectPiece(from);
      if (piece && piece->getColor() == color) {
        for (int r = 0; r < 8; ++r) {
          for (int c = 0; c < 8; ++c) {
            Point to{c, r};
            try {
              if (validMove(from, to, board_)) {
                return true;
              }
            } catch (std::runtime_error) {
              continue;
            }
          }
        }
      }
    }
  }
  return false;
}

bool Game::isCheckmate(Color color) {
  return (isCheck(color, board_) && !canMove(color));
}

// Materiale insufficiente: re contro re, re contro re e pezzo leggero, re e p.l
// contro re e p.l, re contro due cavalli
//bool Game::insufficientMaterial() {
//  for (int i = 0; i < 8; ++i) {
//    for (int j = 0; j < 8; ++j) {
//      Piece* piece = board_.selectPiece({i, j});
//      if (piece != nullptr or piece->getName() != king) {
//        return false;
//      }
//      return true;
//    }
//  }
//}

void Game::checkGameOver() {
  if (!canMove(playerTurn_)) {
    setGameOver(true);
    std::cout << "help" << '\n';
    if (isCheck(playerTurn_, board_)) {
      // Checkmate
      Player winner = getPlayer(
          (playerTurn_ == White) ? Black : White);  // Opposite player wins
      std::cout << "Checkmate! " << winner.getName() << " wins!" << '\n';
    } else {
      // Stalemate
      std::cout << "Draw by stalemate" << '\n';
    }
  }
  //if (insufficientMaterial()) {
  //  // Draw by insufficient material
  //  std::cout << "Draw by insufficient material" << '\n';
  //}
  if (isFiftyMoves()) {
    std::cout << "Draw by fiftyMoves rule" << '\n';
  }
}

bool Game::isFiftyMoves() { return getFiftyMovesCounter() == 50; }