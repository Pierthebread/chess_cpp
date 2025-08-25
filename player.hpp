#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "pieces.hpp"
namespace chess {
class Player {
 private:
  std::string name_;
  Color color_;
  int moves_;

 public:
  Player(const std::string& name, Color color);

  const std::string& getName() const noexcept;
  Color getColor() const noexcept;
  int getMoves() const noexcept;

  void incrementMoves();
};
}  // namespace chess
#endif