#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "Board.hpp"
#include "doctest.h"

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
