// -*- C++ -*-
#ifndef _CHESSGAME_H
#define _CHESSGAME_H

#include "ChessPiece.h"
#include "ChessPosition.h"

class ChessGame {
  //
  // Constructors and destructors
  //
public:
  // Default constructor (initial game setup, move 1)
  ChessGame();

  // Read PGN data
  ChessGame( const char *pgn );

  //
  // Accessors
  //
  const ChessPosition &current_position() const;
};

#endif /* !_CHESSGAME_H */
