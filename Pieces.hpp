#ifndef PIECES_HPP
#define PIECES_HPP

#include <assert.h>

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <string>

// la classe pezzo sarà usata come base

enum Color { White, Black };
enum Column { A = 0, B, C, D, E, F, G, H };
enum Name { king, queen, bishop, knight, rook, pawn };

struct Point {
  Column c;
  int r;  // 0-7 (riga da 1-8)
};

bool operator==(const Point& lp, const Point& rp);

// LA CLASSE MADRE
class Piece {
 protected:  // così potrà essere usato anche dalle classi derivate
  Name name_;
  Color color_;
  bool moved_{false};
  sf::Texture texture_;
  sf::Sprite sprite_;

 public:
  // costruttore
  Piece(Name name_, Color color);

  Piece(const Piece& other);

  // metodi per accedere alle variabili protette
  Name getName();
  Color getColor();
  bool isWhite() const;
  bool getMoved();

  // metodi per modificare le variabili protette
  inline void setName(Name new_name);
  inline void setColor(Color new_color);
  void setMoved(bool has_moved);
  void setTexture(const sf::Texture& texture);

  void setPositionImage(Point p);  // definita in board

  // puramente virtuali (riguardano i singoli pezzi)
  inline virtual bool validPieceMove(Point cell_from, Point cell_to) = 0;  // mosse dei pezzi
  virtual void loadTexture() = 0;                         // immagini dei pezzi
  virtual void drawPiece(sf::RenderWindow& window) = 0;   // disegna i pezzi PUO ESSERE GENERALIZZATA?
};

// LE CLASSI DERIVATE: I SINGOLI PEZZI

// RE
class King : public Piece {
 public:
  // Constructor
  King(Color color);

  bool validPieceMove(Point cell_from, Point cell_to) override;
  inline bool getMoved();
  void setMoved(bool has_moved);
  void loadTexture() override;
  void drawPiece(sf::RenderWindow& window) override;
};

// REGINA
class Queen : public Piece {
 public:
  // Constructor
  Queen(Color color);
  bool validPieceMove(Point cell_from, Point cell_to) override;

  void loadTexture() override;
  void drawPiece(sf::RenderWindow& window) override;
};

// CAVALLO
class Knight : public Piece {
 public:
  // Constructor
  Knight(Color color);

  bool validPieceMove(Point cell_from, Point cell_to) override;
  void loadTexture() override;
  void drawPiece(sf::RenderWindow& window) override;
};

// ALFIERE
class Bishop : public Piece {
 public:
  // Constructor
  Bishop(Color color);

  bool validPieceMove(Point cell_from, Point cell_to) override;
  void loadTexture() override;
  void drawPiece(sf::RenderWindow& window) override;
};

// TORRE
class Rook : public Piece {
 public:
  // Constructor
  Rook(Color color);

  bool validPieceMove(Point cell_from, Point cell_to) override;
  void loadTexture() override;
  void drawPiece(sf::RenderWindow& window) override;
};

// PEDONE
class Pawn : public Piece {
 public:
  Pawn(Color color);

  bool validPieceMove(Point cell_from, Point cell_to) override;

  void loadTexture() override;
  void drawPiece(sf::RenderWindow& window) override;
};

#endif