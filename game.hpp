#ifndef GAME_HPP
#define GAME_HPP
#include "board.hpp"
namespace chess {
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
    (void)p;  // evito warning in build Release
  }

 public:
  Game(const std::string nameWhite, const std::string nameBlack,
       sf::RenderWindow&);

  Color getPlayerTurn() const;
  bool getGameOver() const;
  const Player& getPlayer(Color) const;
  int getFiftyMovesCounter() const;
  Point getEnpassantTarget() const;

  Board& getBoard();

  void setGameOver(bool p);
  void setPlayerWinner(const Player& player);
  void addMovesCounter();
  void resetMovesCounter();
  void switchTurn();

  // funzioni per il movimento pezzi
  bool rightStarting(Point from) const;
  bool rightArrival(Point to) const;
  bool isChecking(Point p, Color color, const Board& board) const;
  bool isCellAttached(Point p, Color color, const Board& board) const;
  bool isCheck(Color color, const Board& board) const;  // color è sotto scacco?
  bool createCheck(Point from,
                   Point to) const;  // questa mossa genera uno scacco?
  bool validMove(Point from, Point to, const Board& board) const;

  // funzioni per promozione
  Name pieceToPromote();
  void executePromotion(Point from, Point to);

  // funzioni per arrocco
  bool isCastlingValid(Point from, Point to) const;
  void executeCastling(Point from, Point to);

  // funzioni per enPassant
  void setEnPassantTarget(Point from,
                          Point to);  // pedone si muove di due caselle
  bool isEnPassantValid(Point from,
                        Point to) const;        // questo enPassant si può fare?
  void executeEnPassant(Point from, Point to);  // esegue l'enPassant

  void executeMove(Point from, Point to);

  // conclusione partita
  bool canMove(Color color) const;      // ci sono mosse disponibili per color?
  bool isCheckmate(Color color) const;  // color è in scacco matto?
  bool isFiftyMoves() const;  // 50 mosse senza cattura o movimento di pedone?
  bool insufficientMaterial() const;  // il materiale è sufficiente?
  void checkGameOver();
};
}  // namespace chess

#endif