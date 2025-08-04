#include "Game.hpp"

int main() {
  try {
    sf::RenderWindow window(sf::VideoMode(640, 640), "Board");
    sf::Texture texture;
    Game game("name1", "name2", window);
    Board& board = game.getBoard();
    board.setPieces();

    sf::Sprite sprite;
    sprite.setTexture(texture);

    bool firstClick{true};
    Point starting_cell;
    Point arrival_cell;

    while (window.isOpen()) {
      sf::Event event;

      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left) {
          int c = event.mouseButton.x / 80;
          int r = event.mouseButton.y / 80;
          if (firstClick) {
            if (game.rightStarting({c, r})) {
              starting_cell = {c, r};
              firstClick = false;
            }
          } else  {
            arrival_cell = {c, r};
            game.playMove(starting_cell, arrival_cell);
            firstClick = true;
          }
        }
      }

      window.clear();
      board.drawBoard();
      board.drawPieces();
      window.display();
    }
  } catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << '\n';
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "Unknown exception occurred!" << '\n';
    return EXIT_FAILURE;
  }
};
