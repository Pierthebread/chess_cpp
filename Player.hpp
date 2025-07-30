#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Pieces.hpp"

class Player {
 private:
  std::string name_;
  Color color_;
  int moves_;

 public:
  // costruttore
  Player(std::string name, Color color);

  // metodi per accedere alle variabili private
  const std::string getName();
  const Color getColor();
  const int getMoves();

  // metodi per modificare le variabili private
  void incrementMoves();
};

#endif