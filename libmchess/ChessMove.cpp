#ifdef HAVE_ASSERT_H
#include <assert.h>
#else
#define assert(x)
#endif

#include "ChessMove.h"
#include "alg_parse.h"

ChessMove::ChessMove( const char *data, ChessMove::MoveFormat format,
		      const ChessGame *game ) {
  assert( format == Algebraic || format == Descriptive );

  if ( format == Algebraic ) {
    assert( game != NULL ); // Programmatic error

    if ( strlen( data ) < 2 ) // User error
      throw InvalidMove;
      

    //
    // Call the algebraic notation parser
    //

    struct Schessmove *move = alg_parse( data );
    if (!move)
      throw InvalidMove;
      
    ChessPiece::Type piece_type = move->piece;

    // Make sure we got a valid piece

    switch ( piece_type ) {
    case ChessPiece::Pawn:
    case ChessPiece::Bishop:
    case ChessPiece::Rook:
    case ChessPiece::Queen:
    case ChessPiece::King:
      break;
    default:
      throw InvalidMove;
    }


    // Trust no one...not even the parser.

    if ( move->square.file >= 1 && move->square.file <= 8  )
      end_x = move->square.file;
    else 
      throw InvalidMove;

    if ( move->square.rank >= 1 && move->square.rank <= 8 )
      end_y = move->square.rank;
    else 
      throw InvalidMove;

    if ( piece_type == ChessPiece::Pawn &&
	 end_y <=2 ) { // Pawns should never do this
      throw InvalidMove;
    }
	
    //
    // Determine start square
    //

    switch ( piece_type ) {
    case ChessPiece::Pawn:
      if ( end_y == 1 )
	throw InvalidMove;

      // Initial two-square advance
      if ( end_y == 4 &&
	   game->current_position().get_piece_at( end_x, 2 ).get_type() ==
	   ChessPiece::Pawn ) {
	start_x = 2;
      } else {
	start_x = end_x - 1;
      }

      break;

    case ChessPiece::Knight:
      break;
    }
  } else { // Descriptive notation
    // This isn't here yet
    throw InvalidMove;
  }
}
