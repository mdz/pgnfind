#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_ASSERT_H
#include <assert.h>
#else
#define assert(x)
#endif

#include <iostream.h>

#include "ChessMove.h"

// For searching diagonals
static const struct boardvec vectors[] = {
  // Diagonals
  { 1, 1 },
  { -1, 1 },
  { 1, -1 },
  { -1, -1 },
  // Horizontals and verticals
  { 1, 0 },
  { -1, 0 },
  { 0, 1 },
  { 0, -1 } };

static const struct boardvec *diagonal_vectors = &vectors[0];
static const struct boardvec *hv_vectors = &vectors[4];

ChessMove::ChessMove( const char *data, ChessMove::MoveFormat format,
		      const ChessGame *game ) {
  assert( format == Algebraic || format == Descriptive );

  ChessPosition::Color pos_color =
    game->current_position().get_active_color();

  // Initialize the new state variables
  en_passant_x = 0;
  en_passant_y = 0;
  castling = game->current_position().get_castling( pos_color );
  halfmove_clock = game->current_position().get_halfmove_clock() + 1;
						  
  if ( format == Algebraic ) {
    assert( game != NULL ); // Programmatic error

    if ( strlen( data ) < 2 ) // User error
      throw InvalidMove;
      
    //
    // Call the algebraic notation parser
    //

    struct Schessmove *move = do_alg_parse( data );

    if (!move)
      throw InvalidMove;

//      cout << "Parsed move:" << endl
//  	 << "piece: " << (int)move->piece << endl
//  	 << "clarifier.rank: " << move->clarifier.rank << endl
//  	 << "clarifier.file: " << move->clarifier.file << endl
//  	 << "square.rank: " << move->square.rank << endl
//  	 << "square.file: " << move->square.file << endl
//  	 << "capture: " << move->capture << endl
//  	 << "check: " << move->check << endl
//  	 << "promote: " << (int)move->promote << endl
//  	 << "castling: " << (int)move->castling << endl
//  	 << endl
//  	 << "Deduced: " << endl;
      
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
    
    // Sanity checks...trust no one...not even the parser.

    if ( !move->castling )
      if ( move->square.file >= 1 && move->square.file <= 8  )
	end_x = move->square.file;
      else 
	throw InvalidMove;

    if ( !move->castling )
      if ( move->square.rank >= 1 && move->square.rank <= 8 )
	end_y = move->square.rank;
      else 
	throw InvalidMove;


    // Check active color
    assert( pos_color == ChessPosition::White || pos_color == ChessPosition::Black );

    ChessPiece::Color piece_color =
      ( pos_color == ChessPosition::White )
      ? ChessPiece::White
      : ChessPiece::Black;

    // Make sure we're not trying to capture our own piece, or make a
    // null move

    if ( !move->castling ) {
      piece = game->current_position().get_piece_at( end_x, end_y );
      if ( piece.get_type() != ChessPiece::Empty &&
	   piece.get_color() == piece_color )
	throw InvalidMove;
    }

    // Reset the halfmove clock for captures
    if ( move->capture )
      halfmove_clock = 0;

    
    //
    // Determine start square
    //

    int found = 0;
    switch ( piece_type ) {
    case ChessPiece::Pawn:
      int dir, second_rank;

      // Reset the halfmove clock
      halfmove_clock = 0;

      if ( pos_color == ChessPosition::White ) {
	dir = 1;
	second_rank = 2;
      } else {
	dir = -1;
	second_rank = 7;
      }

      if ( end_y == ( second_rank ) || end_y == (second_rank - dir) )
	throw InvalidMove;

      if ( move->capture ) { // Pawn captures

	start_x = move->clarifier.file;
	start_y = end_y - dir;

	if ( !( start_x >= 1 && start_x <= 8 ) ) {
	  throw InvalidMove;
	}

      } else { // Pawn moves without capture
	start_x = end_x;
	
	// Initial two-square advance
	if ( ( end_y == second_rank + ( dir * 2 ) )
	     && ( game->current_position().get_piece_at( end_x,
							 second_rank + dir )
		  .get_type() == ChessPiece::Empty )
	     && ( game->current_position().get_piece_at( end_x, second_rank )
		  == ChessPiece( ChessPiece::Pawn, piece_color ) ) ) {
	  start_y = second_rank;
	  en_passant_x = start_x;
	  en_passant_y = second_rank + dir;
	} else if ( game->current_position(). // Normal pawn move
		    get_piece_at( end_x, end_y - dir )
		    == ChessPiece( ChessPiece::Pawn, piece_color ) ) {
	  start_y = end_y - dir;
	} else {
	  throw InvalidMove;
	}
      }
      
      if ( game->current_position().get_piece_at( start_x,
						   start_y ).get_type()
	     != ChessPiece::Pawn ) {
	throw InvalidMove;
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
	    if ( found ) // ambiguous
	      throw InvalidMove;
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
      find_piece( game->current_position(),
		  diagonal_vectors, 4,
		  ChessPiece::Bishop,
		  piece_color,
		  move->clarifier );
      break;

    case ChessPiece::Rook:
      find_piece( game->current_position(),
		  hv_vectors, 4,
		  ChessPiece::Rook,
		  piece_color,
		  move->clarifier );
      break;

    case ChessPiece::Queen:
      find_piece( game->current_position(),
		  vectors, 8,
		  ChessPiece::Queen,
		  piece_color,
		  move->clarifier );
      break;

    case ChessPiece::King:
      if ( move->castling ) {
	int back_rank = ( pos_color == ChessPosition::White ? 1 : 8 );
	struct {
	  int x;
	  int y;
	} king_square = { 5, back_rank },
		  rook_square = { 0, back_rank },
		  int_squares[2] = { { 6, back_rank },
				     { 7, back_rank } };

	switch ( move->castling ) {

	case ChessPosition::Kingside:
	  rook_square.x = 8;break;

	case ChessPosition::Queenside:
	  rook_square.x = 1;break;

	default:
	  assert(0);
	}

//  	cout << "Castling: " << endl
//  	     << "king_square.x: " << king_square.x << endl
//  	     << "king_square.y: " << king_square.y << endl
//  	     << "rook_square.x: " << rook_square.x << endl
//  	     << "rook_square.y: " << rook_square.y << endl
//  	     << "int_squares[0].x" << int_squares[0].x << endl
//  	     << "int_squares[0].y" << int_squares[0].y << endl
//  	     << "int_squares[1].x" << int_squares[1].x << endl
//  	     << "int_squares[1].y" << int_squares[1].y << endl
//  	     << endl;
	  
	
	if ( game->current_position().get_piece_at( king_square.x,
						  king_square.y )
	     .get_type()
	     == ChessPiece::King
	     && game->current_position().get_piece_at( rook_square.x,
						     rook_square.y )
	     .get_type()
	     == ChessPiece::Rook
	     && game->current_position().get_piece_at( int_squares[0].x,
						     int_squares[0].y )
	     .get_type()
	     == ChessPiece::Empty
	     && game->current_position().get_piece_at( int_squares[1].x,
						     int_squares[1].y )
	     .get_type()
	     == ChessPiece::Empty ) {
	  start_x = king_square.x;
	  start_y = king_square.y;
	  end_x = int_squares[1].x;
	  end_y = int_squares[1].y;
	} else {
	  throw InvalidMove;
	}

      } else {
	struct {
	  int x;
	  int y;
	} starts[8] = { { end_x, end_y - 1 },
			{ end_x, end_y + 1 },
			{ end_x + 1, end_y },
			{ end_x + 1, end_y + 1 },
			{ end_x + 1, end_y - 1 },
			{ end_x - 1, end_y + 1 },
			{ end_x - 1, end_y - 1 },
			{ end_x - 1, end_y } };

	// Look for a king on each of them
	for ( int i = 0 ; i < 8 ; ++i ) {
	  if (starts[i].x >= 1 && starts[i].x <= 8 && // is on the board
	      starts[i].y >= 1 && starts[i].y <= 8 &&
	      game->current_position().get_piece_at( starts[i].x,
						     starts[i].y )
	      == ChessPiece( ChessPiece::King, piece_color ) ) {

	    start_x = starts[i].x;
	    start_y = starts[i].y;
	    found = 1;
	    break;
	  }
	}

	if (!found)
	  throw InvalidMove;
      }
					    
      break;
    default:
      // Should never happen, since we checked this case above
      assert(0);
    }
    
//      cout << "start_x: " << start_x << endl;
//      cout << "start_y: " << start_y << endl;

  } else { // Descriptive notation
    // This isn't here yet
    throw InvalidMove;
  }
}

// Search for a piece along the specified vectors from the end square,
// to see if it could have moved there.  If none is found, or more
// than one, throw InvalidMove
void ChessMove::find_piece( const ChessPosition &pos,
			    const struct boardvec *vectors,
			    int num_vectors,
			    ChessPiece::Type piece_type,
			    ChessPiece::Color color,
			    const Sclarifier &clarifier ) {
  int x, y, vec;
  int found = 0;
  for ( vec = 0 ; vec < num_vectors ; ++vec ) {
    for ( x = end_x + vectors[ vec ].x,    // Start one square away from
	    y = end_y + vectors[ vec ].y ; // the end square

	  x >= 1 && x <= 8 && y >= 1 && y <= 8 ; // Stay on the board

	  x += vectors[ vec ].x,           // Move in the direction of
	    y += vectors[ vec ].y ) {      // the current vector

      // It's rather inefficient to check the clarifier this way, but
      // they're relatively rare in practice
      if ( clarifier.file && clarifier.file != x )
	continue;
      if ( clarifier.rank && clarifier.rank != y )
	continue;

//        cout << "Looking for a piece on (" << x << ',' << y << ')'
//  	   << endl;
      ChessPiece piece = pos.get_piece_at( x, y );
      if ( piece.get_type() != ChessPiece::Empty ) {
	if ( piece == ChessPiece( piece_type, color ) ) {
	      
	  if ( found ) // Ambiguous
	    throw InvalidMove;
	      
	  start_x = x;
	  start_y = y;
	  found = 1;
	      
	}
	    
	break;
      }
    }
  }

  if (!found)
    throw InvalidMove;
}
