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
#define YYDEBUG 1

#define MAX_MOVE_LEN 16
  
  static int move_index;
  static char move_str[MAX_MOVE_LEN + 1] = "";
  static struct Sclarifier clar_none = { 0,0 };
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

%token <num> INT

%type <num> rank file maybe_capture
%type <piece> piece maybe_promote
%type <square> square
%type <move> input init move_check move

%%

   input: init move_check { move_data = $1; move_data = $2; }
	;

   init: /* empty */	{ $$.piece = ChessPiece::Empty;
			  $$.clarifier.rank = 0;
			  $$.clarifier.file = 0;
			  $$.square.rank = 0;
			  $$.square.file = 0;
			  $$.capture = 0;
			  $$.check = 0;
			  $$.promote = ChessPiece::Empty;
			  $$.castling = ChessPosition::None; }
	;

   move_check: move	{ $$ = $1; }
	| move '+'	{ $$ = $1; $$.check = 1; }
	;

   move: piece 'x' square	        { $$.piece = $1;
					  $$.capture = 1;
					  $$.square = $3; }
	| piece square			{ $$.piece = $1;
					  $$.square = $2; }

	| piece file maybe_capture square { $$.piece = $1;
					    $$.capture = $2;
					    $$.clarifier.file =  $3;
					    $$.square = $4; }

	| piece rank maybe_capture square { $$.piece = $1;
					    $$.capture = $2;
					    $$.clarifier.rank = $3;
					    $$.square = $4; }

	| square maybe_promote		{ $$.piece = ChessPiece::Pawn;
					  $$.square = $1;
					  $$.promote = $2; }

        | file 'x' square maybe_promote	{ $$.piece = ChessPiece::Pawn;
					  $$.clarifier.file = $1;
					  $$.capture = 1;
					  $$.square = $3;
					  $$.promote = $4; }

	| 'O' '-' 'O'			{ $$.piece = ChessPiece::King;
					  $$.castling =
						ChessPosition::Kingside; }

	| 'O' '-' 'O' '-' 'O'		{ $$.piece = ChessPiece::King;
					  $$.castling =
						ChessPosition::Queenside; }
					  
	;

   piece: 'N'	{ $$ = ChessPiece::Knight; }
        | 'B'	{ $$ = ChessPiece::Bishop; }
	| 'R'	{ $$ = ChessPiece::Rook; }
	| 'Q'	{ $$ = ChessPiece::Queen; }
	| 'K'	{ $$ = ChessPiece::King; }
	;

   maybe_capture: 'x' { $$ = 1; }
	| /* empty */ { $$ = 0; }

   square: file rank	{ $$.file = $1; $$.rank = $2; }

   maybe_promote: /* empty */ { $$ = ChessPiece::Empty; }
	| '=' piece	      { $$ = $2; };

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
    return move_str[ move_index++ ];
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

  //yydebug = 1;

  if ( yyparse() == 0 ) {
    return &move_data;
  } else {
    return NULL;
  }
}

