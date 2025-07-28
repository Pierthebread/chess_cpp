#include "Pieces.hpp"

bool operator==(const Point& lp, const Point& rp) {
  return lp.c == rp.c && lp.r == rp.r;
}

// LA CLASSE MADRE
// Costructor
Piece::Piece(Name name, Color color) : name_(name), color_(color) {}

// costruttore di copia
Piece::Piece(const Piece& other)
    : name_(other.name_), color_(other.color_), pos_(other.pos_) {
  // Copy constructor
}

// metodi per accedere alle variabili protette
Name Piece::getName() { return name_; }
Color Piece::getColor() { return color_; }  // PERCHE' NE ABBIAMO DUE?
bool Piece::isWhite() const { return color_ == Color::White; }  //
Point Piece::getPosition() { return pos_; }
bool Piece::getMoved() { return moved_; };

// metodi per modificare le variabili protette
void Piece::setName(Name new_name) { name_ = new_name; }
void Piece::setColor(Color new_color) { color_ = new_color; }
void Piece::setPosition(Point new_position) { pos_ = new_position; }
void Piece::setMoved(bool has_moved) { moved_ = has_moved; };

// void Piece::setTexture(const sf::Texture& texture) {
//   sprite_.setTexture(texture);
// }

// void Piece::draw(sf::RenderWindow& window) { window.draw(sprite_); }

// LE CLASSI DERIVATE: I SINGOLI PEZZI

// RE
// Constructor

King::King(Color color) : Piece(king, color) { loadTexture(); };

// le mosse del re
bool King::validPieceMove(Point point_to) {
  int delta_column{abs(point_to.c - pos_.c)};
  int delta_row{abs(point_to.r - pos_.r)};

  return delta_column <= 1 && delta_row <= 1;
};

void King::loadTexture() {
  std::string colorPiece =
      (color_ == Color::White) ? "whiteKing.png" : "blackKing.png";
  if (texture_.loadFromFile(colorPiece)) {
    sprite_.setTexture(texture_);
  } else {
    std::cerr << "Error " << colorPiece << '\n';
  }
}

void King::draw(sf::RenderWindow& window) { window.draw(sprite_); }

// REGINA
// Constructor
Queen::Queen(Color color) : Piece(queen, color) { loadTexture(); };

// le mosse della regina
bool Queen::validPieceMove(Point point_to) {
  int delta_column{
      abs(point_to.c - pos_.c)};            // per muoversi in diagonale colonna
  int delta_row{abs(point_to.r - pos_.r)};  // per muoversi in diagonale riga

  if (delta_column == delta_row) {
    return true;
  }
  if (point_to.r == pos_.r || point_to.c == pos_.c) {
    return true;
  }
  return false;
};

void Queen::loadTexture() {
  std::string colorPiece =
      (color_ == Color::White) ? "whiteQueen.png" : "blackQueen.png";
  if (texture_.loadFromFile(colorPiece)) {
    sprite_.setTexture(texture_);
  } else {
    std::cerr << "Error " << colorPiece << '\n';
  }
}

void Queen::draw(sf::RenderWindow& window) { window.draw(sprite_); }

// CAVALLO
// Constructor
Knight::Knight(Color color) : Piece(knight, color) { loadTexture(); };

// le mosse del cavallo
bool Knight::validPieceMove(Point point_to) {
  int delta_column{abs(point_to.c - pos_.c)};
  int delta_row{abs(point_to.r - pos_.r)};

  return delta_column * delta_row == 2;
};

void Knight::loadTexture() {
  std::string colorPiece =
      (color_ == Color::White) ? "whiteKnight.png" : "blackKnight.png";
  if (texture_.loadFromFile(colorPiece)) {
    sprite_.setTexture(texture_);
  } else {
    std::cerr << "Error " << colorPiece << '\n';
  }
}

void Knight::draw(sf::RenderWindow& window) { window.draw(sprite_); }

// ALFIERE
// Constructor
Bishop::Bishop(Color color) : Piece(bishop, color) { loadTexture(); };

// le mosse dell'alfiere
bool Bishop::validPieceMove(Point p_to) {
  int delta_column{abs(p_to.c - pos_.c)};
  int delta_row{abs(p_to.r - pos_.r)};

  return delta_column == delta_row;
};

void Bishop::loadTexture() {
  std::string colorPiece =
      (color_ == Color::White) ? "whiteBishop.png" : "blackBishop.png";
  if (texture_.loadFromFile(colorPiece)) {
    sprite_.setTexture(texture_);
  } else {
    std::cerr << "Error " << colorPiece << '\n';
  }
}

void Bishop::draw(sf::RenderWindow& window) { window.draw(sprite_); }

// TORRE
// Constructor
Rook::Rook(Color color) : Piece(rook, color) { loadTexture(); };

// le mosse della torre
bool Rook::validPieceMove(Point p_to) {
  return p_to.r == pos_.r || p_to.c == pos_.c;
};

void Rook::loadTexture() {
  std::string colorPiece =
      (color_ == Color::White) ? "whiteRook.png" : "blackRook.png";
  if (texture_.loadFromFile(colorPiece)) {
    sprite_.setTexture(texture_);
  } else {
    std::cerr << "Error " << colorPiece << '\n';
  }
}

void Rook::draw(sf::RenderWindow& window) { window.draw(sprite_); }

// PEDONE
// Costructor
Pawn::Pawn(Color color) : Piece(pawn, color) { loadTexture(); };

// le mosse del pedone
bool Pawn::validPieceMove(Point point_to) {
  int direction = (color_ == White) ? -1 : +1;

  if (pos_.c == point_to.c && point_to.r == pos_.r + direction) {
    return true;
  }

  // solo mossa
  if (pos_.c == point_to.c && moved_ == false &&
      point_to.r == pos_.r + 2 * direction) {
    return true;
  }

  // per mangiare
  if (abs(pos_.c - point_to.c) == 1 && point_to.r == pos_.r + direction) {
    return true;
  }

  return false;
};

void Pawn::loadTexture() {
  std::string colorPiece =
      (color_ == Color::White) ? "whitePawn.png" : "blackPawn.png";
  if (texture_.loadFromFile(colorPiece)) {
    sprite_.setTexture(texture_);
  } else {
    std::cerr << "Error " << colorPiece << '\n';
  }
}

void Pawn::draw(sf::RenderWindow& window) { window.draw(sprite_); }