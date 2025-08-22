#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "game.hpp"
using namespace chess;
// Piece.hpp && Piece.cpp
TEST_CASE("Testing validPieceMove") {
  Point A{0, 0};
  Point B{3, 3};
  Point C{3, 4};
  Point D{4, 6};
  Point E{5, 5};
  Point F{5, 2};
  SUBCASE("Testing King") {
    King piece(Black);
    CHECK(piece.validPieceMove(A, B) == false);
    CHECK(piece.validPieceMove(B, C) == true);
    CHECK(piece.validPieceMove(C, D) == false);
    CHECK(piece.validPieceMove(D, E) == true);
    CHECK(piece.validPieceMove(E, F) == false);
  }
  SUBCASE("Testing Knight") {
    Knight piece(Black);
    CHECK(piece.validPieceMove(A, B) == false);
    CHECK(piece.validPieceMove(B, C) == false);
    CHECK(piece.validPieceMove(C, D) == true);
    CHECK(piece.validPieceMove(D, E) == false);
    CHECK(piece.validPieceMove(E, F) == false);
  }
  SUBCASE("Testing Rook") {
    Rook piece(Black);
    CHECK(piece.validPieceMove(A, B) == false);
    CHECK(piece.validPieceMove(B, C) == true);
    CHECK(piece.validPieceMove(C, D) == false);
    CHECK(piece.validPieceMove(D, E) == false);
    CHECK(piece.validPieceMove(E, F) == true);
  }
  SUBCASE("Testing Queen") {
    Queen piece(Black);
    CHECK(piece.validPieceMove(A, B) == true);
    CHECK(piece.validPieceMove(B, C) == true);
    CHECK(piece.validPieceMove(C, D) == false);
    CHECK(piece.validPieceMove(D, E) == true);
    CHECK(piece.validPieceMove(E, F) == true);
  }
  SUBCASE("Testing Bishop") {
    Bishop piece(Black);
    CHECK(piece.validPieceMove(A, B) == true);
    CHECK(piece.validPieceMove(B, C) == false);
    CHECK(piece.validPieceMove(C, D) == false);
    CHECK(piece.validPieceMove(D, E) == true);
    CHECK(piece.validPieceMove(E, F) == false);
  }
  SUBCASE("Testing Pawn") {
    Pawn piece(Black);
    CHECK(piece.validPieceMove(A, B) == false);
    CHECK(piece.validPieceMove(B, C) == true);
    CHECK(piece.validPieceMove(C, D) == false);
    CHECK(piece.validPieceMove(D, E) == false);
    CHECK(piece.validPieceMove(E, F) == false);
  }
}

// Test Board.hpp && Board.cpp
TEST_CASE("Testing board") {
  sf::RenderWindow window;
  Board board(window);
  SUBCASE("Testing setPieces") {
    board.setPieces();
    std::array<Name, 8> firstRow = {rook, knight, bishop, queen,
                                    king, bishop, knight, rook};
    for (int i{0}; i < 8; ++i) {
      CHECK(board.selectPiece({i, 0})->getName() ==
            firstRow[static_cast<std::size_t>(i)]);
      CHECK(board.selectPiece({i, 0})->getColor() == Black);
      CHECK(board.selectPiece({i, 1})->getName() == pawn);
      CHECK(board.selectPiece({i, 1})->getColor() == Black);
    }
    for (int i{0}; i < 8; ++i) {
      CHECK(board.selectPiece({i, 7})->getName() ==
            firstRow[static_cast<std::size_t>(i)]);
      CHECK(board.selectPiece({i, 7})->getColor() == White);
      CHECK(board.selectPiece({i, 6})->getName() == pawn);
      CHECK(board.selectPiece({i, 6})->getColor() == White);
    }
  }
  SUBCASE("Testing deletePiece") {
    board.setPieces();
    board.deletePiece({0, 0});
    board.deletePiece({3, 3});
    board.deletePiece({1, 0});
    board.deletePiece({6, 7});
    CHECK(board.selectPiece({0, 0}) == nullptr);
    CHECK(board.selectPiece({3, 3}) == nullptr);
    CHECK(board.selectPiece({1, 0}) == nullptr);
    CHECK(board.selectPiece({6, 7}) == nullptr);
  }
}

TEST_CASE("Testing movePiece") {
  sf::RenderWindow window;
  Board board(window);
  board.setPieces();

  board.movePiece({0, 0}, {3, 3});
  CHECK(board.selectPiece({3, 3})->getName() == rook);
  CHECK(board.selectPiece({3, 3})->getColor() == Black);
  CHECK(board.selectPiece({3, 3})->getMoved() == true);

  board.movePiece({7, 0}, {7, 7});
  CHECK(board.selectPiece({7, 7})->getName() == rook);
  CHECK(board.selectPiece({7, 7})->getColor() == Black);
  CHECK(board.selectPiece({7, 7})->getMoved() == true);

  board.movePiece({3, 7}, {1, 0});
  CHECK(board.selectPiece({1, 0})->getColor() == White);
  CHECK(board.selectPiece({1, 0})->getName() == queen);

  CHECK_THROWS_AS(board.movePiece({0, 4}, {0, 6}), std::runtime_error);
  CHECK_THROWS_AS(board.movePiece({3, 4}, {6, 6}), std::runtime_error);
};

