%{

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef STDC_HEADERS
#include <string.h>
#endif

#include "ChessPiece.h"
#include "ChessMove.h"
#include "alg_parse.h"

#define YYERROR_VERBOSE

#define MAX_MOVE_LEN 16
  
  static int move_index;
  static char move_str[MAX_MOVE_LEN + 1] = "";
  static struct Schessmove move_data;
  static char *err_msg;

  static int yylex();
  static int yyerror(char *);

%}

%union {
  char ch;
  int num;
  ChessPiece::Type piece;
  struct Sclarifier clarifier;
  struct Ssquare square;
  struct Schessmove move;
}

%token <ch> CHAR

%type <num> rank file
%type <piece> piece
%type <clarifier> clarifier rank_clarifier file_clarifier
%type <square> square
%type <num> capture
%type <move> input move_check move

%%

   input: move_check	{ $$ = $1; }
	;

   move_check: move	{ $$ = $1; }
	| move '+'	{ $$ = $1; $$.check = 1; }
	;

   move: piece clarifier capture square	{ $$.piece = $1;
					  $$.clarifier = $2;
					  $$.capture = $3;
					  $$.square = $4; }

	| square			{ $$.piece = ChessPiece::Pawn;
					  $$.capture = 0;
					  $$.square = $1;
					  $$.promote = ChessPiece::Empty }

        | file_clarifier 'x' square	{ $$.piece = ChessPiece::Pawn;
					  $$.clarifier = $1;
					  $$.capture = 1;
					  $$.square = $3;
					  $$.promote = ChessPiece::Empty }
					  
	| square '=' piece		{ $$.piece = ChessPiece::Pawn;
					  $$.square = $1;
					  $$.capture = 0;
					  $$.promote = $3; }

	| file_clarifier 'x' square '=' piece	{ $$.piece = ChessPiece::Pawn;
						  $$.clarifier = $1;
						  $$.square = $3;
						  $$.promote = $5;
						  $$.capture = 1; }
	;

   piece: 'N'	{ $$ = ChessPiece::Knight; }
        | 'B'	{ $$ = ChessPiece::Bishop; }
	| 'R'	{ $$ = ChessPiece::Rook; }
	| 'Q'	{ $$ = ChessPiece::Queen; }
	| 'K'	{ $$ = ChessPiece::King; }
	;

   capture: 'x'	{ $$ = 1; }
	|	{ $$ = 0; }
	;

   square: rank file	{ $$.rank = $1; $$.file = $2; }

   clarifier: rank_clarifier
	| file_clarifier
	|	{ $$.rank = 0; $$.file = '\0'; }
	;

   rank_clarifier: rank	{ $$.rank = $1; $$.file = '\0'; }
	;

   file_clarifier: file	{ $$.file = $1; $$.rank = 0; }

   rank: '1'		{ $$ = 1; }
	| '2'		{ $$ = 2; }
	| '3'		{ $$ = 3; }
	| '4'		{ $$ = 4; }
	| '5'		{ $$ = 5; }
	| '6'		{ $$ = 6; }
	| '7'		{ $$ = 7; }
	| '8'		{ $$ = 8; }
	;

   file: 'a'		{ $$ = 1; }
	| 'b'		{ $$ = 2; }
	| 'c'		{ $$ = 3; }
	| 'd'		{ $$ = 4; }
	| 'e'		{ $$ = 5; }
	| 'f'		{ $$ = 6; }
	| 'g'		{ $$ = 7; }
	| 'h'		{ $$ = 8; }
	;
       
%%

int yylex() {
  if ( move_index >= MAX_MOVE_LEN ) {
    return 0;
  } else {
    yylval.ch = move_str[ move_index++ ];
    return CHAR;
  }
}

int yyerror( char *s ) {
  err_msg = s;
  return 1;
}

struct Schessmove *alg_parse( const char *_move_str ) {
  strncpy( move_str, _move_str, 16 );
  move_str[ MAX_MOVE_LEN ] = '\0';
  move_index = 0;
  if ( yyparse() ) {
    return &move_data;
  } else {
    return NULL;
  }
}

