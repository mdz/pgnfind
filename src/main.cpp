#include <iostream.h>
#include "ChessMove.h"

int main(int argc, char *argv[]) {
  //char buf[1024] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  //  char buf[1024] = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KkqQ e3 0 1";
  //  ChessPosition pos( buf );

//    pos.write_FEN( buf );
//    cout << "FEN: " << buf << endl;

//    pos.set_piece_at( "a2", ChessPiece( ChessPiece::Rook, ChessPiece::Black ) );

//    pos.write_FEN( buf );
//    cout << "FEN: " << buf << endl;

//    cout << "Piece at a1 is: " << pos.get_piece_at( "a1" ).get_type()
//    << ',' << pos.get_piece_at( "a1" ).get_color() << endl;

  char buf[1024];
  ChessGame game;

  while (1) {
    cin.getline( buf, 1024 );
    try {
      ChessMove move( buf, ChessMove::Algebraic, &game );
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
