
#include "Board.hpp"

// Costruttore
Board::Board(sf::RenderWindow& window) : window_(window) {
  try {
    for (auto& column : board) {
      for (auto& cell : column) {
        cell = nullptr;
      }
    }
    const float cellSize = CELL_SIZE;
    cellWhite_.setSize(sf::Vector2f(cellSize, cellSize));
    cellWhite_.setFillColor(sf::Color(245, 245, 220));

    cellBlack_.setSize(sf::Vector2f(cellSize, cellSize));
    cellBlack_.setFillColor(sf::Color(139, 69, 19));
  } catch (const std::exception& e) {
    std::cerr << "Error in Board constructor: " << e.what() << std::endl;
    throw;
  }
}

// clona la board attuale
Board Board::cloneBoard(const Board& other_board) {
  Board temporary_board(other_board.window_);
  for (std::size_t c = 0; c < 8; ++c) {
    for (std::size_t r = 0; r < 8; ++r) {
      assertInRange({static_cast<int>(c), static_cast<int>(r)});
      if (other_board.board[c][r]) {
        Name name_piece = other_board.board[c][r]->getName();
        Color color_piece = other_board.board[c][r]->getColor();
        Point p{static_cast<int>(c), static_cast<int>(r)};
        temporary_board.setPiece(name_piece, color_piece, p);
      } else {
        temporary_board.board[c][r] = nullptr;
      }
    }
  }
  return temporary_board;
}

// questa funzione mi permette di posizionare pezzi sulla scacchiera
void Board::setPiece(Name type, Color color, Point p) {
  switch (type) {
    case Name::queen:
      board[static_cast<std::size_t>(p.c)][static_cast<std::size_t>(p.r)] =
          std::make_unique<Queen>(color);
      break;
    case Name::bishop:
      board[static_cast<std::size_t>(p.c)][static_cast<std::size_t>(p.r)] =
          std::make_unique<Bishop>(color);
      break;
    case Name::knight:
      board[static_cast<std::size_t>(p.c)][static_cast<std::size_t>(p.r)] =
          std::make_unique<Knight>(color);
      break;
    case Name::rook:
      board[static_cast<std::size_t>(p.c)][static_cast<std::size_t>(p.r)] =
          std::make_unique<Rook>(color);
      break;
    case Name::pawn:
      board[static_cast<std::size_t>(p.c)][static_cast<std::size_t>(p.r)] =
          std::make_unique<Pawn>(color);
      break;
    case Name::king:
      board[static_cast<std::size_t>(p.c)][static_cast<std::size_t>(p.r)] =
          std::make_unique<King>(color);
      break;
    default:
      break;
  }
  board[static_cast<std::size_t>(p.c)][static_cast<std::size_t>(p.r)]
      ->setPositionImage({p.c, p.r});
}

// posiziona tutti i pezzi sulla scacchiera
void Board::setPieces() {
  for (int c = 0; c < 8; ++c) {
    setPiece(pawn, White, {c, 6});
    setPiece(pawn, Black, {c, 1});
  }
  setPiece(rook, Black, {0, 0});
  setPiece(knight, Black, {1, 0});
  setPiece(bishop, Black, {2, 0});
  setPiece(queen, Black, {3, 0});
  setPiece(king, Black, {4, 0});
  setPiece(bishop, Black, {5, 0});
  setPiece(knight, Black, {6, 0});
  setPiece(rook, Black, {7, 0});
  setPiece(rook, White, {0, 7});
  setPiece(knight, White, {1, 7});
  setPiece(bishop, White, {2, 7});
  setPiece(queen, White, {3, 7});
  setPiece(king, White, {4, 7});
  setPiece(bishop, White, {5, 7});
  setPiece(knight, White, {6, 7});
  setPiece(rook, White, {7, 7});
}

//////////// Creazione dell'interfaccia grafica
// traduzione point - pixel per posizionare le celle
void Piece::setPositionImage(Point p) {
  sprite_.setPosition(static_cast<float>(p.c) * CELL_SIZE + 40.0f -
                          sprite_.getGlobalBounds().width / 2,
                      static_cast<float>(p.r) * CELL_SIZE + 40.0f -
                          sprite_.getGlobalBounds().height / 2);
}

// disegna la scacchiera vuota sulla window
void Board::drawBoard() {            // questa è la finestra grafica SFML
  const float cellSize = CELL_SIZE;  // dimensione della casella
  for (int c = 0; c < 8; ++c) {
    for (int r = 0; r < 8; ++r) {
      sf::RectangleShape& cell = ((c + r) % 2 == 0) ? cellWhite_ : cellBlack_;
      cell.setPosition(static_cast<float>(c) * cellSize,
                       static_cast<float>(r) * cellSize);
      window_.draw(cell);
    }
  }
}

