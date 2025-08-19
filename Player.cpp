#include "Player.hpp"

Player::Player(const std::string& name, Color color)
    : name_(name), color_(color), moves_(0) {}

const std::string& Player::getName() { return name_; }

Color Player::getColor() { return color_; }

int Player::getMoves() { return moves_; }

void Player::incrementMoves() { ++moves_; }