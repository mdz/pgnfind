%{
// -*- C++ -*-

#include "ChessMove.h"
#include "alg_parse_int.h"
#include "pgn_parse_int.h" // Our stuff
#include "pgn_parse.h" // yacc's stuff

#undef yylex
#define yylex pgn_lex

%}

ws [ \t]
digit [0-9]
number {digit}+
alg_move [NBRQKa-hO][0-9a-hx+O-]
other .

%%

{ws} // skip whitespace

{number} yylval.num = atoi(yytext); return NUM;

{alg_move} {
  try {
    yylval.move =  new ChessMove( yytext,
				  ChessMove::Algebraic,
				  game );
  }

  catch ( ChessMove::Exception cme ) {
    switch ( cme ) {
    case ChessMove::InvalidMove:
      yyerror("Invalid move"); return 0;
    case ChessMove::IllegalMove:
      yyerror("Illegal move"); return 0;
    default:
      yyerror("Unrecognized exception from ChessMove"); return 0;
    }
  }

  return MOVE;
}

{other} yylval.num = yytext[0];

%%

