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
  // costruttore
  Player(const std::string& name, Color color);

  // metodi per accedere alle variabili private
  const std::string& getName();
  Color getColor();
  int getMoves();

  // metodi per modificare le variabili private
  void incrementMoves();
};
}  // namespace chess
#endif