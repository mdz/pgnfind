#ifdef HAVE_ASSERT_H
#include <assert.h>
#else
#define assert(x)
#endif

#include <iostream.h>

#include "ChessMove.h"
#include "alg_parse.h"

ChessMove::ChessMove( const char *data, ChessMove::MoveFormat format,
		      const ChessGame *game ) {
  assert( format == Algebraic || format == Descriptive );

  if ( format == Algebraic ) {
    //assert( game != NULL ); // Programmatic error

    if ( strlen( data ) < 2 ) // User error
      throw InvalidMove;
      
    //
    // Call the algebraic notation parser
    //

    struct Schessmove *move = alg_parse( data );

    if (!move)
      throw InvalidMove;

    cout << "Parsed move:" << endl
	 << "piece: " << move->piece << endl
	 << "clarifier.rank: " << move->clarifier.rank << endl
	 << "clarifier.file: " << move->clarifier.file << endl
	 << "square.rank: " << move->square.rank << endl
	 << "square.file: " << move->square.file << endl
	 << "capture: " << move->capture << endl
	 << "check: " << move->check << endl
	 << "promote: " << move->promote << endl
	 << endl
	 << "Deduced: " << endl;
      
    ChessPiece::Type piece_type = move->piece;
    ChessPiece piece;

    // Make sure we got a valid piece

    switch ( piece_type ) {
    case ChessPiece::Pawn:
    case ChessPiece::Bishop:
    case ChessPiece::Knight:
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

	
    //
    // Determine start square and color
    //

    ChessPosition::Color pos_color =
      game->current_position().get_active_color();

    assert( color == ChessPosition::White || color == ChessPosition::Black );

    ChessPiece::Color piece_color =
      ( pos_color == ChessPosition::White )
      ? ChessPiece::White
      : ChessPiece::Black;

    // Make sure we're not trying to capture our own piece
    piece = game->current_position().get_piece_at( end_x, end_y );
    if ( piece.get_type() != ChessPiece::Empty &&
	 piece.get_color() == piece_color )
      throw InvalidMove;
    
    switch ( piece_type ) {
    case ChessPiece::Pawn:
      int dir, second_rank;
      if ( pos_color == ChessPosition::White ) {
	dir = 1;
	second_rank = 2;
      } else {
	dir = -1;
	second_rank = 7;
      }

      if ( end_y == ( second_rank ) || end_y == (second_rank - dir) )
	throw InvalidMove;

      if ( move->capture ) {
	
      } else { // No capture
	start_x = end_x;
	
	// Initial two-square advance
	if ( ( end_y == second_rank + ( dir * 2 ) ) &&
	     ( game->current_position().get_piece_at( end_x, second_rank )
	       == ChessPiece( ChessPiece::Pawn, piece_color ) ) ) {
	  start_y = second_rank;
	} else if ( game->current_position(). // Normal pawn move
		    get_piece_at( end_x, end_y - dir )
		    == ChessPiece( ChessPiece::Pawn, piece_color ) ) {
	  start_y = end_y - dir;
	} else {
	  throw InvalidMove;
	}
      }
      
      break;

    case ChessPiece::Knight:
      // Build a table of possible start squares
      // We enclose this in a block so that its scope is limited
      // to this case...otherwise, the compiler might get upset
      if (1) {
	struct {
	  int x;
	  int y;
	} starts[8] = { { end_x + 1, end_y + 2 },
			{ end_x + 2, end_y + 1 },
			{ end_x + 1, end_y - 2 },
			{ end_x + 2, end_y - 1 },
			{ end_x - 1, end_y + 2 },
			{ end_x - 2, end_y + 1 },
			{ end_x - 1, end_y - 2 },
			{ end_x - 2, end_y - 1 } };
	int found = 0;
	
	// Look for a knight on each of them
	for ( int i = 0 ; i < 8 ; ++i ) {
	  if (starts[i].x >= 1 && starts[i].x <= 8 && // is on the board
	      starts[i].y >= 1 && starts[i].y <= 8 &&
	      ( move->clarifier.rank == 0 ||
		move->clarifier.rank == starts[i].y ) && // and matches a
	      ( move->clarifier.file == 0 || // clarifier if one was given
		move->clarifier.file == starts[i].x ) &&
	      game->current_position().get_piece_at( starts[i].x,
						     starts[i].y )
	      == ChessPiece( ChessPiece::Knight, piece_color ) ) {
	    start_x = starts[i].x;
	    start_y = starts[i].y;
	    found = 1;
	  }
	}

	if (!found)
	  throw InvalidMove;
      }
					    
      break;

    case ChessPiece::Bishop:
      
      break;
    default:
      // Should never happen, since we checked this case above
      assert(0);
    }
    
    cout << "start_x: " << start_x << endl;
    cout << "start_y: " << start_y << endl;

  } else { // Descriptive notation
    // This isn't here yet
    throw InvalidMove;
  }
}
