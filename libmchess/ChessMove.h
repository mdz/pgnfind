// -*- C++ -*-
#ifndef _CHESSMOVE_H
#define _CHESSMOVE_H

#include <stdlib.h>

#include "ChessGame.h"
#include "alg_parse.h"

struct boardvec {
  int x;
  int y;
};

class ChessMove {
  //
  // Public data types
  //
public:
  enum MoveFormat { Algebraic, Descriptive };
  enum Exception { InvalidMove, IllegalMove };

  //
  // Constructors and destructors
  //
public:
  // Build a ChessMove from symbolic notation.  A pointer to a
  // ChessGame object must be passed for Algebraic format, as data
  // from the board is required to determine the correct move
  ChessMove( const char *data , MoveFormat format,
	     const class ChessGame *game = NULL );

  //
  // Accessors
  //
public:
  int get_start_x();
  int get_start_y();
  int get_end_x();
  int get_end_y();

  //
  // Protected methods
  //
protected:
  void find_piece( const ChessPosition &pos,
		   const struct boardvec *vectors,
		   int num_vectors,
		   ChessPiece::Type piece_type,
		   ChessPiece::Color color,
		   const Sclarifier &clarifier );


  //
  // Protected data
  //
protected:
  int start_x;
  int start_y;
  int end_x;
  int end_y;
};

#endif /* !_CHESSMOVE_H */
