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

  // funzioni per il movimento pezzi (DA IMPLEMENTARE I TEST)
  bool rightStarting(Point from);  // ho tolto i throw
  bool rightArrival(Point to);     // ho tolto i throw
  bool validMove(Point from, Point to);

  // controllo della scacchiera
  bool clearPath(Point from, Point to);
  bool clearOrizzontalPath(Point p_from, Point p_to);
  bool clearVerticalPath(Point r_from, Point r_to);
  bool clearDiagonalPath(Point p_from, Point p_to);
  Point kingPosition(Color color);

  // per la promozione
  bool isPromotion(Point to, Point from);
  void promote(Point p_pawn, Name piece, Color color);
};

#endif