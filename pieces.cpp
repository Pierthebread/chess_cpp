#include "pieces.hpp"
namespace chess {
bool operator==(Point lp, Point rp) {
  assert(lp.c >= 0 && lp.c < 8 && lp.r >= 0 && lp.r < 8);
  assert(rp.c >= 0 && rp.c < 8 && rp.r >= 0 && rp.r < 8);
  return lp.c == rp.c && lp.r == rp.r;
}

bool operator!=(Point lp, Point rp) {
  assert(lp.c >= 0 && lp.c < 8 && lp.r >= 0 && lp.r < 8);
  assert(rp.c >= 0 && rp.c < 8 && rp.r >= 0 && rp.r < 8);
  return lp.c != rp.c || lp.r != rp.r;
}

// LA CLASSE MADRE
// Costructor
Piece::Piece(Name name, Color color) : name_(name), color_(color) {
  assert(name >= king && name <= pawn && "Nome pezzo invalido");
  assert(color == White || color == Black);
}

// metodi per accedere alle variabili protette
Name Piece::getName() {
  assert(name_ >= king && name_ <= pawn);
  return name_;
}
Color Piece::getColor() {
  assert(color_ == White || color_ == Black);
  return color_;
}
bool Piece::getMoved() { return moved_; };

// metodi per modificare le variabili protette
void Piece::setName(Name new_name) { name_ = new_name; }
void Piece::setColor(Color new_color) { color_ = new_color; }
void Piece::setMoved(bool has_moved) { moved_ = has_moved; };

void Piece::drawPiece(sf::RenderWindow& window) {
  if (!texture_.getSize().x) {
    throw std::runtime_error("drawPiece: Texture non caricata");
  }
  window.draw(sprite_);
}

// LE CLASSI DERIVATE: I SINGOLI PEZZI

// KING

King::King(Color color) : Piece(king, color) { loadTexture(); };

bool King::validPieceMove(Point cell_from, Point cell_to) {
  assertInRange_Pieces(cell_from);
  assertInRange_Pieces(cell_to);

  int delta_column{abs(cell_from.c - cell_to.c)};
  int delta_row{abs(cell_from.r - cell_to.r)};

  return delta_column <= 1 && delta_row <= 1;
};

void King::loadTexture() {
  std::string colorPiece =
      (color_ == Color::White) ? "Image/whiteKing.png" : "Image/blackKing.png";
  if (!texture_.loadFromFile(colorPiece)) {
    throw std::runtime_error("King::loadTexture: impossibile caricare " +
                             colorPiece);
  }
  sprite_.setTexture(texture_);
}

// REGINA
Queen::Queen(Color color) : Piece(queen, color) { loadTexture(); };

bool Queen::validPieceMove(Point cell_from, Point cell_to) {
  assertInRange_Pieces(cell_from);
  assertInRange_Pieces(cell_to);

  int delta_column{
      abs(cell_from.c - cell_to.c)};  // per muoversi in diagonale colonna
  int delta_row{
      abs(cell_from.r - cell_to.r)};  // per muoversi in diagonale riga

  if (delta_column == delta_row) {
    return true;
  }
  if (cell_from.r == cell_to.r || cell_from.c == cell_to.c) {
    return true;
  }
  return false;
};

void Queen::loadTexture() {
  std::string colorPiece = (color_ == Color::White) ? "Image/whiteQueen.png"
                                                    : "Image/blackQueen.png";
  if (!texture_.loadFromFile(colorPiece)) {
    throw std::runtime_error("Queen::loadTexture: impossibile caricare " +
                             colorPiece);
  }
  sprite_.setTexture(texture_);
}

// CAVALLO

Knight::Knight(Color color) : Piece(knight, color) { loadTexture(); };

bool Knight::validPieceMove(Point cell_from, Point cell_to) {
  assertInRange_Pieces(cell_from);
  assertInRange_Pieces(cell_to);

  int delta_column{abs(cell_from.c - cell_to.c)};
  int delta_row{abs(cell_from.r - cell_to.r)};

  return delta_column * delta_row == 2;
};

void Knight::loadTexture() {
  std::string colorPiece = (color_ == Color::White) ? "Image/whiteKnight.png"
                                                    : "Image/blackKnight.png";
  if (!texture_.loadFromFile(colorPiece)) {
    throw std::runtime_error("Knight::loadTexture: impossibile caricare " +
                             colorPiece);
  }
  sprite_.setTexture(texture_);
}

// ALFIERE
Bishop::Bishop(Color color) : Piece(bishop, color) { loadTexture(); };

bool Bishop::validPieceMove(Point cell_from, Point cell_to) {
  assertInRange_Pieces(cell_from);
  assertInRange_Pieces(cell_to);

  int delta_column{abs(cell_from.c - cell_to.c)};
  int delta_row{abs(cell_from.r - cell_to.r)};

  return delta_column == delta_row;
};

void Bishop::loadTexture() {
  std::string colorPiece = (color_ == Color::White) ? "Image/whiteBishop.png"
                                                    : "Image/blackBishop.png";
  if (!texture_.loadFromFile(colorPiece)) {
    throw std::runtime_error("Bishop::loadTexture: impossibile caricare " +
                             colorPiece);
  }
  sprite_.setTexture(texture_);
}

// TORRE
Rook::Rook(Color color) : Piece(rook, color) { loadTexture(); };

bool Rook::validPieceMove(Point cell_from, Point cell_to) {
  assertInRange_Pieces(cell_from);
  assertInRange_Pieces(cell_to);

  return cell_from.r == cell_to.r || cell_from.c == cell_to.c;
};

void Rook::loadTexture() {
  std::string colorPiece =
      (color_ == Color::White) ? "Image/whiteRook.png" : "Image/blackRook.png";
  if (!texture_.loadFromFile(colorPiece)) {
    throw std::runtime_error("Rook::loadTexture: impossibile caricare " +
                             colorPiece);
  }
  sprite_.setTexture(texture_);
}

// PEDONE
Pawn::Pawn(Color color) : Piece(pawn, color) { loadTexture(); };

bool Pawn::validPieceMove(Point cell_from, Point cell_to) {
  assertInRange_Pieces(cell_from);
  assertInRange_Pieces(cell_to);

  int direction = (color_ == White) ? +1 : -1;

  // mossa doppia
  if (cell_from.c == cell_to.c && moved_ == false &&
      cell_from.r == cell_to.r + 2 * direction) {
    return true;
  }

  // solo mossa e mangiare
  if (abs(cell_from.c - cell_to.c) <= 1 &&
      cell_from.r == cell_to.r + direction) {
    return true;
  }

  return false;
};

void Pawn::loadTexture() {
  std::string colorPiece =
      (color_ == Color::White) ? "Image/whitePawn.png" : "Image/blackPawn.png";
  if (!texture_.loadFromFile(colorPiece)) {
    throw std::runtime_error("Rook::loadTexture: impossibile caricare " +
                             colorPiece);
  }
  sprite_.setTexture(texture_);
}
}  // namespace chess