TEST_CASE("Testing clearPath") {
  sf::RenderWindow window;
  Board board(window);
  SUBCASE("Testing clearHorizontalPath") {
    board.setPiece(rook, White, {0, 0});
    board.setPiece(pawn, Black, {3, 0});

    CHECK(board.clearHorizontalPath({0, 1}, {3, 1}) == true);
    CHECK(board.clearHorizontalPath({0, 0}, {5, 0}) == false);
    CHECK(board.clearHorizontalPath({4, 0}, {0, 0}) == false);
  }

  SUBCASE("Testing clearVerticalPath") {
    board.setPiece(rook, White, {0, 0});
    board.setPiece(pawn, Black, {0, 3});

    CHECK(board.clearVerticalPath({1, 0}, {1, 3}) == true);
    CHECK(board.clearVerticalPath({0, 0}, {0, 5}) == false);
    CHECK(board.clearVerticalPath({0, 5}, {0, 0}) == false);
  }

  // SUBCASE per percorsi diagonali
  SUBCASE("Testing clearDiagonalPath") {
    board.setPiece(bishop, White, {2, 2});
    board.setPiece(pawn, Black, {4, 4});

    CHECK(board.clearDiagonalPath({0, 0}, {2, 2}) == true);
    CHECK(board.clearDiagonalPath({2, 2}, {5, 5}) == false);
    CHECK(board.clearDiagonalPath({4, 4}, {1, 1}) == false);
  }

  SUBCASE("Testing clearPath") {
    // Horizontal
    board.setPiece(rook, White, {1, 0});
    CHECK(board.clearPath({0, 0}, {7, 0}) == false);
    board.deletePiece({1, 0});
    CHECK(board.clearPath({0, 0}, {7, 0}) == true);

    // Vertical
    board.setPiece(rook, White, {3, 3});
    CHECK(board.clearPath({3, 0}, {3, 7}) == false);
    board.deletePiece({3, 3});
    CHECK(board.clearPath({3, 0}, {3, 7}) == true);

    // Diagonal
    board.setPiece(bishop, White, {2, 2});
    CHECK(board.clearPath({0, 0}, {4, 4}) == false);
    board.deletePiece({2, 2});
    CHECK(board.clearPath({0, 0}, {4, 4}) == true);
  }

  // more testing
  board.setPiece(king, White, {0, 2});
  board.setPiece(pawn, Black, {1, 6});
  board.setPiece(bishop, White, {2, 3});

  SUBCASE("Testing clearVerticalPath false") {
    CHECK(board.clearVerticalPath({1, 5}, {1, 7}) == false);
    CHECK(board.clearPath({1, 5}, {1, 7}) == false);
  }
  SUBCASE("Testing clearVerticalPath true") {
    CHECK(board.clearVerticalPath({1, 2}, {1, 5}) == true);
    CHECK(board.clearPath({1, 2}, {1, 5}) == true);
    CHECK(board.clearVerticalPath({1, 5}, {1, 6}) == true);
    CHECK(board.clearPath({1, 5}, {1, 6}) == true);
  }
  SUBCASE("Testing clearHorizontalPath false") {
    CHECK(board.clearHorizontalPath({0, 6}, {3, 6}) == false);
    CHECK(board.clearPath({0, 6}, {3, 6}) == false);
  }
  SUBCASE("Testing clearHorizontalPath true") {
    CHECK(board.clearHorizontalPath({1, 5}, {4, 5}) == true);
    CHECK(board.clearPath({1, 5}, {4, 5}) == true);
    CHECK(board.clearHorizontalPath({1, 6}, {3, 6}) == true);
    CHECK(board.clearPath({1, 6}, {3, 6}) == true);
  }
  SUBCASE("Testing clearDiagonalPath false") {
    CHECK(board.clearDiagonalPath({0, 5}, {2, 7}) == false);
    CHECK(board.clearPath({0, 5}, {2, 7}) == false);
  }
  SUBCASE("Testing clearDiagonalPath true") {
    CHECK(board.clearDiagonalPath({3, 3}, {5, 5}) == true);
    CHECK(board.clearPath({3, 3}, {5, 5}) == true);
    CHECK(board.clearDiagonalPath({7, 7}, {4, 4}) == true);
    CHECK(board.clearPath({7, 7}, {4, 4}) == true);
  }
  SUBCASE("Testing clearPath with disconnected cell") {
    CHECK(board.clearPath({1, 2}, {0, 5}) == true);
    CHECK(board.clearPath({4, 2}, {1, 1}) == true);
  }
};

TEST_CASE("Testing getKingPosition") {
  sf::RenderWindow window;
  Board board(window);
  board.setPieces();

  SUBCASE("Testing firt position") {
    Point white_king_pos{4, 7};
    Point black_king_pos{4, 0};
    CHECK(board.getKingPosition(White) == white_king_pos);
    CHECK(board.getKingPosition(Black) == black_king_pos);
  }
  SUBCASE("Testing getKingPosition after king has moved") {
    board.movePiece({4, 7}, {5, 1});
    board.movePiece({4, 0}, {6, 6});
    Point white_king_pos{5, 1};
    Point black_king_pos{6, 6};

    CHECK(board.getKingPosition(White) == white_king_pos);
    CHECK(board.getKingPosition(Black) == black_king_pos);
  }
};

