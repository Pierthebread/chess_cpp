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
      fifty_movescounter_(0) {
  if (nameWhite.empty() || nameBlack.empty()) {
    throw std::invalid_argument(
        "I nomi dei giocatori non possono essere vuoti");
  }
}

// metodi per accedere alle variabili private
Board& Game::getBoard() {
  assert(!gameOver_);
  return board_;
}
bool Game::getGameOver() { return gameOver_; }
Color Game::getPlayerTurn() {
  assert(!gameOver_);
  return playerTurn_;
}
const Player& Game::getPlayer(Color color) {
  assert(color == White || color == Black);
  return (color == White) ? whitePlayer_ : blackPlayer_;
}
int Game::getFiftyMovesCounter() {
  assert(!gameOver_);
  assert(fifty_movescounter_ >= 0 && fifty_movescounter_ <= 50);
  return fifty_movescounter_;
}

// metodi per modificare variabili private
void Game::switchTurn() {
  assert(!gameOver_);
  playerTurn_ = (playerTurn_ == White) ? Black : White;
}
void Game::setGameOver(bool the_end) { gameOver_ = the_end; }
void Game::addMovesCounter() {
  assert(fifty_movescounter_ >= 0 && fifty_movescounter_ <= 50);
  ++fifty_movescounter_;
}
void Game::resetMovesCounter() {
  assert(fifty_movescounter_ >= 0 && fifty_movescounter_ <= 50);
  fifty_movescounter_ = 0;
}

// funzioni per il movimento pezzi
bool Game::rightStarting(Point from) {
  Piece* piece_from = board_.selectPiece(from);
  assertInRange_Game(from);
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
  assertInRange_Game(to);
  if (!piece_to) {
    return true;
  }
  if (piece_to->getColor() != playerTurn_) {
    return true;
  }
  return false;
}

bool Game::validMove(Point from, Point to, const Board& board) {
  assertInRange_Game(from);
  assertInRange_Game(to);
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
    return false;
  }
  if (piece->getName() == pawn) {
    if (to.c == enPassantTarget_.c && to.r == enPassantTarget_.r) {
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
bool Game::isCheck(Color color, const Board& board) {
  Point king_pos = board.getKingPosition(color);
  return isCellAttached(king_pos, color, board);
}

// la mossa genera uno scacco a se stessi?
bool Game::createCheck(Point from, Point to) {
  Piece* piece = board_.selectPiece(from);  // pezzo che voglio spostare
  if (!piece) {
    return false;
  }
  Board temporary_board = board_.cloneBoard(board_);
  temporary_board.movePiece(from, to);
  bool createCheck = isCheck(piece->getColor(), temporary_board);
  return createCheck;
}

bool Game::isCellAttached(Point p, Color color, const Board& board) {
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
  assert(board_.isCastling(from, to));
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
  assert(board_.selectPiece(to)->getName() == king);
  board_.selectPiece(to)->setMoved(true);
  board_.setKingPosition(board_.selectPiece(to)->getColor(), to);
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
  return (to.c == enPassantTarget_.c && to.r == enPassantTarget_.r &&
          !board_.selectPiece(to) &&
          board_.selectPiece(from)->validPieceMove(from, to));
}

void Game::executeEnPassant(Point from, Point to) {
  board_.deletePiece({to.c, from.r});
  board_.movePiece(from, to);
}

// funzioni per la promotion

void Game::executePromotion(Point from, Point to) {
  Name promotedPiece{pieceToPromote()};
  Color color{board_.selectPiece(from)->getColor()};
  board_.promote(to, promotedPiece, color);
  board_.deletePiece(from);
}

Name Game::pieceToPromote() {
  int i{0};
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

// funzioni per il movimento dei pezzi
void Game::executeMove(Point from, Point to) {
  Piece* piece{board_.selectPiece(from)};
  Name Name_piece{piece->getName()};
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
  else if (to.c == enPassantTarget_.c && to.r == enPassantTarget_.r &&
           isEnPassantValid(from, to)) {
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
    if (ate || piece->getName() == pawn) {
      resetMovesCounter();
    } else {
      addMovesCounter();
    }
    if (Name_piece == king) {
      board_.setKingPosition(piece->getColor(), to);
    }
    std::cout << "posizione del re" << board_.getKingPosition(White).c << "  "
              << board_.getKingPosition(White).r << '\n';
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
            } catch (std::runtime_error& e) {
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

bool Game::insufficientMaterial() {
  int whitePieces = 0, blackPieces = 0;
  int whiteKnights = 0, blackKnights = 0;
  bool whiteHasBishop = false, blackHasBishop = false;
  bool whiteHasDarkSquaredBishop = false, whiteHasLightSquaredBishop = false;
  bool blackHasDarkSquaredBishop = false, blackHasLightSquaredBishop = false;

  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      Piece* piece = board_.selectPiece({i, j});
      if (!piece) continue;  // Casella vuota

      Color color = piece->getColor();
      Name name = piece->getName();

      // Se c'è una donna, torre o pedone, il materiale NON è insufficiente
      if (name == queen || name == rook || name == pawn) {
        return false;
      }

      if (color == White) {
        whitePieces++;
        if (name == knight)
          whiteKnights++;
        else if (name == bishop) {
          whiteHasBishop = true;
          // Controlla se l'alfiere è su casella chiara o scura
          if ((i + j) % 2 == 0)
            whiteHasDarkSquaredBishop = true;
          else
            whiteHasLightSquaredBishop = true;
        }
      } else {  // Black
        blackPieces++;
        if (name == knight)
          blackKnights++;
        else if (name == bishop) {
          blackHasBishop = true;
          if ((i + j) % 2 == 0)
            blackHasDarkSquaredBishop = true;
          else
            blackHasLightSquaredBishop = true;
        }
      }
    }
  }

  // Caso 1: Solo i due re → patta
  if (whitePieces == 1 && blackPieces == 1) {
    return true;
  }

  // Caso 2: Re + cavallo/alfiere vs Re → patta
  if ((whitePieces == 1 && blackPieces == 2 &&
       (blackKnights == 1 || blackHasBishop)) ||
      (blackPieces == 1 && whitePieces == 2 &&
       (whiteKnights == 1 || whiteHasBishop))) {
    return true;
  }

  // Caso 3: Re + alfiere vs Re + alfiere (stesso colore degli alfieri) → patta
  if (whitePieces == 2 && blackPieces == 2 && whiteHasBishop &&
      blackHasBishop) {
    if ((whiteHasLightSquaredBishop && blackHasLightSquaredBishop) ||
        (whiteHasDarkSquaredBishop && blackHasDarkSquaredBishop)) {
      return true;
    }
  }

  return false;
}

void Game::checkGameOver() {
  if (!canMove(playerTurn_)) {
    setGameOver(true);
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
  if (insufficientMaterial()) {
    // Draw by insufficient material
    std::cout << "Draw by insufficient material" << '\n';
  }
  if (isFiftyMoves()) {
    std::cout << "Draw by fiftyMoves rule" << '\n';
  }
}

bool Game::isFiftyMoves() { return getFiftyMovesCounter() == 50; }
