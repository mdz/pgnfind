// -*- C++ -*-
#ifndef _CHESSPOSITION_H
#define _CHESSPOSITION_H

#include "ChessPiece.h"

//
// Note: all positions are specified in rank-file notation (e.g., a4,
// h7, etc.), except in the (x, y) form of the get_piece_at and
// set_piece_at functions (which accept numerical coordinates)

class ChessPosition {
  //
  // Constructors and destructors
  //
public:

  // Default constructor
  // (start-of-game position)
  ChessPosition();

  // FEN constructor
  ChessPosition( const char *FEN );

  //
  // Public data types
  //
public:
  enum Color { White, Black };
  enum Castling { None = 0x00,
		  Queenside = 0x01,
		  Kingside = 0x02,
		  Both = Queenside | Kingside };

  //
  // Accessors
  //
public:
  // What sort of piece is at position (x, y)
  // x and y have the range [1,8]
  const ChessPiece &get_piece_at( int x, int y ) const;

  // What sort of piece is at <square>?
  const ChessPiece &get_piece_at( const char *square ) const;

  // Whose turn is it?
  Color get_active_color() const;

  // Can the specified player castle, and if so, where?
  Castling get_castling( Color ) const;

  // If an en passant capture is possible, on which square?
  // If none, returns 0
  int get_en_passant_x() const;
  int get_en_passant_y() const;

  // How many half-moves since the last pawn advance or capturing move?
  int get_halfmove_clock() const;

  // How many full moves so far in the game?
  int get_fullmove_number() const;

  // Dump position data as a FEN into an in-memory buffer (hope it
  // fits)
  void write_FEN( char *buf ) const;

  //
  // Mutators
  //
public:
  // Place a piece at position (x, y)
  // (replacing any existing piece on that square)
  // x and y have the range [1,8]
  void set_piece_at( int x, int y, const ChessPiece &piece );

  // Place a piece at <square>
  // (replacing any existing piece on that square)
  void set_piece_at( const char *square, const ChessPiece &piece );

  // Set active color
  void set_active_color( Color );

  // Set castling availability
  void set_castling( Color, Castling );

  // Set en passant move (0, 0 for none)
  void set_en_passant( int x, int y );

  // Set the halfmove clock
  void set_halfmove_clock( int );

  // Set the fullmove number
  void set_fullmove_number( int );

  // Increment the halfmove clock, and (if appropriate) the fullmove
  // number
  void increment_move();

  // Read a FEN from an in-memory buffer Returns 1 if successful, 0 if
  // the buffer does not contain a valid FEN
  int read_FEN( const char *buf );

  //
  // Protected member functions
  //
protected:
  // Convert between SAN square notation and integer coordinates
  static int square_x( const char *square);
  static int square_y( const char *square);
  static const char *make_square( int x, int y );

  //
  // Private data
  //
private:
  ChessPiece board[8][8];
  Color active_color;
  Castling white_castling;
  Castling black_castling;
  int en_passant_x;
  int en_passant_y;
  int halfmove_clock;
  int fullmove_number;
};

#endif /* !_CHESSPOSITION_H */
