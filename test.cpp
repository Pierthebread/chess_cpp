#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "Board.hpp"
#include "Game.hpp"
#include "doctest.h"
/*
///////// Test Pieces
// Test king
TEST_CASE("Testing  mossaValida") {
  King king(White);
  Point p_from{A, 2};
  king.setPosition(p_from);

  SUBCASE("mossaValida true horizontal") {
    Point p_to{A, 3};
    CHECK(king.validPieceMove(p_to) == true);
  }
  SUBCASE("mossaValida true vertical") {
    Point p_to{B, 2};
    CHECK(king.validPieceMove(p_to) == true);
  }
  SUBCASE("mossaValida true diagonal") {
    Point p_to{B, 3};
    CHECK(king.validPieceMove(p_to) == true);
  }
  SUBCASE("mossaValida false") {
    Point p_to{A, 4};
    CHECK(king.validPieceMove(p_to) == false);
  }
};

// Test Queen
TEST_CASE("Testing  validMove") {
  Queen queen(White);
  Point p_from{A, 2};
  queen.setPosition(p_from);

  SUBCASE("validMove true horizontal") {
    Point to{A, 4};
    CHECK(queen.validPieceMove(to) == true);
  }

  SUBCASE("validMove true vertical") {
    Point to{H, 2};
    CHECK(queen.validPieceMove(to) == true);
  }

  SUBCASE("validMove true diagonal") {
    Point to{C, 4};
    CHECK(queen.validPieceMove(to) == true);
  }

  SUBCASE("validMove false") {
    Point to{C, 8};
    CHECK(queen.validPieceMove(to) == false);
  }
};

// Test bishop

TEST_CASE("Testing  validMove") {
  Bishop bishop(White);
  Point p_from{A, 2};
  bishop.setPosition(p_from);

  SUBCASE("validMove true") {
    Point p_to{C, 4};
    CHECK(bishop.validPieceMove(p_to) == true);
  }

  SUBCASE("validMove false") {
    Point p_to{D, 4};
    CHECK(bishop.validPieceMove(p_to) == false);
  }
};

// Test knight

TEST_CASE("Testing  mossaValida") {
  Knight knight(White);
  Point p_from{A, 2};
  knight.setPosition(p_from);

  SUBCASE("mossaValida true") {
    Point p_to{B, 4};
    CHECK(knight.validPieceMove(p_to) == true);
  }

  SUBCASE("mossaValida false") {
    Point p_to{D, 4};
    CHECK(knight.validPieceMove(p_to) == false);
  }
};

// Test rook

TEST_CASE("Testing  validMove") {
  Rook rook(White);
  Point p_from{A, 2};
  rook.setPosition(p_from);

  SUBCASE("validMove true horizontal") {
    Point p_to{A, 6};
    CHECK(rook.validPieceMove(p_to) == true);
  }

  SUBCASE("validMove true vertical") {
    Point p_to{H, 2};
    CHECK(rook.validPieceMove(p_to) == true);
  }

  SUBCASE("validMove false") {
    Point p_to{C, 8};
    CHECK(rook.validPieceMove(p_to) == false);
  }
};

// Test Pawn
TEST_CASE("Testing  validMove") {
  Pawn pawn(Black);
  Point p_from{C, 2};
  pawn.setPosition(p_from);

  SUBCASE("Testing double advancement") {
    Point p_to{C, 4};
    pawn.setMoved(true);
    CHECK(pawn.validPieceMove(p_to) == false);
    pawn.setMoved(false);
    CHECK(pawn.validPieceMove(p_to) == true);
  }

  SUBCASE("Testing single advancement") {
    Point p_to{C, 3};
    CHECK(pawn.validPieceMove(p_to) == true);
  }

  SUBCASE("Testing lateral movement") {
    Point p_to{D, 3};
    CHECK(pawn.validPieceMove(p_to) == true);
  }
};

///////////////////////////////////////////////////////////////////////////////////
// Test Board
TEST_CASE("Testing  selectPiece") {
  Board board;
  board.setPiece(king, White, {A, 2});
  board.setPiece(rook, Black, {B, 6});

  SUBCASE("Testing selectPiece true") {
    CHECK(board.selectPiece({A, 2})->getName() == king);
    CHECK(board.selectPiece({A, 2})->getColor() == White);
    CHECK(board.selectPiece({B, 6})->getName() == rook);
    CHECK(board.selectPiece({B, 6})->getColor() != White);
  }

  SUBCASE("Testing selectPiece false") {
    CHECK(board.selectPiece({A, 3}) == nullptr);
    CHECK(board.selectPiece({E, 7}) == nullptr);
    CHECK_THROWS_AS(board.selectPiece({E, 20}), std::runtime_error);
  }
};

TEST_CASE("Testing  movePiece") {
  Board board;
  board.setPiece(king, White, {A, 2});

  SUBCASE("Testing movePiece with no piece_to") {
    board.movePiece({A, 2}, {A, 6});
    CHECK(board.selectPiece({A, 2}) == nullptr);
    CHECK(board.selectPiece({A, 6})->getName() == king);
  }

  SUBCASE("Testing movePiece with piece_to") {
    board.setPiece(rook, White, {A, 6});
    board.movePiece({A, 2}, {A, 6});
    CHECK(board.selectPiece({A, 2}) == nullptr);
    CHECK(board.selectPiece({A, 6})->getName() == king);
  }

  SUBCASE("Testing movePiece not working") {
    CHECK_THROWS(board.movePiece({B, 2}, {A, 2}));
  }

  SUBCASE("Testing movePiece changing has_moved") {
    board.setPiece(rook, White, {A, 6});
    CHECK(board.selectPiece({A, 2})->getMoved() == false);
    CHECK(board.selectPiece({A, 6})->getMoved() == false);
  }

  SUBCASE("Testing movePiece changing has_moved") {
    board.setPiece(rook, White, {A, 6});
    board.movePiece({A, 2}, {A, 4});  // king
    board.movePiece({A, 6}, {A, 7});  // rook
    CHECK(board.selectPiece({A, 4})->getMoved() == true);
    CHECK(board.selectPiece({A, 7})->getMoved() == true);
  }
};

TEST_CASE("Testing clearPath") {
  Board board;
  board.setPiece(king, White, {A, 2});
  board.setPiece(pawn, Black, {B, 6});
  board.setPiece(bishop, White, {C, 3});

  SUBCASE("Testing clearVerticalPath false") {
    CHECK(board.clearVerticalPath({B, 5}, {B, 7}) == false);
    CHECK(board.clearPath({B, 5}, {B, 7}) == false);
  }
  SUBCASE("Testing clearVerticalPath true") {
    CHECK(board.clearVerticalPath({B, 2}, {B, 5}) == true);
    CHECK(board.clearPath({B, 2}, {B, 5}) == true);
    CHECK(board.clearVerticalPath({B, 5}, {B, 6}) == true);
    CHECK(board.clearPath({B, 5}, {B, 6}) == true);
  }
  SUBCASE("Testing clearHorizontalPath false") {
    CHECK(board.clearHorizontalPath({A, 6}, {D, 6}) == false);
    CHECK(board.clearPath({A, 6}, {D, 6}) == false);
  }
  SUBCASE("Testing clearHorizontalPath true") {
    CHECK(board.clearHorizontalPath({B, 5}, {E, 5}) == true);
    CHECK(board.clearPath({B, 5}, {E, 5}) == true);
    CHECK(board.clearHorizontalPath({B, 6}, {D, 6}) == true);
    CHECK(board.clearPath({B, 6}, {D, 6}) == true);
  }
  SUBCASE("Testing clearDiagonalPath false") {
    CHECK(board.clearDiagonalPath({A, 5}, {C, 7}) == false);
    CHECK(board.clearPath({A, 5}, {C, 7}) == false);
  }
  SUBCASE("Testing clearDiagonalPath false") {
    CHECK(board.clearDiagonalPath({B, 7}, {G, 0}) == true);
    CHECK(board.clearPath({B, 7}, {G, 0}) == true);
    CHECK(board.clearDiagonalPath({A, 5}, {B, 6}) == true);
    CHECK(board.clearPath({A, 5}, {B, 6}) == true);
  }
  SUBCASE("Testing clearPath with disconnected cell") {
    CHECK(board.clearPath({B, 2}, {A, 5}) == true);
    CHECK(board.clearPath({E, 2}, {B, 1}) == true);
  }
  SUBCASE("Testing clearVerticalPath false") {
    CHECK(board.clearVerticalPath({B, 5}, {B, 7}) == false);
    CHECK(board.clearPath({B, 5}, {B, 7}) == false);
  }
  SUBCASE("Testing clearVerticalPath true") {
    CHECK(board.clearVerticalPath({B, 2}, {B, 5}) == true);
    CHECK(board.clearPath({B, 2}, {B, 5}) == true);
    CHECK(board.clearVerticalPath({B, 5}, {B, 6}) == true);
    CHECK(board.clearPath({B, 5}, {B, 6}) == true);
  }
  SUBCASE("Testing clearHorizontalPath false") {
    CHECK(board.clearHorizontalPath({A, 6}, {D, 6}) == false);
    CHECK(board.clearPath({A, 6}, {D, 6}) == false);
  }
  SUBCASE("Testing clearHorizontalPath true") {
    CHECK(board.clearHorizontalPath({B, 5}, {E, 5}) == true);
    CHECK(board.clearPath({B, 5}, {E, 5}) == true);
    CHECK(board.clearHorizontalPath({B, 6}, {D, 6}) == true);
    CHECK(board.clearPath({B, 6}, {D, 6}) == true);
  }
  SUBCASE("Testing clearDiagonalPath false") {
    CHECK(board.clearDiagonalPath({A, 5}, {C, 7}) == false);
    CHECK(board.clearPath({A, 5}, {C, 7}) == false);
  }
  SUBCASE("Testing clearDiagonalPath false") {
    CHECK(board.clearDiagonalPath({B, 7}, {G, 0}) == true);
    CHECK(board.clearPath({B, 7}, {G, 0}) == true);
    CHECK(board.clearDiagonalPath({A, 5}, {B, 6}) == true);
    CHECK(board.clearPath({A, 5}, {B, 6}) == true);
  }
  SUBCASE("Testing clearPath with disconnected cell") {
    CHECK(board.clearPath({B, 2}, {A, 5}) == true);
    CHECK(board.clearPath({E, 2}, {B, 1}) == true);
  }
};

TEST_CASE("Testing kingPosition") {
  Board board;
  board.setPiece(king, White, {A, 2});
  Point king_pos{A, 2};
  bool check = (board.kingPosition(White) == king_pos);
  CHECK(check == true);
  CHECK_THROWS_AS(board.kingPosition(Black), std::runtime_error);
};
TEST_CASE("Testing kingPosition") {
  Board board;
  board.setPiece(king, White, {A, 2});
  Point king_pos{A, 2};
  bool check = (board.kingPosition(White) == king_pos);
  CHECK(check == true);
  CHECK_THROWS_AS(board.kingPosition(Black), std::runtime_error);
};

TEST_CASE("Testing isPromotion and Promote") {
  Board board;
  board.setPiece(pawn, Black, {A, 6});
  board.setPiece(pawn, White, {A, 1});

  SUBCASE("Testing isPromotion") {
    CHECK(board.isPromotion({A, 1}, {A, 0}) == true);
    CHECK(board.isPromotion({A, 1}, {A, 2}) == false);
    CHECK(board.isPromotion({A, 6}, {A, 7}) == true);
    CHECK(board.isPromotion({A, 6}, {A, 5}) == false);
    CHECK(board.isPromotion({A, 4}, {A, 5}) == false);
    CHECK(board.isPromotion({A, 6}, {A, 0}) == false);
    CHECK(board.isPromotion({A, 1}, {A, 7}) == false);
  }

  SUBCASE("Testing promote") {
    board.movePiece({A, 1}, {A, 0});  // white
    board.promote({A, 0}, rook);
    board.movePiece({A, 6}, {A, 7});  // black
    board.promote({A, 7}, queen);
    CHECK(board.selectPiece({A, 0})->getName() == rook);
    CHECK(board.selectPiece({A, 0})->getColor() == White);
    CHECK(board.selectPiece({A, 7})->getName() == queen);
    CHECK(board.selectPiece({A, 7})->getColor() == Black);
    CHECK_THROWS(board.promote({A, 4}, queen));
    CHECK_THROWS(board.promote({A, 0}, king));
    CHECK(board.selectPiece({A, 1}) == nullptr);
  }
};



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
//TEST_CASE("Testing checkmate") {
//  sf::RenderWindow window;
//  Game game("Tizio 1", "Tizio 2", window);
//  for (int i{0}; i < 8; ++i) {
//    for (int k{0}; k < 8; ++k) {
//      game.getBoard().clearPieceAt({i, k});
//    }
//  };
//  game.getBoard().setPiece(king, White, {0, 0});
//  game.getBoard().setPiece(queen, Black, {2, 0});
//  game.getBoard().setPiece(rook, Black, {2, 1});
//  game.checkGameOver();
//  CHECK(game.getGameOver() == true);
//}
//