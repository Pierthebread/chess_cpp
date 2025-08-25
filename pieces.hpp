#ifndef PIECES_HPP
#define PIECES_HPP

#include <SFML/Graphics.hpp>
#include <cassert>
#include <iostream>
#include <memory>
#include <string>

namespace chess {
enum Color { White, Black };
enum Name { king, queen, bishop, knight, rook, pawn };

struct Point {
  int c;  // 0-7 (colonna da 1-8)
  int r;  // 0-7 (riga da 1-8)
};

bool operator==(Point lp, Point rp);
bool operator!=(Point lp, Point rp);

static inline void assertInRange(Point p) {
  assert(p.c >= 0 && p.c < 8 && p.r >= 0 && p.r < 8);
  (void)p;  // I avoid warnings in build Release
}

class Piece {
 protected:
  Name name_;
  Color color_;
  bool moved_{false};
  sf::Texture texture_;
  sf::Sprite sprite_;

 public:
  Piece(Name name_, Color color);

  Name getName() const noexcept;
  Color getColor() const noexcept;
  bool getMoved() const noexcept;

  void setName(Name new_name);
  void setColor(Color new_color);
  void setMoved(bool has_moved);
  virtual void drawPiece(sf::RenderWindow& window);

  void setPositionImage(Point p);  // defined on the board

  virtual bool validPieceMove(Point cell_from, Point cell_to) = 0;
  virtual void loadTexture() = 0;
};

// King
class King : public Piece {
 public:
  King(Color color);

  bool validPieceMove(Point cell_from, Point cell_to) override;
  bool getMoved() const noexcept;
  void setMoved(bool has_moved);
  void loadTexture() override;
};

// Queen
class Queen : public Piece {
 public:
  Queen(Color color);

  bool validPieceMove(Point cell_from, Point cell_to) override;
  void loadTexture() override;
};

// Knight
class Knight : public Piece {
 public:
  Knight(Color color);

  bool validPieceMove(Point cell_from, Point cell_to) override;
  void loadTexture() override;
};

// Bishop
class Bishop : public Piece {
 public:
  Bishop(Color color);

  bool validPieceMove(Point cell_from, Point cell_to) override;
  void loadTexture() override;
};

// Rook
class Rook : public Piece {
 public:
  Rook(Color color);

  bool validPieceMove(Point cell_from, Point cell_to) override;
  void loadTexture() override;
};

// Pawn
class Pawn : public Piece {
 public:
  Pawn(Color color);
  bool validPieceMove(Point cell_from, Point cell_to) override;
  void loadTexture() override;
};
}  // namespace chess
#endif