// disegna i pezzi sulla window
void Board::drawPieces() {
  for (std::size_t c = 0; c < 8; ++c) {
    for (std::size_t r = 0; r < 8; ++r) {
      if (board[c][r]) {
        board[c][r]->drawPiece(window_);
      }
    }
  }
}

void Board::clearPieceAt(Point x) {
  board[static_cast<std::size_t>(x.c)][static_cast<std::size_t>(x.r)] = nullptr;
}

////////// Movimento dei pezzi
Piece* Board::selectPiece(Point p) const {
  if (p.c < 0 or p.c >= 8 or p.r < 0 or p.r >= 8) {
    throw std::runtime_error{"Point out of board"};
  } else if (board[static_cast<std::size_t>(p.c)]
                  [static_cast<std::size_t>(p.r)] == nullptr) {
    return nullptr;
  }
  return board[static_cast<std::size_t>(p.c)][static_cast<std::size_t>(p.r)]
      .get();
}

void Board::movePiece(Point from, Point to) {
  // std::move sposta l'oggetto da from a to, distruggendo l'oggetto che
  // conteneva in precedenza to.
  if (selectPiece(from)) {
    selectPiece(from)->setPositionImage({to.c, to.r});
    board[static_cast<std::size_t>(to.c)][static_cast<std::size_t>(to.r)] =
        std::move(board[static_cast<std::size_t>(from.c)]
                       [static_cast<std::size_t>(from.r)]);
    // serve per le mosse "speciali" di questi pezzi
    if (selectPiece(to)->getName() == king ||
        selectPiece(to)->getName() == rook ||
        selectPiece(to)->getName() == pawn) {
      selectPiece(to)->setMoved(true);
    }
  } else {
    throw std::runtime_error{"Selected cell is empty"};
  }
}
/////////
/////////// Controllo della scacchiera (clearPath e kingPosition)
bool Board::clearPath(Point from, Point to) const {
  if (from.r == to.r) {
    return clearOrizzontalPath(from, to);
  } else if (from.c == to.c) {
    return clearVerticalPath(from, to);
  } else if (abs(from.r - to.r) == abs(from.c - to.c)) {
    return clearDiagonalPath(from, to);
  }
  return true;
}

bool Board::clearOrizzontalPath(Point from, Point to) const {
  int x = (to.c - from.c > 0) ? +1 : -1;
  for (int column = from.c + x; column != to.c; column += x) {
    Point p{column, to.r};
    if (selectPiece({p}) != nullptr) {
      return false;
    }
  }
  return true;
}

bool Board::clearVerticalPath(Point from, Point to) const {
  int y = (to.r - from.r > 0) ? +1 : -1;
  for (int row = from.r + y; row != to.r; row += y) {
    Point p{to.c, row};
    if (selectPiece(p) != nullptr) {
      return false;
    }
  }
  return true;
}

bool Board::clearDiagonalPath(Point from, Point to) const {
  int x = (to.c - from.c > 0) ? +1 : -1;
  int y = (to.r - from.r > 0) ? +1 : -1;
  int column = from.c + x;
  int row = from.r + y;
  while (column != to.c && row != to.r) {
    Point p{column, row};
    if (selectPiece(p) != nullptr) {
      return false;
    }
    column = column + x;
    row = row + y;
  }
  return true;
}

// restituisce la posizione del re, ma potrebbe essere migliorata
Point Board::kingPosition(Color color) const {
  for (int c{0}; c < 8; ++c) {
    for (int r{0}; r < 8; ++r) {
      Piece* piece = selectPiece({c, r});
      if (piece != nullptr && piece->getName() == king &&
          piece->getColor() == color) {
        return {c, r};
      }
    }
  }
  throw std::runtime_error("King not found on the board");
}

bool Board::isCastling(Point p_from, Point p_to) const {
  Piece* king = selectPiece(p_from);
  if (king && !king->getMoved()) {
    return (abs(p_to.c - p_from.c) == 2);
  }
  return false;
}

// riconosce se la mossa appena compiuta ha portato un pedone a promozione
bool Board::isPromotion(Point from, Point to) const {
  Piece* piece = selectPiece(from);
  if (piece != nullptr && piece->getName() == pawn) {
    return ((piece->getColor() == White && to.r == 0) ||
            (piece->getColor() == Black && to.r == 7));
  }
  return false;
}

// promuove il pedone a pezzo desiderato
void Board::promote(Point p_pawn, Name piece, Color color) {
  setPiece(piece, color, p_pawn);
}
