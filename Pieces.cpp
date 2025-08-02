#include "Pieces.hpp"

bool operator==(const Point& lp, const Point& rp) {
  return lp.c == rp.c && lp.r == rp.r;
}

// LA CLASSE MADRE
// Costructor
Piece::Piece(Name name, Color color) : name_(name), color_(color) { }

// costruttore di copia
Piece::Piece(const Piece& other) : name_(other.name_), color_(other.color_) {}

// metodi per accedere alle variabili protette
Name Piece::getName() { return name_; }
Color Piece::getColor() { return color_; }  // PERCHE' NE ABBIAMO DUE?
bool Piece::isWhite() const { return color_ == Color::White; }
bool Piece::getMoved() { return moved_; };

// metodi per modificare le variabili protette
void Piece::setName(Name new_name) { name_ = new_name; }
void Piece::setColor(Color new_color) { color_ = new_color; }
void Piece::setMoved(bool has_moved) { moved_ = has_moved; };

// void Piece::setTexture(const sf::Texture& texture) {
//   sprite_.setTexture(texture);
// }

// LE CLASSI DERIVATE: I SINGOLI PEZZI

// RE
// Constructor
King::King(Color color) : Piece(king, color) { loadTexture(); };

// le mosse del re
bool King::validPieceMove(Point cell_from, Point cell_to) {
  int delta_column{abs(cell_from.c - cell_to.c)};
  int delta_row{abs(cell_from.r - cell_to.r)};

  return delta_column <= 1 && delta_row <= 1;
};

void King::loadTexture() {
  std::string colorPiece =
      (color_ == Color::White) ? "Image/whiteKing.png" : "Image/blackKing.png";
  if (texture_.loadFromFile(colorPiece)) {
    sprite_.setTexture(texture_);  // inserisce come sprite la texture caricata
  } else {
    std::cerr << "Error " << colorPiece << '\n';
  }
}

void King::drawPiece(sf::RenderWindow& window) { window.draw(sprite_); }

// REGINA
// Constructor
Queen::Queen(Color color) : Piece(queen, color) { loadTexture(); };

// le mosse della regina
bool Queen::validPieceMove(Point cell_from, Point cell_to) {
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
  std::string colorPiece =
      (color_ == Color::White) ? "Image/whiteQueen.png" : "Image/blackQueen.png";
  if (texture_.loadFromFile(colorPiece)) {
    sprite_.setTexture(texture_);
  } else {
    std::cerr << "Error " << colorPiece << '\n';
  }
}

void Queen::drawPiece(sf::RenderWindow& window) { window.draw(sprite_); }

// CAVALLO
// Constructor
Knight::Knight(Color color) : Piece(knight, color) { loadTexture(); };

// le mosse del cavallo
bool Knight::validPieceMove(Point cell_from, Point cell_to) {
  int delta_column{abs(cell_from.c - cell_to.c)};
  int delta_row{abs(cell_from.r - cell_to.r)};

  return delta_column * delta_row == 2;
};

void Knight::loadTexture() {
  std::string colorPiece =
      (color_ == Color::White) ? "Image/whiteKnight.png" : "Image/blackKnight.png";
  if (texture_.loadFromFile(colorPiece)) {
    sprite_.setTexture(texture_);
  } else {
    std::cerr << "Error " << colorPiece << '\n';
  }
}

void Knight::drawPiece(sf::RenderWindow& window) { window.draw(sprite_); }

// ALFIERE
// Constructor
Bishop::Bishop(Color color) : Piece(bishop, color) { loadTexture(); };

// le mosse dell'alfiere
bool Bishop::validPieceMove(Point cell_from, Point cell_to) {
  int delta_column{abs(cell_from.c - cell_to.c)};
  int delta_row{abs(cell_from.r - cell_to.r)};

  return delta_column == delta_row;
};

void Bishop::loadTexture() {
  std::string colorPiece =
      (color_ == Color::White) ? "Image/whiteBishop.png" : "Image/blackBishop.png";
  if (texture_.loadFromFile(colorPiece)) {
    sprite_.setTexture(texture_);
  } else {
    std::cerr << "Error " << colorPiece << '\n';
  }
}

void Bishop::drawPiece(sf::RenderWindow& window) { window.draw(sprite_); }

// TORRE
// Constructor
Rook::Rook(Color color) : Piece(rook, color) { loadTexture(); };

// le mosse della torre
bool Rook::validPieceMove(Point cell_from, Point cell_to) {
  return cell_from.r == cell_to.r || cell_from.c == cell_to.c;
};

void Rook::loadTexture() {
  std::string colorPiece =
      (color_ == Color::White) ? "Image/whiteRook.png" : "Image/blackRook.png";
  if (texture_.loadFromFile(colorPiece)) {
    sprite_.setTexture(texture_);
  } else {
    std::cerr << "Error " << colorPiece << '\n';
  }
}

void Rook::drawPiece(sf::RenderWindow& window) { window.draw(sprite_); }

// PEDONE
// Costructor
Pawn::Pawn(Color color) : Piece(pawn, color) { loadTexture(); };

// le mosse del pedone
bool Pawn::validPieceMove(Point cell_from, Point cell_to) {
  int direction = (color_ == White) ? -1 : +1;

  if (cell_from.c == cell_to.c && cell_from.r == cell_to.r + direction) {
    return true;
  }

  // solo mossa
  if (cell_from.c == cell_to.c && moved_ == false &&
      cell_from.r == cell_to.r + 2 * direction) {
    return true;
  }

  // per mangiare
  if (abs(cell_from.c - cell_to.c) == 1 &&
      cell_from.r == cell_from.r + direction) {
    return true;
  }

  return false;
};

void Pawn::loadTexture() {
  std::string colorPiece =
      (color_ == Color::White) ? "Image/whitePawn.png" : "Image/blackPawn.png";
  if (texture_.loadFromFile(colorPiece)) {
    sprite_.setTexture(texture_);
  } else {
    std::cerr << "Error " << colorPiece << '\n';
  }
}

void Pawn::drawPiece(sf::RenderWindow& window) { window.draw(sprite_); }