#include <iostream.h>
#include <ChessPosition.h>

int main(int argc, char *argv[]) {
  //char buf[1024] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  char buf[1024] = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KkqQ e3 0 1";
  ChessPosition pos( buf );

  pos.write_FEN( buf );
  cout << "FEN: " << buf << endl;

  pos.set_piece_at( "a2", ChessPiece( ChessPiece::Rook, ChessPiece::Black ) );

  pos.write_FEN( buf );
  cout << "FEN: " << buf << endl;

  cout << "Piece at a1 is: " << pos.get_piece_at( "a1" ).get_type()
       << ',' << pos.get_piece_at( "a1" ).get_color() << endl;
}
