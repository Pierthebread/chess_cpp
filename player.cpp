#include "player.hpp"
namespace chess {
Player::Player(const std::string& name, Color color)
    : name_(name), color_(color), moves_(0) {}

const std::string& Player::getName() { return name_; }

Color Player::getColor() const { return color_; }

int Player::getMoves() const { return moves_; }

void Player::incrementMoves() { ++moves_; }
}  // namespace chess