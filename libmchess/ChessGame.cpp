#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_ASSERT_H
#include <assert.h>
#else
#define assert(x)
#endif

#include "ChessGame.h"

ChessGame::ChessGame() {
  int i;
  for( i = 0 ; i < CHESSGAME_MAX_MOVES ; ++i ) {
    moves[i] = NULL;
  }
}

ChessGame::ChessGame( const char *pgn ) {
  ChessGame();
}

ChessGame::ChessGame( const ChessPosition &pos ):
  position(pos) {
  ChessGame();
}

const ChessPosition &ChessGame::current_position() const {
  return position;
}

void ChessGame::make_move( const ChessMove &move ) {
  int start_x = move.get_start_x();
  int start_y = move.get_start_y();
  int end_x = move.get_end_x();
  int end_y = move.get_end_y();

  ChessPiece piece( position.get_piece_at( start_x, start_y ) );

  // ChessMove should check this
  assert( piece.get_type() != ChessPiece::Empty );

  position.set_piece_at( start_x, start_y, ChessPiece( ChessPiece::Empty ) );
  position.set_piece_at( end_x, end_y, piece );
  position.increment_move();
}