TEST_CASE("Testing isPromotion and Promote") {
  sf::RenderWindow window;
  Board board(window);
  board.setPiece(pawn, Black, {0, 6});
  board.setPiece(pawn, White, {0, 1});
  board.setPiece(queen, White, {4, 4});

  SUBCASE("Testing isPromotion") {
    CHECK(board.isPromotion({0, 1}, {0, 0}) == true);
    CHECK(board.isPromotion({0, 1}, {0, 2}) == false);
    CHECK(board.isPromotion({0, 6}, {0, 7}) == true);
    CHECK(board.isPromotion({0, 6}, {0, 5}) == false);
    CHECK(board.isPromotion({0, 4}, {0, 5}) == false);
    CHECK(board.isPromotion({0, 6}, {0, 0}) == false);
    CHECK(board.isPromotion({0, 1}, {0, 7}) == false);
    CHECK(board.isPromotion({4, 4}, {0, 0}) == false);
  }
  SUBCASE("Testing promote") {
    board.movePiece({0, 1}, {0, 0});  // white
    board.promote({0, 0}, rook, White);
    board.movePiece({0, 6}, {0, 7});  // black
    board.promote({0, 7}, queen, Black);
    CHECK(board.selectPiece({0, 0})->getName() == rook);
    CHECK(board.selectPiece({0, 0})->getColor() == White);
    CHECK(board.selectPiece({0, 7})->getName() == queen);
    CHECK(board.selectPiece({0, 7})->getColor() == Black);
    CHECK(board.selectPiece({0, 1}) == nullptr);
  }
};

TEST_CASE("Testing isCastling") {
  sf::RenderWindow window;
  Board board(window);
  board.setPiece(king, White, {4, 7});
  board.setPiece(rook, Black, {4, 0});

  SUBCASE("Testing isCastling true") {
    CHECK(board.isCastling({4, 7}, {2, 7}) == true);
    CHECK(board.isCastling({4, 7}, {6, 7}) == true);
    CHECK(board.isCastling({4, 0}, {2, 0}) == true);
    CHECK(board.isCastling({4, 0}, {6, 0}) == true);
  }
  SUBCASE("Testing isCastling false") {
    CHECK(board.isCastling({4, 7}, {5, 7}) == false);
    CHECK(board.isCastling({4, 0}, {3, 7}) == false);
    CHECK(board.isCastling({5, 7}, {3, 7}) == false);
  }
  SUBCASE("Testing isCastling with king->getMoved() true") {
    board.movePiece({4, 7}, {4, 6});
    board.movePiece({4, 0}, {5, 0});
    CHECK(board.isCastling({4, 6}, {2, 7}) == false);
    CHECK(board.isCastling({5, 0}, {7, 0}) == false);
  }
};

// test Game
TEST_CASE("Testing  rightStarting") {
  sf::RenderWindow window;
  Game game(std::string("White"), std::string("Black"), window);
  game.getBoard().setPiece(rook, White, {0, 0});
  game.getBoard().setPiece(queen, Black, {1, 7});

  SUBCASE("Testing rightStarting") {
    CHECK(game.getPlayerTurn() == White);
    CHECK(game.rightStarting({0, 0}) == true);
    CHECK(game.rightStarting({1, 7}) == false);
    CHECK(game.rightStarting({2, 7}) == false);

    game.switchTurn();

    CHECK(game.getPlayerTurn() == Black);
    CHECK(game.rightStarting({1, 7}) == true);
    CHECK(game.rightStarting({0, 0}) == false);
    CHECK(game.rightStarting({2, 7}) == false);
  }
}

TEST_CASE("Testing  rightArrival") {
  sf::RenderWindow window;
  Game game("white", "black", window);
  game.getBoard().setPiece(rook, White, {0, 0});
  game.getBoard().setPiece(queen, Black, {1, 7});

  SUBCASE("Testing rightArrival true ") {
    CHECK(game.rightArrival({0, 5}) == true);
    CHECK(game.rightArrival({1, 7}) == true);
    CHECK(game.rightArrival({0, 0}) == false);
    game.switchTurn();
    CHECK(game.getPlayerTurn() == Black);
    CHECK(game.rightArrival({0, 0}) == true);
    CHECK(game.rightArrival({1, 4}) == true);
    CHECK(game.rightArrival({1, 7}) == false);
  }
}

