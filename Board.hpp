#ifndef SCACCHIERA_HPP
#define SCACCHIERA_HPP

#include <array>
#include <memory>

#include "Pieces.hpp"
#include "Player.hpp"

enum castleType { Long, Short };

constexpr float CELL_SIZE = 80.0f;

class Board {
  std::array<std::array<std::unique_ptr<Piece>, 8>, 8> board;
  sf::RenderWindow& window_;
  sf::RectangleShape cellWhite_;
  sf::RectangleShape cellBlack_;
  Point whiteKingPos_;
  Point blackKingPos_;

  static inline void assertInRange_Board(Point p) {
    assert(p.c >= 0 && p.c < 8 && p.r >= 0 && p.r < 8);
  }

 public:
  // Costruttore
  Board(sf::RenderWindow& window);

  // clone board
  Board cloneBoard(const Board& other_board);

  // Crea il pezzo indicato e lo fa puntare dal puntatore indicato
  void setPiece(Name, Color, Point);

  // crea i pezzi e li assegna ai puntatori iniziali insieme alle immagini
  void setPieces();

  // elimina un pezzo
  void deletePiece(Point);

  // Creazione dell'interfaccia
  void drawPieces();
  void drawBoard();

  // posizione del re
  Point getKingPosition(Color) const;
  void setKingPosition(Color, Point);

  // Movimento dei pezzi
  Piece* selectPiece(Point) const;       //  puntatore data la posizione
  void movePiece(Point from, Point to);  // muovo un pezzo

  // mosse speciali
  bool isPromotion(Point to, Point from) const;
  void promote(Point p_pawn, Name piece, Color color);
  bool isCastling(Point from, Point to) const;

  // controllo della scacchiera
  bool clearPath(Point from, Point to) const;
  bool clearHorizontalPath(Point from, Point to) const;
  bool clearVerticalPath(Point from, Point to) const;
  bool clearDiagonalPath(Point from, Point to) const;
};
#endif