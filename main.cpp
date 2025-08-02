#include "Board.hpp"
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
        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left &&
            game.rightStarting({static_cast<Column>(event.mouseButton.x / 80),
                                event.mouseButton.y / 80}) &&
            firstClick == true) {
          starting_cell = {static_cast<Column>(event.mouseButton.x / 80),
                           event.mouseButton.y / 80};
          firstClick = false;
        } else if (event.type == sf::Event::MouseButtonPressed &&
                   event.mouseButton.button == sf::Mouse::Left &&
                   firstClick == false) {
          arrival_cell = {static_cast<Column>(event.mouseButton.x / 80),
                          event.mouseButton.y / 80};
        }
        game.playMove(starting_cell, arrival_cell);
        firstClick = true;
      }
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      window.clear();
      board.drawBoard(window);
      board.drawPieces(window);
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
