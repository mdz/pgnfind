#include <iostream.h>
#include "ChessMove.h"

int main(int argc, char *argv[]) {
  char buf[1024];
  ChessGame game;/*( ChessPosition(
		   "rnbqkbnr/pppppppp/8/8/8/8/8/RNBQKBNR w KQkq - 0 1" ) );*/

  while (1) {
    cin.getline( buf, 1024 );
    try {
      ChessMove move( buf, ChessMove::Algebraic, &game );

      game.make_move( move );

      game.current_position().write_FEN( buf );
      cout << buf << endl;
    }

    catch (ChessMove::Exception cme) {
      if ( cme == ChessMove::InvalidMove ) {
	cout << "Invalid move" << endl;
      } else if ( cme == ChessMove::IllegalMove ) {
	cout << "Illegal move" << endl;
      } else {
	cout << "Unrecognized exception" << endl;
      }
    }

  }
}