TEST_CASE("Testing createCheck") {
  sf::RenderWindow window;
  Game game("white", "black", window);
  game.getBoard().setPiece(king, White, {3, 3});

  SUBCASE("Testing createCheck I") {
    game.getBoard().setPiece(rook, Black, {7, 3});
    CHECK(game.createCheck({3, 3}, {4, 3}) == true);
    CHECK(game.createCheck({3, 3}, {2, 3}) == true);
    CHECK(game.createCheck({3, 3}, {3, 4}) == false);
    game.createCheck({3, 3}, {3, 4});
    CHECK(game.getBoard().selectPiece({3, 3})->getName() == king);
  }
  SUBCASE("Testing createCheck II") {
    game.getBoard().setPiece(rook, Black, {7, 3});
    game.getBoard().setPiece(rook, White, {4, 3});
    CHECK(game.createCheck({4, 3}, {5, 3}) == false);
    CHECK(game.createCheck({4, 3}, {7, 3}) == false);
    CHECK(game.createCheck({4, 3}, {4, 2}) == true);
  }
  SUBCASE("Testing createCheck III") {
    game.getBoard().setPiece(rook, Black, {7, 3});
    game.getBoard().setPiece(pawn, White, {4, 3});
    CHECK(game.createCheck({4, 3}, {4, 4}) == true);
    CHECK(game.createCheck({4, 3}, {4, 5}) == true);
    CHECK(game.createCheck({4, 3}, {5, 4}) == true);
  }
  SUBCASE("Testing createCheck IV") {
    game.getBoard().setPiece(queen, Black, {3, 6});
    game.getBoard().setPiece(pawn, White, {3, 4});
    CHECK(game.createCheck({3, 4}, {3, 5}) == false);
    CHECK(game.createCheck({3, 4}, {3, 6}) == false);
    CHECK(game.createCheck({3, 4}, {3, 2}) == true);
  }
  SUBCASE("Testing createCheck V") {
    game.getBoard().setPiece(rook, White, {0, 1});
    CHECK(game.createCheck({0, 1}, {0, 2}) == false);
    CHECK(game.createCheck({0, 1}, {0, 3}) == false);
  }
  SUBCASE("Testing createCheck VI") {
    game.getBoard().setPiece(bishop, White, {2, 2});
    game.getBoard().setPiece(bishop, Black, {5, 5});
    CHECK(game.createCheck({2, 2}, {3, 1}) == true);
    CHECK(game.createCheck({2, 2}, {4, 4}) == false);
  }
  SUBCASE("Testing createCheck VII") {
    game.getBoard().setPiece(knight, White, {1, 1});
    game.getBoard().setPiece(bishop, Black, {7, 7});
    CHECK(game.createCheck({1, 1}, {2, 3}) == true);
    CHECK(game.createCheck({1, 1}, {0, 3}) == true);
  }
  SUBCASE("Testing createCheck VIII") {
    game.getBoard().setPiece(pawn, White, {3, 4});
    game.getBoard().setPiece(queen, Black, {3, 6});
    game.getBoard().setPiece(rook, Black, {7, 7});
    CHECK(game.createCheck({3, 4}, {3, 5}) == false);
    CHECK(game.createCheck({3, 4}, {2, 4}) == true);
  }
  SUBCASE("Testing createCheck IX") {
    game.getBoard().setPiece(pawn, White, {4, 4});
    CHECK(game.createCheck({4, 4}, {4, 4}) == false);
  }
}

TEST_CASE("Game::validMove - General Rules") {
  sf::RenderWindow window;

  SUBCASE("Empty starting cell → false") {
    Game game("white", "black", window);
    CHECK(game.validMove({4, 2}, {4, 4}, game.getBoard()) == false);
  }

  SUBCASE("Move to cell occupied by same color → false") {
    Game game("white", "black", window);
    game.getBoard().setPiece(pawn, White, {4, 2});
    game.getBoard().setPiece(pawn, White, {4, 4});
    CHECK(game.validMove({4, 2}, {4, 4}, game.getBoard()) == false);
  }

  SUBCASE("Invalid move for piece type → false") {
    Game game("white", "black", window);
    game.getBoard().setPiece(rook, White, {4, 2});
    CHECK(game.validMove({4, 2}, {5, 3}, game.getBoard()) == false);
  }

  SUBCASE("Blocked path → false") {
    Game game("white", "black", window);
    game.getBoard().setPiece(rook, White, {4, 2});
    game.getBoard().setPiece(pawn, White, {4, 3});
    CHECK(game.validMove({4, 2}, {4, 4}, game.getBoard()) == false);
  }

  SUBCASE("Move leaving king in check → false") {
    Game game("white", "black", window);
    game.getBoard().setPiece(king, White, {4, 1});
    game.getBoard().setPiece(rook, Black, {4, 4});
    game.getBoard().setPiece(pawn, White, {4, 2});
    CHECK(game.validMove({4, 2}, {4, 3}, game.getBoard()) == false);
  }

  SUBCASE("Valid move → true") {
    Game game("white", "black", window);
    game.getBoard().setPiece(king, White, {0, 0});
    game.getBoard().setPiece(rook, White, {4, 2});
    CHECK(game.validMove({4, 2}, {4, 3}, game.getBoard()) == true);
  }
}

TEST_CASE("Game::validMove - Pawn Specific Rules") {
  sf::RenderWindow window;
  Game game("white", "black", window);

  SUBCASE("Pawn cannot capture frontally → false") {
    game.getBoard().setPiece(pawn, White, {4, 2});
    game.getBoard().setPiece(pawn, Black, {4, 3});
    CHECK(game.validMove({4, 2}, {4, 3}, game.getBoard()) == false);
  }

  SUBCASE("Pawn valid single push → true") {
    game.getBoard().setPiece(king, White, {0, 0});
    game.getBoard().setPiece(pawn, White, {0, 6});
    CHECK(game.validMove({0, 6}, {0, 5}, game.getBoard()) == true);
  }

  SUBCASE("Pawn invalid backward move → false") {
    game.getBoard().setPiece(king, White, {0, 0});
    game.getBoard().setPiece(pawn, White, {0, 6});
    CHECK(game.validMove({0, 6}, {0, 7}, game.getBoard()) == false);
  }

  SUBCASE("Pawn valid diagonal capture → true") {
    game.getBoard().setPiece(king, White, {0, 0});
    game.getBoard().setPiece(pawn, White, {3, 4});
    game.getBoard().setPiece(pawn, Black, {4, 3});
    CHECK(game.validMove({3, 4}, {4, 3}, game.getBoard()) == true);
  }

  SUBCASE("Pawn invalid vertical capture → false") {
    game.getBoard().setPiece(pawn, White, {2, 2});
    game.getBoard().setPiece(pawn, Black, {2, 3});
    CHECK(game.validMove({2, 2}, {2, 3}, game.getBoard()) == false);
  }
}

