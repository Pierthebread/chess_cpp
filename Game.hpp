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
  int fifty_movescounter_;
  std::vector<Name> white_pieces_;
  std::vector<Name> black_pieces_;

  static inline void assertInRange_Game(Point p) {
    assert(p.c >= 0 && p.c < 8 && p.r >= 0 && p.r < 8);
  }
  
 public:
  Game(const std::string nameWhite, const std::string nameBlack,
       sf::RenderWindow&);

  // metodi per accedere alle variabili private
  Board& getBoard();  // non posso copiare unique_ptr
  Color getPlayerTurn();
  bool getGameOver();
  const Player& getPlayer(
      Color);  // non mi serve una copia, voglio solo leggere
  int getFiftyMovesCounter();

  // metodi per modificare le variabili private
  void setGameOver(bool p);
  void setPlayerWinner(const Player& player);
  void addMovesCounter();
  void resetMovesCounter();
  void switchTurn();

  // funzioni per il movimento pezzi (IMPLEMENTARE TEST)
  bool rightStarting(Point from);  // ho tolto i throw
  bool rightArrival(Point to);     // ho tolto i throw
  bool validMove(Point from, Point to, const Board& board);

  // funzioni per promozione (IMPLEMENTARE TEST)
  Name pieceToPromote();
  void executePromotion(Point from, Point to);

  // funzioni per arrocco (IMPLEMENTARE TEST)
  bool isCastling(Point to);
  bool isCastlingValid(Point from, Point to);
  void executeCastling(Point from, Point to);

  // funzioni per enPassant (IMPLEMENTARE TEST)
  void setEnPassantTarget(Point from,
                          Point to);  // pedone si muove di due caselle
  bool isEnPassantValid(Point from, Point to);  // questo enPassant si può fare?
  void executeEnPassant(Point from, Point to);  // esegue l'enPassant

  // funzioni per lo scacco
  bool isChecking(Point p, Color color, const Board& board);
  bool isCellAttached(Point p, Color color, const Board& board);
  bool isCheck(Color color, const Board& board);  // color è sotto scacco?
  bool createCheck(Point from, Point to);  // questa mossa genera uno scacco?

  // funzione definitiva
  void executeMove(Point from, Point to);
  void playMove(Point from, Point to);

  // conclusione partita
  bool canMove(Color color);      // ci sono mosse disponibili per color?
  bool isCheckmate(Color color);  // color è in scacco matto? (!color vince)
  bool isFiftyMoves();          // 50 mosse senza cattura o movimento di pedone?
  bool insufficientMaterial();  // il materiale è sufficiente?
  void checkGameOver();
};

#endif