#ifndef GAME_HPP
#define GAME_HPP
#include "Board.hpp"
#include "Pieces.hpp"
#include "Player.hpp"

class Game {
 private:
  sf::RenderWindow& window_;
  Board board_;
  Player whitePlayer_;
  Player blackPlayer_;
  Color playerTurn_;
  Point enPassantTarget_;  // Point su casella di Enpassant
  bool gameOver_;

 public:
  Game(std::string nameWhite, std::string nameBlack, sf::RenderWindow& window);

  // metodi per accedere alle variabili private
  Board& getBoard();  // non posso copiare unique_ptr
  Color getPlayerTurn();
  bool getGameOver();
  Player getPlayer(Color color);

  // metodi per modificare le variabili private
  void setPlayerTurn(Color color);
  void setGameOver(bool p);
  void setPlayerWinner(Player player);

  // funzioni per il movimento pezzi (IMPLEMENTARE TEST)
  bool rightStarting(Point from);  // ho tolto i throw
  bool rightArrival(Point to);     // ho tolto i throw
  bool validMove(Point from, Point to, Board& board);
  Name pieceToPromote();

  // funzioni per arrocco (IMPLEMENTARE TEST)
  bool isCastling(Point to);
  bool isCastlingValid(Point from, Point to);
  void executeCastling(Point from, Point to);

  // funzioni per enPassant (IMPLEMENTARE TEST)
  void setEnPassantTarget(Point from,
                          Point to);  // pedone si muove di due caselle
  bool isEnPassantValid(Point from, Point to);    // questo enPassant si può fare?
  void executeEnPassant(Point from, Point to);  // esegue l'enPassant

  // funzioni per lo scacco
  bool isChecking(Point p, Color color, Board& board);
  bool isCheck(Color color, Board& board);  // color è sotto scacco?
  bool createCheck(Point from, Point to);   // questa mossa genera uno scacco?

  // funzione definitiva
  void switchTurn();
  void executeMove(Point from, Point to);
  void playMove(Point from, Point to);

  // conclusione partita
  bool canMove(Color color);      // ci sono mosse disponibili per color?
  bool isCheckmate(Color color);  // color è in scacco matto? (!color vince)
  bool isStalemate();             // è patta?
  bool insufficientMaterial();    // il materiale è sufficiente?
  void checkGameOver();

};

#endif