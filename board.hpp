#ifndef BOARD_HPP
#define BOARD_HPP

#include <array>
#include <memory>

#include "pieces.hpp"
#include "player.hpp"
namespace chess {
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
    (void)p;  // evito warning in build Release
  }

 public:
  Board(sf::RenderWindow& window);
  Board cloneBoard(const Board& other_board) const;

  void setPiece(Name, Color, Point);
  void setPieces();
  void deletePiece(Point);

  void loadTextures();
  void drawPieces();
  void drawBoard();

  Point getKingPosition(Color) const;
  void setKingPosition(Color, Point);

  Piece* selectPiece(Point) const;
  void movePiece(Point from, Point to);

  bool isPromotion(Point to, Point from) const;
  void promote(Point p_pawn, Name piece, Color color);
  bool isCastling(Point from, Point to) const;

  bool clearPath(Point from, Point to) const;
  bool clearHorizontalPath(Point from, Point to) const;
  bool clearVerticalPath(Point from, Point to) const;
  bool clearDiagonalPath(Point from, Point to) const;
};
}  // namespace chess
#endif