#include "game.hpp"

int main() {
  try {
    /*
    std::string name1;
    std::string name2;

    std::cout << "Player 1 name: " << '\n';
    std::getline(std::cin, name1);
    Player whitePlayer(name1, Color::White);

    std::cout << "Player 2 name: " << '\n';
    std::getline(std::cin, name2);
    Player blackPlayer(name2, Color::Black);

    std::cout << name1 << " is white" << '\n';
    std::cout << name2 << " is black" << '\n';
    */

    sf::RenderWindow window(sf::VideoMode(640, 640), "Chess" );
    window.setFramerateLimit(60);

    Game game("name1", "name2", window);
    Board& board = game.getBoard();
    board.setPieces();

    bool firstClick{true};
    Point starting_cell;
    Point arrival_cell;

    while (window.isOpen()) {
      sf::Event event;

      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Resized) {
        window.setSize(sf::Vector2u(640, 640));
    }
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
          } else {
            if (game.rightArrival({c, r}) && starting_cell != Point{c, r}) {
              arrival_cell = {c, r};
              game.executeMove(starting_cell, arrival_cell);
              game.checkGameOver();
              firstClick = true;
              if (game.getGameOver()) {
                window.close();
              }
            } else {
              firstClick = true;
            }
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
