// -*- C++ -*-
#ifndef _CHESSMOVE_H
#define _CHESSMOVE_H

#include <stdlib.h>

#include "ChessGame.h"
#include "alg_parse_int.h"

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
  // These are guaranteed to return values in the range [1,8]
  int get_start_x() const { return start_x; };
  int get_start_y() const { return start_y; };
  int get_end_x() const   { return end_x; };
  int get_end_y() const   { return end_y; };

  // These will return values suitable for
  // ChessPosition::set_en_passant indicating the en passant capture
  // square AFTER this move is made
  int get_en_passant_x() const { return en_passant_x; };
  int get_en_passant_y() const { return en_passant_y; };

  // This will return a value suitable for
  // ChessPosition::set_halfmove_counter indicating the proper value
  // for the halfmove clock AFTER this move is made
  int get_halfmove_clock() const { return halfmove_clock; };

  // This will return the castling status AFTER this move is made
  ChessPosition::Castling get_castling() const { return castling; };

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

  // En passant capture square AFTER this move is made
  int en_passant_x;
  int en_passant_y;

  // Castling status AFTER this move is made
  ChessPosition::Castling castling;

  // Halfmove clock AFTER this move is mode
  int halfmove_clock;
};

#endif /* !_CHESSMOVE_H */