TEST_CASE("Game::validMove - Check Scenarios") {
  sf::RenderWindow window;
  Game game("white", "black", window);
  game.getBoard().setPiece(king, White, {3, 3});

  SUBCASE("King cannot move into check") {
    game.getBoard().setPiece(rook, Black, {7, 3});
    CHECK(game.validMove({3, 3}, {4, 3}, game.getBoard()) == false);
    CHECK(game.validMove({3, 3}, {2, 3}, game.getBoard()) == false);
    CHECK(game.validMove({3, 3}, {3, 4}, game.getBoard()) == true);
  }

  SUBCASE("Piece blocks check → valid move") {
    game.getBoard().setPiece(rook, Black, {7, 3});
    game.getBoard().setPiece(rook, White, {4, 3});
    CHECK(game.validMove({4, 3}, {5, 3}, game.getBoard()) == true);
    CHECK(game.validMove({4, 3}, {7, 3}, game.getBoard()) == true);
  }

  SUBCASE("Moving pinned piece reveals check → invalid move") {
    game.getBoard().setPiece(queen, Black, {3, 6});
    game.getBoard().setPiece(pawn, White, {3, 5});
    CHECK(game.validMove({3, 5}, {3, 4}, game.getBoard()) == true);
    CHECK(game.validMove({3, 5}, {3, 6}, game.getBoard()) == false);
  }
}

TEST_CASE("Game::validMove - Edge Cases") {
  sf::RenderWindow window;

  SUBCASE("Moving to same position → false") {
    Game game("white", "black", window);
    game.getBoard().setPiece(pawn, White, {4, 4});
    CHECK(game.validMove({4, 4}, {4, 4}, game.getBoard()) == false);
  }

  SUBCASE("Turn validation - white's turn") {
    Game game("white", "black", window);
    game.getBoard().setPiece(king, White, {0, 0});
    game.getBoard().setPiece(pawn, White, {0, 6});
    game.getBoard().setPiece(pawn, Black, {0, 2});
    CHECK(game.validMove({0, 6}, {0, 5}, game.getBoard()) == true);
    CHECK(game.validMove({0, 2}, {0, 3}, game.getBoard()) == false);
  }

  SUBCASE("Turn validation - black's turn") {
    Game game("white", "black", window);
    game.getBoard().setPiece(king, Black, {6, 6});
    game.getBoard().setPiece(pawn, White, {0, 5});
    game.getBoard().setPiece(pawn, Black, {0, 2});
    game.switchTurn();
    CHECK(game.validMove({0, 2}, {0, 3}, game.getBoard()) == true);
    CHECK(game.validMove({0, 6}, {0, 5}, game.getBoard()) == false);
  }
}

TEST_CASE("Game::validMove - Complex Scenarios") {
  sf::RenderWindow window;

  SUBCASE("Scenario 1 - Pawn blocks bishop attack") {
    Game game("white", "black", window);
    game.getBoard().setPiece(king, White, {4, 0});
    game.getBoard().setPiece(pawn, White, {4, 4});
    game.getBoard().setPiece(bishop, White, {1, 5});
    game.getBoard().setPiece(pawn, Black, {3, 6});
    game.getBoard().setPiece(king, Black, {4, 7});
    game.switchTurn();
    CHECK(game.validMove({3, 6}, {3, 4}, game.getBoard()) == false);
  }

  SUBCASE("Scenario 2 - Knight movement with check") {
    Game game("white", "black", window);
    game.getBoard().setPiece(king, White, {3, 3});
    game.getBoard().setPiece(knight, White, {1, 1});
    game.getBoard().setPiece(bishop, Black, {7, 7});
    CHECK(game.validMove({1, 1}, {2, 3}, game.getBoard()) == false);
    CHECK(game.validMove({1, 1}, {0, 3}, game.getBoard()) == false);
  }
}

// Test per arrocco
TEST_CASE("Game::isCastlingValid - Valid Conditions") {
  sf::RenderWindow window;
  Game game("white", "black", window);

  SUBCASE("Kingside castling valid") {
    game.getBoard().setPiece(king, White, {4, 0});
    game.getBoard().setPiece(rook, White, {7, 0});
    CHECK(game.isCastlingValid({4, 0}, {6, 0}) == true);
  }

  SUBCASE("Queenside castling valid") {
    game.getBoard().setPiece(king, White, {4, 0});
    game.getBoard().setPiece(rook, White, {0, 0});
    CHECK(game.isCastlingValid({4, 0}, {2, 0}) == true);
  }
}

