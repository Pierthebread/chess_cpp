#include "Player.hpp"

Player::Player(std::string name, Color color)
    : name_(name), color_(color), moves_(0) {}

const std::string Player::getName() { return name_; }

const Color Player::getColor() { return color_; }

const int Player::getMoves() { return moves_; }

void Player::incrementMoves() { ++moves_; }