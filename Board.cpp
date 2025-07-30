
#include "Board.hpp"

// Costruttore
Board::Board() {
  for (auto& column : board) {
    for (auto& cell : column) {
      cell = nullptr;
    }
  }
}

// questa funzione mi permette di posizionare pezzi sulla scacchiera
// IMPORTANTE manca l'assegnazione dell'immagine
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
  Board::selectPiece({p.c, p.r})
      ->setPosition({p.c, p.r});  /////////aaaaaaaaaaaaaaaa
}

void Board::setPieces() {
  for (unsigned int c = 0; c < 8; ++c) {
    board[c][6] = std::make_unique<Pawn>(White);
    board[c][6]->setPositionImage({static_cast<Column>(c), 6});
    board[c][1] = std::make_unique<Pawn>(Black);
    board[c][1]->setPositionImage({static_cast<Column>(c), 1});
  }
  board[0][7] = std::make_unique<Rook>(White);
  board[0][7]->setPositionImage({A, 7});
  board[7][7] = std::make_unique<Rook>(White);
  board[7][7]->setPositionImage({H, 7});
  board[0][0] = std::make_unique<Rook>(Black);
  board[0][0]->setPositionImage({A, 0});
  board[7][0] = std::make_unique<Rook>(Black);
  board[7][0]->setPositionImage({H, 0});

  board[1][7] = std::make_unique<Knight>(White);
  board[1][7]->setPositionImage({B, 7});
  board[6][7] = std::make_unique<Knight>(White);
  board[6][7]->setPositionImage({G, 7});
  board[1][0] = std::make_unique<Knight>(Black);
  board[1][0]->setPositionImage({B, 0});
  board[6][0] = std::make_unique<Knight>(Black);
  board[6][0]->setPositionImage({G, 0});

  board[2][7] = std::make_unique<Bishop>(White);
  board[2][7]->setPositionImage({C, 7});
  board[5][7] = std::make_unique<Bishop>(White);
  board[5][7]->setPositionImage({F, 7});
  board[2][0] = std::make_unique<Bishop>(Black);
  board[2][0]->setPositionImage({C, 0});
  board[5][0] = std::make_unique<Bishop>(Black);
  board[5][0]->setPositionImage({F, 0});

  board[3][7] = std::make_unique<Queen>(White);
  board[3][7]->setPositionImage({D, 7});
  board[3][0] = std::make_unique<Queen>(Black);
  board[3][0]->setPositionImage({D, 0});

  board[4][7] = std::make_unique<King>(White);
  board[4][7]->setPositionImage({E, 7});
  board[4][0] = std::make_unique<King>(Black);
  board[4][0]->setPositionImage({E, 0});
}
//////////// Creazione dell'interfaccia
void Piece::setPositionImage(Point p) {
  sprite_.setPosition(static_cast<float>(p.c) * 80.f + 40.f -
                          sprite_.getGlobalBounds().width / 2,
                      static_cast<float>(p.r) * 80.f + 40.f -
                          sprite_.getGlobalBounds().height / 2);
}

void Board::draw(sf::RenderWindow& window) {
  for (unsigned int c = 0; c < 8; ++c) {
    for (unsigned int r = 0; r < 8; ++r) {
      if (board[c][r] != nullptr) {
        board[c][r]->setPosition({static_cast<Column>(static_cast<float>(c)),
                                  static_cast<int>(static_cast<float>(r))});
        board[c][r]->draw(window);
      }
    }
  }
}

