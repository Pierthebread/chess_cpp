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

  const std::string& getName();
  Color getColor() const;
  int getMoves() const;

  void incrementMoves();
};
}  // namespace chess
#endif