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
  Point enPassantTarget_;  // Point on enPassant cell
  bool gameOver_;
  int fifty_movescounter_;
  std::vector<Name> white_pieces_;
  std::vector<Name> black_pieces_;

 public:
  Game(const std::string nameWhite, const std::string nameBlack,
       sf::RenderWindow&);

  bool getGameOver() const noexcept;
  Color getPlayerTurn() const noexcept;

  Board& getBoard();

  void switchTurn();

  // functions for moving pieces
  bool rightStarting(Point from) const;
  bool rightArrival(Point to) const;
  bool createCheck(Point from, Point to) const;
  bool validMove(Point from, Point to) const;

  // promotion functions
  Name pieceToPromote();
  void executePromotion(Point from, Point to);

  // castling functions
  bool isCastlingValid(Point from, Point to) const;
  void executeCastling(Point from, Point to);

  // enPassant functions
  void setEnPassantTarget(Point from,
                          Point to);  // when the pawn moves two squares
  bool isEnPassantValid(Point from, Point to) const;
  void executeEnPassant(Point from, Point to);

  void executeMove(Point from, Point to);
  void checkGameOver();

 private:
  const Player& getPlayer(Color) const noexcept;
  int getFiftyMovesCounter() const noexcept;
  Point getEnpassantTarget() const noexcept;

  void setGameOver(bool p);
  void addMovesCounter();
  void resetMovesCounter();

  // functions for moving pieces
  bool isChecking(Point p, Color color, const Board& board) const;
  bool isCellAttached(Point p, Color color, const Board& board) const;
  bool isCheck(Color color, const Board& board) const;  // color è sotto scacco?

  // endgame
  bool canMove(Color color) const;  // are there moves available per color?
  bool isFiftyMoves() const;        // 50 moves without a capture or pawn move?
  bool insufficientMaterial() const;  // is there enough material?
};
}  // namespace chess

#endif