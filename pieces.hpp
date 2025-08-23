#ifndef PIECES_HPP
#define PIECES_HPP

#include <SFML/Graphics.hpp>
#include <cassert>
#include <iostream>
#include <memory>
#include <string>

// la classe pezzo sarà usata come base
namespace chess {
enum Color { White, Black };
enum Name { king, queen, bishop, knight, rook, pawn };

struct Point {
  int c;  // 0-7 (colonna da 1-8)
  int r;  // 0-7 (riga da 1-8)
};

bool operator==(Point lp, Point rp);
bool operator!=(Point lp, Point rp);

// CLASSE MADRE
class Piece {
 protected:
  Name name_;
  Color color_;
  bool moved_{false};
  sf::Texture texture_;
  sf::Sprite sprite_;

  static inline void assertInRange_Pieces(Point p) {
    assert(p.c >= 0 && p.c < 8 && p.r >= 0 && p.r < 8);
    (void)p;  // evito warning in build Release
  }

 public:
  // costruttore
  Piece(Name name_, Color color);

  // metodi per accedere alle variabili protette
  Name getName();
  Color getColor();
  bool getMoved();

  // metodi per modificare le variabili protette
  void setName(Name new_name);
  void setColor(Color new_color);
  void setMoved(bool has_moved);
  virtual void drawPiece(sf::RenderWindow& window);  // disegna il pezzo

  void setPositionImage(Point p);  // definita in board

  // puramente virtuali (riguardano i singoli pezzi)
  inline virtual bool validPieceMove(Point cell_from,
                                     Point cell_to) = 0;  // mosse dei pezzi
  virtual void loadTexture() = 0;                         // immagini dei pezzi
};

// LE CLASSI DERIVATE: I SINGOLI PEZZI

// RE
class King : public Piece {
 public:
  King(Color color);

  bool validPieceMove(Point cell_from, Point cell_to) override;
  bool getMoved();
  void setMoved(bool has_moved);
  void loadTexture() override;
};

// REGINA
class Queen : public Piece {
 public:
  Queen(Color color);

  bool validPieceMove(Point cell_from, Point cell_to) override;
  void loadTexture() override;
};

// CAVALLO
class Knight : public Piece {
 public:
  Knight(Color color);

  bool validPieceMove(Point cell_from, Point cell_to) override;
  void loadTexture() override;
};

// ALFIERE
class Bishop : public Piece {
 public:
  Bishop(Color color);

  bool validPieceMove(Point cell_from, Point cell_to) override;
  void loadTexture() override;
};

// TORRE
class Rook : public Piece {
 public:
  Rook(Color color);

  bool validPieceMove(Point cell_from, Point cell_to) override;
  void loadTexture() override;
};

// PEDONE
class Pawn : public Piece {
 public:
  Pawn(Color color);
  bool validPieceMove(Point cell_from, Point cell_to) override;
  void loadTexture() override;
};
}  // namespace chess
#endif