TEST_CASE("Game::isCastlingValid - Invalid Conditions") {
  sf::RenderWindow window;
  Game game("white", "black", window);
  game.getBoard().setPiece(king, White, {4, 0});

  SUBCASE("Rook has moved") {
    game.getBoard().setPiece(rook, White, {7, 0});
    game.getBoard().selectPiece({7, 0})->setMoved(true);
    CHECK(game.isCastlingValid({4, 0}, {6, 0}) == false);
  }

  SUBCASE("Path not clear") {
    game.getBoard().setPiece(rook, White, {7, 0});
    game.getBoard().setPiece(bishop, White, {5, 0});
    CHECK(game.isCastlingValid({4, 0}, {6, 0}) == false);
  }

  SUBCASE("King in check during castling") {
    game.getBoard().setPiece(rook, White, {7, 0});
    game.getBoard().setPiece(rook, Black, {4, 7});
    CHECK(game.isCastlingValid({4, 0}, {6, 0}) == false);
  }
}

TEST_CASE("Game::executeCastling") {
  sf::RenderWindow window;
  Game game("white", "black", window);

  SUBCASE("Kingside castling execution") {
    game.getBoard().setPiece(king, White, {4, 0});
    game.getBoard().setPiece(rook, White, {7, 0});
    game.executeCastling({4, 0}, {6, 0});
    CHECK(game.getBoard().selectPiece({6, 0})->getName() == king);
    CHECK(game.getBoard().selectPiece({5, 0})->getName() == rook);
  }

  SUBCASE("Queenside castling execution") {
    game.getBoard().setPiece(king, White, {4, 0});
    game.getBoard().setPiece(rook, White, {0, 0});
    game.executeCastling({4, 0}, {2, 0});
    CHECK(game.getBoard().selectPiece({2, 0})->getName() == king);
    CHECK(game.getBoard().selectPiece({3, 0})->getName() == rook);
  }
}

// Test per en passant
TEST_CASE("Game::setEnPassantTarget") {
  sf::RenderWindow window;
  Game game("white", "black", window);

  SUBCASE("Pawn double move sets en passant") {
    game.getBoard().setPiece(pawn, White, {1, 6});
    game.getBoard().movePiece({1, 6}, {1, 4});
    game.setEnPassantTarget({1, 6}, {1, 4});
    CHECK(game.getEnpassantTarget().c == 1);
    CHECK(game.getEnpassantTarget().r == 5);
  }

  SUBCASE("Non-pawn move resets en passant") {
    game.getBoard().setPiece(rook, White, {0, 0});
    game.getBoard().movePiece({0, 0}, {0, 5});
    game.setEnPassantTarget({0, 0}, {0, 5});
    CHECK(game.getEnpassantTarget().c == 8);
    CHECK(game.getEnpassantTarget().r == 8);
  }
}

TEST_CASE("Game::isEnPassantValid") {
  sf::RenderWindow window;
  Game game("white", "black", window);

  SUBCASE("Valid en passant capture") {
    game.getBoard().setPiece(pawn, White, {1, 6});
    game.getBoard().setPiece(pawn, Black, {2, 4});
    game.getBoard().movePiece({1, 6}, {1, 4});
    game.setEnPassantTarget({1, 6}, {1, 4});
    CHECK(game.isEnPassantValid({2, 4}, {1, 5}) == true);
  }

  SUBCASE("Invalid target position") {
    game.getBoard().setPiece(pawn, White, {1, 3});
    game.getBoard().movePiece({1, 3}, {3, 2});
    game.setEnPassantTarget({1, 3}, {3, 2});
    CHECK(game.isEnPassantValid({1, 3}, {2, 2}) == false);
  }
}

TEST_CASE("Game::executeEnPassant") {
  sf::RenderWindow window;
  Game game("white", "black", window);

  SUBCASE("En passant execution removes correct pawn") {
    game.getBoard().setPiece(pawn, White, {1, 3});
    game.getBoard().setPiece(pawn, Black, {2, 3});
    game.executeEnPassant({1, 3}, {2, 2});
    CHECK(game.getBoard().selectPiece({2, 3}) == nullptr);
    CHECK(game.getBoard().selectPiece({2, 2})->getName() == pawn);
  }
}

// Test per promotion
TEST_CASE("Game::pieceToPromote") {
  sf::RenderWindow window;
  Game game("white", "black", window);

  // Save old cin buffer
  std::streambuf* oldCin = std::cin.rdbuf();

  SUBCASE("Select queen") {
    std::stringstream input;
    input << "1\n";
    std::cin.rdbuf(input.rdbuf());
    CHECK(game.pieceToPromote() == queen);
  }

  SUBCASE("Select bishop") {
    std::stringstream input;
    input << "2\n";
    std::cin.rdbuf(input.rdbuf());
    CHECK(game.pieceToPromote() == bishop);
  }

  SUBCASE("Select knight") {
    std::stringstream input;
    input << "3\n";
    std::cin.rdbuf(input.rdbuf());
    CHECK(game.pieceToPromote() == knight);
  }

  SUBCASE("Select rook") {
    std::stringstream input;
    input << "4\n";
    std::cin.rdbuf(input.rdbuf());
    CHECK(game.pieceToPromote() == rook);
  }

  SUBCASE("Invalid input then valid") {
    std::stringstream input;
    input << "5\n0\n2\n";
    std::cin.rdbuf(input.rdbuf());
    CHECK(game.pieceToPromote() == bishop);
  }

  // Restore cin
  std::cin.rdbuf(oldCin);
}