void Board::renderBoard(
    sf::RenderWindow& window) {  // questa è la finestra grafica SFML
  const float cellSize =
      80.f;  // do la dimensione della casella, ho usato 100.f
             // così da disegnare una casella 100x100 pixel
  sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
  for (unsigned int c = 0; c < 8; ++c) {
    for (unsigned int r = 0; r < 8; ++r) {
      if ((c + r) % 2 == 0)  // se pari è bianca, se è dispari è nera
        cell.setFillColor(sf::Color(245, 245, 220));
      else
        cell.setFillColor(sf::Color(139, 69, 19));
      cell.setPosition(static_cast<float>(c) * cellSize,
                       static_cast<float>(r) * cellSize);
      // disegna la casella nella posizione giusta
      window.draw(cell);
    }
  }
}
//
//////////
////////// Movimento dei pezzi
Piece* Board::selectPiece(Point p) {
  // int c = static_cast<int>(p.c);
  if (p.c < 0 or p.c >= 8 or p.r < 0 or p.r >= 8) {
    throw std::runtime_error{"Point out of board"};
  } else if (board[static_cast<std::size_t>(p.c)]
                  [static_cast<std::size_t>(p.r)] == nullptr) {
    return nullptr;
  }
  return board[static_cast<std::size_t>(p.c)][static_cast<std::size_t>(p.r)]
      .get();  // capisci perché l'abbiamo fatto così
}

void Board::movePiece(Point from, Point to) {
  // std::move sposta l'oggetto da from a to, distruggendo l'oggetto che
  // conteneva in precedenza to.
  if (selectPiece(from) != nullptr) {
    selectPiece(from)->setPositionImage({static_cast<Column>(to.c), to.r});
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
bool Board::clearPath(Point from, Point to) {
  if (from.r == to.r) {
    return clearOrizzontalPath(from, to);
  } else if (from.c == to.c) {
    return clearVerticalPath(from, to);
  } else if (from.r - to.r == from.c - to.c) {
    return clearDiagonalPath(from, to);
  }
  return true;
}

bool Board::clearOrizzontalPath(Point from, Point to) {
  int x = (to.c - from.c > 0) ? +1 : -1;
  for (int column = from.c + x; column != to.c; column += x) {
    Point p{static_cast<Column>(column), to.r};
    if (selectPiece({p}) != nullptr) {
      return false;
    }
  }
  return true;
}

bool Board::clearVerticalPath(Point from, Point to) {
  int y = (to.r - from.r > 0) ? +1 : -1;
  for (int row = from.r + y; row != to.r; row += y) {
    Point p{to.c, row};
    if (selectPiece(p) != nullptr) {
      return false;
    }
  }
  return true;
}

bool Board::clearDiagonalPath(Point from, Point to) {
  int x = (to.c - from.c > 0) ? +1 : -1;
  int y = (to.r - from.r > 0) ? +1 : -1;
  int column = from.c + x;
  int row = from.r + y;
  while (column != to.c && row != to.r) {
    Point p{static_cast<Column>(column), row};
    if (selectPiece(p) != nullptr) {
      return false;
    }
    column = column + x;
    row = row + y;
  }
  return true;
}

// restituisce la posizione del re, ma potrebbe essere migliorata
Point Board::kingPosition(Color color) {
  for (int c{0}; c < 8; ++c) {
    for (int r{0}; r < 8; ++r) {
      Piece* piece = selectPiece({static_cast<Column>(c), r});
      if (piece != nullptr && piece->getName() == king &&
          piece->getColor() == color) {
        return {static_cast<Column>(c), r};
      }
    }
  }
  throw std::runtime_error("King not found on the board");
}

// riconosce se la mossa appena compiuta ha portato un pedone a promozione
bool Board::isPromotion(Point from, Point to) {
  Piece* piece = selectPiece(from);
  if (piece != nullptr && piece->getName() == pawn &&
      ((piece->getColor() == White && to.r == 0) ||
       (piece->getColor() == Black && to.r == 7))) {
    return true;
  };
  return false;
};

// promuove il pedone a pezzo desiderato
void Board::promote(Point p_pawn, Name piece) {
  if (piece != king) {
    if (selectPiece(p_pawn) != nullptr &&
        selectPiece(p_pawn)->getName() == pawn) {
      setPiece(piece, selectPiece(p_pawn)->getColor(), p_pawn);
    } else {
      throw std::runtime_error("There is no pawn in here");
    }
  } else {
    throw std::runtime_error("You can't promote to king");
  }
};
