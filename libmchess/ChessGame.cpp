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