TEST_CASE("Game::executePromotion") {
  sf::RenderWindow window;
  Game game("white", "black", window);

  // Save old cin buffer
  std::streambuf* oldCin = std::cin.rdbuf();

  SUBCASE("Promotion to queen") {
    std::stringstream input;
    input << "1\n";
    std::cin.rdbuf(input.rdbuf());

    game.getBoard().setPiece(pawn, White, {0, 1});
    game.executePromotion({0, 1}, {0, 0});
    CHECK(game.getBoard().selectPiece({0, 0})->getName() == queen);
    CHECK(game.getBoard().selectPiece({0, 1}) == nullptr);
  }

  SUBCASE("Promotion to rook") {
    std::stringstream input;
    input << "4\n";
    std::cin.rdbuf(input.rdbuf());

    game.getBoard().setPiece(pawn, Black, {1, 6});
    game.executePromotion({1, 6}, {1, 7});
    CHECK(game.getBoard().selectPiece({1, 7})->getName() == rook);
    CHECK(game.getBoard().selectPiece({1, 6}) == nullptr);
  }

  // Restore cin
  std::cin.rdbuf(oldCin);
}

// Test per executeMove
TEST_CASE("Game::executeMove - Normal Move") {
  sf::RenderWindow window;
  Game game("white", "black", window);

  SUBCASE("Normal pawn move") {
    game.getBoard().setPiece(king, White, {0, 0});
    game.getBoard().setPiece(pawn, White, {1, 6});
    game.executeMove({1, 6}, {1, 5});
    CHECK(game.getBoard().selectPiece({1, 5})->getName() == pawn);
    CHECK(game.getBoard().selectPiece({1, 6}) == nullptr);
    CHECK(game.getPlayerTurn() == Black);  // Turn should switch
  }

  SUBCASE("Capture move") {
    game.getBoard().setPiece(king, White, {0, 0});
    game.getBoard().setPiece(pawn, White, {1, 6});
    game.getBoard().setPiece(pawn, Black, {2, 5});
    game.executeMove({1, 6}, {2, 5});
    CHECK(game.getBoard().selectPiece({2, 5})->getName() == pawn);
    CHECK(game.getBoard().selectPiece({2, 5})->getColor() == White);
    CHECK(game.getBoard().selectPiece({1, 6}) == nullptr);
  }
}

TEST_CASE("Game::executeMove - Special Moves") {
  sf::RenderWindow window;
  Game game("white", "black", window);

  SUBCASE("Castling") {
    game.getBoard().setPiece(king, White, {4, 0});
    game.getBoard().setPiece(rook, White, {0, 0});
    game.executeMove({4, 0}, {2, 0});
    CHECK(game.getBoard().selectPiece({2, 0})->getName() == king);
    CHECK(game.getBoard().selectPiece({3, 0})->getName() == rook);
  }

  SUBCASE("En passant") {
    game.getBoard().setPiece(pawn, White, {1, 3});
    game.getBoard().setPiece(pawn, Black, {2, 3});
    game.setEnPassantTarget({2, 1},
                            {2, 3});  // Simulate previous double pawn move
    game.executeMove({1, 3}, {2, 2});
    CHECK(game.getBoard().selectPiece({2, 2})->getName() == pawn);
    CHECK(game.getBoard().selectPiece({2, 3}) ==
          nullptr);  // Captured pawn should be removed
  }
  // SUBCASE("Promotion") {
  //  // Redireziono l'input per la scelta della promozione (1 = regina)
  //  std::streambuf* oldCin = std::cin.rdbuf();
  //  std::stringstream input;
  //  input << "1\n";
  //  std::cin.rdbuf(input.rdbuf());
  //
  //  game.getBoard().setPiece(king, White, {6, 6});
  //  game.getBoard().setPiece(king, Black, {7, 7});
  //
  //  game.getBoard().setPiece(pawn, White, {0, 1});
  //
  //  // Muovo il pedone sulla prima traversa → promozione
  //  game.executeMove({0, 1}, {0, 0});
  //
  //  // Verifico che ci sia una regina nella nuova posizione
  //
  //  CHECK(game.getBoard().selectPiece({0, 0})->getName() == queen);
  //
  //  // Ripristino cin
  //  std::cin.rdbuf(oldCin);
  //}
}

TEST_CASE("Game::canMove") {
  sf::RenderWindow window;
  Game game("white", "black", window);

  SUBCASE("King can move") {
    game.getBoard().setPiece(king, White, {4, 4});
    CHECK(game.canMove(White) == true);
  }

  SUBCASE("Pinned piece cannot move") {
    game.getBoard().setPiece(king, White, {4, 0});
    game.getBoard().setPiece(rook, White, {4, 4});
    game.getBoard().setPiece(rook, Black, {4, 7});
    CHECK(game.canMove(White) == true);  // King can still move
  }

  SUBCASE("Checkmate - no moves") {
    game.getBoard().setPiece(king, White, {0, 0});
    game.getBoard().setPiece(rook, Black, {1, 7});
    game.getBoard().setPiece(rook, Black, {7, 1});
    CHECK(game.canMove(White) == false);
  }
}

