#ifndef SCACCHIERA_HPP
#define SCACCHIERA_HPP

#include <array>
#include <memory>

#include "Pieces.hpp"
#include "Player.hpp"

enum castleType { Long, Short };

class Board {
  std::array<std::array<std::unique_ptr<Piece>, 8>, 8> board;

  std::string pieceType_[8][8];
  sf::RenderWindow& window_;
  sf::RectangleShape cellWhite_;
  sf::RectangleShape cellBlack_;

  static inline void assertInRange(Point p) {
    assert(p.c >= 0 && p.c < 8 && p.r >= 0 && p.r < 8);
  }

 public:
  // Costruttore
  Board(sf::RenderWindow& window);

  // clone board
  Board cloneBoard(Board& other_board);

  // Crea il pezzo indicato e lo fa puntare dal puntatore indicato
  void setPiece(Name type, Color color, Point p);
  // Metodo per accedere all'array di array
  void clearPieceAt(Point x);
  // crea i pezzi e li assegna ai puntatori iniziali insieme alle immagini
  void setPieces();

  // Creazione dell'interfaccia
  void drawPieces();
  void drawBoard();

  // Movimento dei pezzi
  Piece* selectPiece(Point p);  // resistuisce un puntatore data la posizione
  void movePiece(Point from, Point to);  // muovo un pezzo

  // mosse speciali
  bool isPromotion(Point to, Point from);
  void promote(Point p_pawn, Name piece, Color color);
  bool isCastling(Point p_from, Point p_to);

  // controllo della scacchiera
  bool clearPath(Point from, Point to);
  bool clearOrizzontalPath(Point p_from, Point p_to);
  bool clearVerticalPath(Point r_from, Point r_to);
  bool clearDiagonalPath(Point p_from, Point p_to);
  Point kingPosition(Color color);
};

#endif