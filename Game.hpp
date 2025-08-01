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
  bool gameOver_;

 public:
  Game(std::string nameWhite, std::string nameBlack, sf::RenderWindow& window);

  // metodi per accedere alle variabili private
  Board& getBoard();  // non posso copiare unique_ptr
  Color getPlayerTurn();
  bool getGameOver();

  // metodi per modificare le variabili private
  void setPlayerTurn(Color color);
  void setGameOver(bool p);

  // funzioni per il movimento pezzi (DA IMPLEMENTARE I TEST)
  bool rightStarting(Point from);  // ho tolto i throw
  bool rightArrival(Point to);     // ho tolto i throw
  bool validMove(Point from, Point to);

  // funzioni per arrocco
  bool isCastlingValid(Point from, Point to);
  void executeCastling(Point from, Point to);
  
  // funzioni per enPassant

  // funzioni per lo scacco (DA FINIRE + IMPLEMENTARE TEST)
  bool isCheck(Color color);  // color è sotto scacco?
  bool isChecking(Point pezzoipotetico,
                  Color color);  // pezzoipotetico punta al re di color?
  bool createCheck(Point from, Point to);  // questa mossa genera uno scacco?

  // funzione definitiva
  void playMove(Point from, Point to);
};

#endif