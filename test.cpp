#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "Board.hpp"
#include "Game.hpp"
#include "doctest.h"

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
  SUBCASE("Testing clearPieceAt") {
    board.setPieces();
    board.clearPieceAt({0, 0});
    board.clearPieceAt({3, 3});
    board.clearPieceAt({1, 0});
    board.clearPieceAt({6, 7});
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
}

TEST_CASE("Testing clearPath") {
  sf::RenderWindow window;
  Board board(window);
  SUBCASE("Testing clearOrizzontalPath") {
    board.setPiece(rook, White, {0, 0});
    board.setPiece(pawn, Black, {3, 0});

    CHECK(board.clearOrizzontalPath({0, 1}, {3, 1}) == true);
    CHECK(board.clearOrizzontalPath({0, 0}, {5, 0}) == false);
    CHECK(board.clearOrizzontalPath({4, 0}, {0, 0}) == false);
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
    // Orizzontal
    board.setPiece(rook, White, {1, 0});
    CHECK(board.clearPath({0, 0}, {7, 0}) == false);
    board.clearPieceAt({1, 0});
    CHECK(board.clearPath({0, 0}, {7, 0}) == true);

    // Vertical
    board.setPiece(rook, White, {3, 3});
    CHECK(board.clearPath({3, 0}, {3, 7}) == false);
    board.clearPieceAt({3, 3});
    CHECK(board.clearPath({3, 0}, {3, 7}) == true);

    // Diagonal
    board.setPiece(bishop, White, {2, 2});
    CHECK(board.clearPath({0, 0}, {4, 4}) == false);
    board.clearPieceAt({2, 2});
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
  SUBCASE("Testing clearOrizzontalPath false") {
    CHECK(board.clearOrizzontalPath({A, 6}, {D, 6}) == false);
    CHECK(board.clearPath({A, 6}, {D, 6}) == false);
  }
  SUBCASE("Testing clearOrizzontalPath true") {
    CHECK(board.clearOrizzontalPath({B, 5}, {E, 5}) == true);
    CHECK(board.clearPath({B, 5}, {E, 5}) == true);
    CHECK(board.clearOrizzontalPath({B, 6}, {D, 6}) == true);
    CHECK(board.clearPath({B, 6}, {D, 6}) == true);
  }
  SUBCASE("Testing clearDiagonalPath false") {
    CHECK(board.clearDiagonalPath({0, 5}, {2, 7}) == false);
    CHECK(board.clearPath({0, 5}, {2, 7}) == false);
  }
  SUBCASE("Testing clearDiagonalPath false") {
    CHECK(board.clearDiagonalPath({1, 7}, {6, 0}) == true);
    CHECK(board.clearPath({1, 7}, {6, 0}) == true);
    CHECK(board.clearDiagonalPath({0, 5}, {1, 6}) == true);
    CHECK(board.clearPath({0, 5}, {1, 6}) == true);
  }
  SUBCASE("Testing clearPath with disconnected cell") {
    CHECK(board.clearPath({1, 2}, {0, 5}) == true);
    CHECK(board.clearPath({4, 2}, {1, 1}) == true);
  }
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
  SUBCASE("Testing clearOrizzontalPath false") {
    CHECK(board.clearOrizzontalPath({A, 6}, {D, 6}) == false);
    CHECK(board.clearPath({A, 6}, {D, 6}) == false);
  }
  SUBCASE("Testing clearOrizzontalPath true") {
    CHECK(board.clearOrizzontalPath({B, 5}, {E, 5}) == true);
    CHECK(board.clearPath({B, 5}, {E, 5}) == true);
    CHECK(board.clearOrizzontalPath({B, 6}, {D, 6}) == true);
    CHECK(board.clearPath({B, 6}, {D, 6}) == true);
  }
  SUBCASE("Testing clearDiagonalPath false") {
    CHECK(board.clearDiagonalPath({0, 5}, {2, 7}) == false);
    CHECK(board.clearPath({0, 5}, {2, 7}) == false);
  }
  SUBCASE("Testing clearDiagonalPath false") {
    CHECK(board.clearDiagonalPath({1, 7}, {6, 0}) == true);
    CHECK(board.clearPath({1, 7}, {6, 0}) == true);
    CHECK(board.clearDiagonalPath({0, 5}, {1, 6}) == true);
    CHECK(board.clearPath({0, 5}, {1, 6}) == true);
  }
  SUBCASE("Testing clearPath with disconnected cell") {
    CHECK(board.clearPath({1, 2}, {0, 5}) == true);
    CHECK(board.clearPath({4, 2}, {1, 1}) == true);
  }
};

TEST_CASE("Testing kingPosition") {
  sf::RenderWindow window;
  Board board(window);
  SUBCASE("Testing finding KingPosition") {
    board.setPiece(king, White, {0, 2});
    Point king_pos{0, 2};
    CHECK(board.kingPosition(White) == king_pos);
  }

  SUBCASE("Testing king not found") {
    CHECK_THROWS_AS(board.kingPosition(Black), std::runtime_error);
  }
};

// FINISCO IO QUESTI CASI

// FINISCO IO QUESTI CASI

// COMPILA E RUNNA I TEST E OSSERVA IL TEST CHE FALLISCE, POI CHIAMAMI PER
// PARLARNE.

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

/*
// test Game
TEST_CASE("Testing  rightStarting") {
  Game game(std::string("nameWhite"), std::string("nameWhite"));
  game.getBoard().setPiece(rook, White, {A, 0});
  game.getBoard().setPiece(queen, Black, {B, 7});

  SUBCASE("Testing rightStarting true ") {
    CHECK(game.rightStarting({A, 0}) == true);
    game.setPlayerTurn_(Black);
    CHECK(game.getPlayerTurn() == Black);
    CHECK(game.rightStarting({B, 7}) == true);
  }

  SUBCASE("Testing rightStarting false") {
    CHECK(game.rightStarting({B, 7}) == false);
    CHECK(game.rightStarting({C, 7}) == false);
    // CHECK_THROWS_AS(game.rightStarting({C, 7}), std::runtime_error);

    game.setPlayerTurn_(Black);
    CHECK(game.rightStarting({A, 0}) == false);
    CHECK(game.rightStarting({C, 7}) == false);
    // CHECK_THROWS_AS(game.rightStarting({C, 7}), std::runtime_error);
  }
}
TEST_CASE("Testing  rightArrival") {
  Game game("nameWhite", "nameWhite");
  game.getBoard().setPiece(rook, White, {A, 0});
  game.getBoard().setPiece(queen, Black, {B, 7});

  SUBCASE("Testing rightArrival true ") {
    CHECK(game.rightArrival({A, 5}) == true);
    CHECK(game.rightArrival({B, 7}) == true);
    game.setPlayerTurn_(Black);
    CHECK(game.getPlayerTurn() == Black);
    CHECK(game.rightArrival({A, 0}) == true);
    CHECK(game.rightArrival({B, 4}) == true);
  }

  SUBCASE("Testing rightArrival false") {
    CHECK(game.rightArrival({A, 0}) == false);
    game.setPlayerTurn_(Black);
    CHECK(game.rightArrival({B, 7}) == false);
  }
};

TEST_CASE("Testing  createCheck") {
  Game game("nameWhite", "nameWhite");
  game.getBoard().setPiece(king, White, {D, 3});

  SUBCASE("Testing createCheck I") {
    game.getBoard().setPiece(rook, Black, {H, 3});
    CHECK(game.createCheck({D, 3}, {E, 3}) == true);
    CHECK(game.createCheck({D, 3}, {C, 3}) == true);
    CHECK(game.createCheck({D, 3}, {D, 4}) == false);
    // non dovrebbe essersi spostato
    game.createCheck({D, 3}, {D, 4});
    CHECK(game.getBoard().selectPiece({D, 3})->getName() == king);
  }
  SUBCASE("Testing createCheck II") {
    game.getBoard().setPiece(rook, Black, {H, 3});
    game.getBoard().setPiece(rook, White, {E, 3});
    CHECK(game.createCheck({E, 3}, {F, 3}) == false);
    CHECK(game.createCheck({E, 3}, {H, 3}) == false);
    CHECK(game.createCheck({E, 3}, {E, 2}) == true);
    // non dovrebbe essersi spostato
    game.createCheck({E, 3}, {D, 4});
    CHECK(game.getBoard().selectPiece({E, 3})->getName() == rook);
  }

  SUBCASE("Testing createCheck III") {
    game.getBoard().setPiece(pawn, White, {E, 3});
    game.getBoard().setPiece(rook, Black, {H, 3});

    CHECK(game.createCheck({E, 3}, {E, 4}) == true);
    CHECK(game.createCheck({E, 3}, {E, 5}) == true);
    CHECK(game.createCheck({E, 3}, {F, 4}) == true);
    CHECK(game.createCheck({E, 3}, {E, 2}) == true);
  }

  SUBCASE("Testing createCheck IV") {
    game.getBoard().setPiece(pawn, White, {D, 4});
    game.getBoard().setPiece(queen, Black, {D, 6});

    CHECK(game.createCheck({D, 4}, {D, 5}) == false);
    CHECK(game.createCheck({D, 4}, {D, 6}) == false);
    CHECK(game.createCheck({D, 4}, {D, 2}) == true);
  }

  SUBCASE("Testing createCheck V") {
    game.getBoard().setPiece(rook, White, {A, 1});
    CHECK(game.createCheck({A, 1}, {A, 2}) == false);
    CHECK(game.createCheck({A, 1}, {A, 3}) == false);
  }

  SUBCASE("Testing createCheck VI") {
    game.getBoard().setPiece(bishop, White, {C, 2});
    game.getBoard().setPiece(bishop, Black, {F, 5});

    CHECK(game.createCheck({C, 2}, {D, 1}) == true);
    CHECK(game.createCheck({C, 2}, {E, 4}) == false);
  }

  SUBCASE("Testing createCheck VII") {
    game.getBoard().setPiece(knight, White, {B, 1});
    game.getBoard().setPiece(bishop, Black, {H, 7});

    CHECK(game.createCheck({B, 1}, {C, 3}) == true);
    CHECK(game.createCheck({B, 1}, {A, 3}) == true);
  }

  SUBCASE("Testing createCheck VIII") {
    game.getBoard().setPiece(pawn, White, {D, 4});
    game.getBoard().setPiece(queen, Black, {D, 6});
    game.getBoard().setPiece(rook, Black, {H, 7});

    CHECK(game.createCheck({D, 4}, {D, 5}) == false);
    CHECK(game.createCheck({D, 4}, {C, 4}) == true);
  }

  SUBCASE("Testing createCheck IX") {
    game.getBoard().setPiece(pawn, White, {E, 4});
    CHECK(game.createCheck({E, 4}, {E, 4}) == false);  // nessun movimento
  }
}

TEST_CASE("Game::validMove") {
  Game game("whiteName", "blackName");

  SUBCASE("1. Casella di partenza vuota → false") {
    CHECK(game.validMove({E, 2}, {E, 4}) == false);
  }

  SUBCASE("2. Mossa su casella occupata da proprio pezzo → false") {
    game.getBoard().setPiece(pawn, White, {E, 2});
    game.getBoard().setPiece(pawn, White, {E, 4});
    CHECK(game.validMove({E, 2}, {E, 4}) == false);
  }

  SUBCASE("3. Mossa non valida per il tipo di pezzo → false") {
    game.getBoard().setPiece(rook, White, {E, 2});
    CHECK(game.validMove({E, 2}, {F, 3}) == false);
  }

  SUBCASE("4. Percorso ostruito → false") {
    game.getBoard().setPiece(rook, White, {E, 2});
    game.getBoard().setPiece(pawn, White, {E, 3});
    CHECK(game.validMove({E, 2}, {E, 4}) == false);
  }

  SUBCASE("5. Pedone non può mangiare frontalmente → false") {
    game.getBoard().setPiece(pawn, White, {E, 2});
    game.getBoard().setPiece(pawn, Black, {E, 3});
    CHECK(game.validMove({E, 2}, {E, 3}) == false);
  }

  SUBCASE("6. Mossa che lascia il re sotto scacco → false") {
    game.getBoard().setPiece(king, White, {E, 1});
    game.getBoard().setPiece(rook, Black, {E, 4});
    game.getBoard().setPiece(pawn, White, {E, 2});
    CHECK(game.validMove({E, 2}, {E, 3}) ==
          false);  // Rimuove la protezione, re sotto scacco
  }

  SUBCASE("7. Mossa valida → true") {
    game.getBoard().setPiece(rook, White, {E, 2});
    game.getBoard().setPiece(king, White, {E, 4});
    CHECK(game.validMove({E, 2}, {E, 3}) == true);
  }

  SUBCASE("Testing validMove 0") {
    game.getBoard().setPiece(king, White, {E, 0});
    game.getBoard().setPiece(pawn, White, {E, 4});
    game.getBoard().setPiece(bishop, White, {B, 5});
    game.getBoard().setPiece(pawn, Black, {D, 6});
    game.getBoard().setPiece(king, Black, {E, 7});
    game.setPlayerTurn_(Black);
    CHECK(game.validMove({D, 6}, {D, 4}) == false);
    game.getBoard().movePiece({D, 6}, {D, 4});
    CHECK(game.validMove({E, 7}, {F, 7}) == false);
    CHECK(game.validMove({E, 7}, {D, 6}) == false);
  }
  SUBCASE("Testing validMove I") {
    game.getBoard().setPiece(king, White, {D, 3});
    game.getBoard().setPiece(rook, Black, {H, 3});
    CHECK(game.validMove({D, 3}, {E, 3}) == false);
    CHECK(game.validMove({D, 3}, {C, 3}) == false);
    CHECK(game.validMove({D, 3}, {D, 4}) == true);
    game.validMove({D, 3}, {D, 4});  // non valido, non si muove
    CHECK(game.getBoard().selectPiece({D, 3})->getName() == king);
  }

  SUBCASE("Testing validMove II") {
    game.getBoard().setPiece(king, White, {D, 3});
    game.getBoard().setPiece(rook, Black, {H, 3});
    game.getBoard().setPiece(rook, White, {E, 3});
    CHECK(game.validMove({E, 3}, {F, 3}) == true);
    CHECK(game.validMove({E, 3}, {H, 3}) == true);
    CHECK(game.validMove({E, 3}, {E, 2}) == false);
    game.validMove({E, 3}, {D, 4});  // non valido, non si muove
    CHECK(game.getBoard().selectPiece({E, 3})->getName() == rook);
  }

  SUBCASE("Testing validMove III") {
    game.getBoard().setPiece(king, White, {D, 3});
    game.getBoard().setPiece(pawn, White, {E, 3});
    game.getBoard().setPiece(rook, Black, {H, 3});

    CHECK(game.validMove({E, 3}, {E, 4}) == false);
    CHECK(game.validMove({E, 3}, {E, 5}) == false);
    CHECK(game.validMove({E, 3}, {F, 4}) == false);
    CHECK(game.validMove({E, 3}, {E, 2}) == false);
  }

  SUBCASE("Testing validMove IV") {
    game.getBoard().setPiece(king, White, {D, 3});
    game.getBoard().setPiece(pawn, White, {D, 4});
    game.getBoard().setPiece(queen, Black, {D, 6});

    CHECK(game.validMove({D, 4}, {D, 5}) == true);
    CHECK(game.validMove({D, 4}, {D, 6}) == false);
    CHECK(game.validMove({D, 4}, {D, 2}) == false);
  }

  SUBCASE("Testing validMove V") {
    game.getBoard().setPiece(king, White, {D, 3});
    game.getBoard().setPiece(rook, White, {A, 1});
    CHECK(game.validMove({A, 1}, {A, 2}) == true);
    CHECK(game.validMove({A, 1}, {A, 3}) == true);
  }

  SUBCASE("Testing validMove VI") {
    game.getBoard().setPiece(king, White, {D, 3});
    game.getBoard().setPiece(bishop, White, {C, 2});
    game.getBoard().setPiece(bishop, Black, {F, 5});

    CHECK(game.validMove({C, 2}, {D, 1}) == false);  // scopre diagonale
    CHECK(game.validMove({C, 2}, {E, 4}) ==
          false);  // anche se minaccia non cambia
  }

  SUBCASE("Testing validMove VII") {
    game.getBoard().setPiece(king, White, {D, 3});
    game.getBoard().setPiece(knight, White, {B, 1});
    game.getBoard().setPiece(bishop, Black, {H, 7});

    CHECK(game.validMove({B, 1}, {C, 3}) == false);
    CHECK(game.validMove({B, 1}, {A, 3}) == false);
  }

  SUBCASE("Testing validMove VIII") {
    game.getBoard().setPiece(king, White, {D, 3});
    game.getBoard().setPiece(pawn, White, {D, 4});
    game.getBoard().setPiece(queen, Black, {D, 6});
    game.getBoard().setPiece(rook, Black, {H, 7});

    CHECK(game.validMove({D, 4}, {D, 5}) == true);
    CHECK(game.validMove({D, 4}, {C, 4}) == false);
  }

  SUBCASE("Testing validMove IX") {
    game.getBoard().setPiece(king, White, {D, 3});
    game.getBoard().setPiece(pawn, White, {E, 4});
    CHECK(game.validMove({E, 4}, {E, 4}) ==
          false);  // nessun movimento tecnicamente vietato?
  }
}

TEST_SUITE("Test validMove") {
  TEST_CASE("Basic Pawn Moves") {
    Game game("Player1", "Player2");
    game.getBoard().setPiece(pawn, White, {A, 1});

    SUBCASE("Valid single push") {
      CHECK(game.validMove({A, 1}, {A, 2}) == true);
    }

    SUBCASE("Invalid backward move") {
      CHECK(game.validMove({A, 1}, {A, 0}) == false);
    }
  }

  TEST_CASE("Pawn Captures") {
    Game game("Player1", "Player2");
    game.getBoard().setPiece(pawn, White, {C, 2});
    game.getBoard().setPiece(pawn, Black, {D, 3});

    SUBCASE("Valid diagonal capture") {
      CHECK(game.validMove({C, 2}, {D, 3}) == true);
    }

    SUBCASE("Invalid vertical capture") {
      game.getBoard().setPiece(pawn, Black, {C, 3});
      CHECK(game.validMove({C, 2}, {C, 3}) == false);
    }
  }

  TEST_CASE("King Safety") {
    Game game("Player1", "Player2");
    game.getBoard().setPiece(king, White, {E, 1});
    game.getBoard().setPiece(rook, Black, {E, 7});

    SUBCASE("Blocked by check") {
      CHECK(game.validMove({E, 1}, {E, 2}) == false);
    }
  }

  TEST_CASE("Turn Validation") {
    Game game("Player1", "Player2");
    game.getBoard().setPiece(pawn, White, {A, 2});
    game.getBoard().setPiece(pawn, Black, {A, 6});

    SUBCASE("White's turn") {
      CHECK(game.validMove({A, 2}, {A, 3}) == true);
      CHECK(game.validMove({A, 6}, {A, 5}) == false);
    }

    SUBCASE("Black's turn") {
      game.setPlayerTurn_(Black);
      CHECK(game.validMove({A, 6}, {A, 5}) == true);
      CHECK(game.validMove({A, 2}, {A, 3}) == false);
    }
  }
}

*/
// TEST_CASE("Testing checkmate") {
//   sf::RenderWindow window;
//   Game game("Tizio 1", "Tizio 2", window);
//   for (int i{0}; i < 8; ++i) {
//     for (int k{0}; k < 8; ++k) {
//       game.getBoard().clearPieceAt({i, k});
//     }
//   };
//   game.getBoard().setPiece(king, White, {0, 0});
//   game.getBoard().setPiece(queen, Black, {2, 0});
//   game.getBoard().setPiece(rook, Black, {2, 1});
//   game.checkGameOver();
//   CHECK(game.getGameOver() == true);
// }
//