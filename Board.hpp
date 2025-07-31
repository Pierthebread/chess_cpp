#ifndef SCACCHIERA_HPP
#define SCACCHIERA_HPP

#include <array>
#include <memory>

#include "Pieces.hpp"

enum castleType { Long, Short };

class Board {
  std::array<std::array<std::unique_ptr<Piece>, 8>, 8> board;
  
  std::string pieceType_[8][8];
  sf::RenderWindow& window_;

 public:
  // Costruttore
  Board(sf::RenderWindow& window);

  // Crea il pezzo indicato e lo fa puntare dal puntatore indicato
  void setPiece(Name type, Color color, Point p);

  // crea i pezzi e li assegna ai puntatori iniziali insieme alle immagini
  void setPieces();

  // Creazione dell'interfaccia
  void draw(sf::RenderWindow& window);
  void renderBoard(sf::RenderWindow& window);  // la dimensione della finestra
                                               // (si mette nel main)

  // Movimento dei pezzi
  Piece* selectPiece(Point p);  // resistuisce un puntatore data la posizione
  void movePiece(Point from, Point to);  // muovo un pezzo

  // controllo della scacchiera
  bool clearPath(Point from, Point to);
  bool clearOrizzontalPath(Point p_from, Point p_to);
  bool clearVerticalPath(Point r_from, Point r_to);
  bool clearDiagonalPath(Point p_from, Point p_to);
  Point kingPosition(Color color);

  // per la promozione
  bool isPromotion(Point to, Point from);
  void promote(Point p_pawn, Name piece);
};

#endif