TEST_CASE("Game::isCheckmate") {
  sf::RenderWindow window;
  Game game("white", "black", window);

  SUBCASE("Checkmate position") {
    game.getBoard().setPiece(king, White, {0, 0});
    game.getBoard().setPiece(rook, Black, {0, 7});
    game.getBoard().setPiece(rook, Black, {7, 0});
    game.getBoard().setPiece(bishop, Black, {7, 7});
    CHECK(game.isCheckmate(White) == true);
  }

  SUBCASE("Check but not mate") {
    game.getBoard().setPiece(king, White, {4, 0});
    game.getBoard().setPiece(rook, Black, {4, 7});
    game.getBoard().setPiece(knight, White, {3, 2});
    CHECK(game.isCheckmate(White) == false);
  }

  SUBCASE("Not in check") {
    game.getBoard().setPiece(king, White, {4, 0});
    game.getBoard().setPiece(king, Black, {4, 7});
    CHECK(game.isCheckmate(White) == false);
  }
}

TEST_CASE("Game::insufficientMaterial") {
  sf::RenderWindow window;
  Game game("white", "black", window);

  SUBCASE("Only kings") {
    game.getBoard().setPiece(king, White, {4, 0});
    game.getBoard().setPiece(king, Black, {4, 7});
    CHECK(game.insufficientMaterial() == true);
  }

  SUBCASE("King and knight vs king") {
    game.getBoard().setPiece(king, White, {4, 0});
    game.getBoard().setPiece(knight, White, {1, 1});
    game.getBoard().setPiece(king, Black, {4, 7});
    CHECK(game.insufficientMaterial() == true);
  }

  SUBCASE("King and bishop vs king") {
    game.getBoard().setPiece(king, White, {4, 0});
    game.getBoard().setPiece(bishop, White, {2, 2});
    game.getBoard().setPiece(king, Black, {4, 7});
    CHECK(game.insufficientMaterial() == true);
  }

  SUBCASE("King and bishop vs king and bishop (same color)") {
    game.getBoard().setPiece(king, White, {4, 0});
    game.getBoard().setPiece(bishop, White, {2, 2});  // Light square
    game.getBoard().setPiece(king, Black, {4, 7});
    game.getBoard().setPiece(bishop, Black, {3, 3});  // Light square
    CHECK(game.insufficientMaterial() == true);
  }

  SUBCASE("King and bishop vs king and bishop (different color)") {
    game.getBoard().setPiece(king, White, {4, 0});
    game.getBoard().setPiece(bishop, White, {2, 2});  // Light square
    game.getBoard().setPiece(king, Black, {4, 7});
    game.getBoard().setPiece(bishop, Black, {2, 3});  // Dark square
    CHECK(game.insufficientMaterial() == false);
  }

  SUBCASE("King and pawn vs king") {
    game.getBoard().setPiece(king, White, {4, 0});
    game.getBoard().setPiece(pawn, White, {4, 1});
    game.getBoard().setPiece(king, Black, {4, 7});
    CHECK(game.insufficientMaterial() == false);
  }
}

TEST_CASE("Game::checkGameOver") {
  sf::RenderWindow window;
  Game game("white", "black", window);

  SUBCASE("Checkmate") {
    game.getBoard().setPiece(king, White, {0, 0});
    game.getBoard().setPiece(rook, Black, {0, 7});
    game.getBoard().setPiece(rook, Black, {7, 0});
    game.getBoard().setPiece(bishop, Black, {7, 7});
    game.checkGameOver();
    CHECK(game.getGameOver() == true);
  }

  SUBCASE("Stalemate") {
    game.getBoard().setPiece(king, White, {0, 0});
    game.getBoard().setPiece(queen, Black, {1, 2});
    game.getBoard().setPiece(king, Black, {2, 2});
    game.checkGameOver();
    CHECK(game.getGameOver() == true);
  }

  SUBCASE("Insufficient material") {
    game.getBoard().setPiece(king, White, {4, 0});
    game.getBoard().setPiece(king, Black, {4, 7});
    game.checkGameOver();
    CHECK(game.getGameOver() == true);
  }

  SUBCASE("Fifty moves rule") {
    game.getBoard().setPiece(king, White, {0, 1});
    game.getBoard().setPiece(king, Black, {0, 0});
    for (int i = 0; i < 50; i++) {
      game.addMovesCounter();
    }
    game.checkGameOver();
    CHECK(game.getGameOver() == true);
  }
}

TEST_CASE("Game::isFiftyMoves") {
  sf::RenderWindow window;
  Game game("white", "black", window);

  SUBCASE("Exactly 50 moves") {
    for (int i = 0; i < 50; i++) {
      game.addMovesCounter();
    }
    CHECK(game.isFiftyMoves() == true);
  }

  SUBCASE("Less than 50 moves") {
    for (int i = 0; i < 49; i++) {
      game.addMovesCounter();
    }
    CHECK(game.isFiftyMoves() == false);
  }

  SUBCASE("Reset after capture") {
    for (int i{0}; i < 30; i++) {
      game.addMovesCounter();
    }
    game.resetMovesCounter();
    CHECK(game.getFiftyMovesCounter() == 0);
  }
}