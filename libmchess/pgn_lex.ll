%{
// -*- C++ -*-

#include "ChessMove.h"
#include "alg_parse_int.h"
#include "pgn_parse_int.h" // Our stuff

// Localify yacc stuff

#undef yymaxdepth
#undef yyparse
#undef yylex
#undef yyerror
#undef yylval
#undef yychar
#undef yydebug
#undef yypact
#undef yyr1
#undef yyr2
#undef yydef
#undef yychk
#undef yypgo
#undef yyact
#undef yyexca
#undef yyerrflag
#undef yynerrs
#undef yyps
#undef yypv
#undef yys
#undef yy_yys
#undef yystate
#undef yytmp
#undef yyv
#undef yy_yyv
#undef yyval
#undef yylloc
#undef yyreds
#undef yytoks
#undef yylhs
#undef yylen
#undef yydefred
#undef yydgoto
#undef yysindex
#undef yyrindex
#undef yygindex
#undef yytable
#undef yycheck
#undef yyname
#undef yyrule
#define	yymaxdepth pgn_maxdepth
#define	yyparse	pgn_parse
#define	yylex	pgn_lex
#define	yyerror	pgn_error
#define	yylval	pgn_lval
#define	yychar	pgn_char
#define	yydebug	pgn_debug
#define	yypact	pgn_pact
#define	yyr1	pgn_r1
#define	yyr2	pgn_r2
#define	yydef	pgn_def
#define	yychk	pgn_chk
#define	yypgo	pgn_pgo
#define	yyact	pgn_act
#define	yyexca	pgn_exca
#define yyerrflag pgn_errflag
#define yynerrs	pgn_nerrs
#define	yyps	pgn_ps
#define	yypv	pgn_pv
#define	yys	pgn_s
#define	yy_yys	pgn_yys
#define	yystate	pgn_state
#define	yytmp	pgn_tmp
#define	yyv	pgn_v
#define	yy_yyv	pgn_yyv
#define	yyval	pgn_val
#define	yylloc	pgn_lloc
#define yyreds	pgn_reds
#define yytoks	pgn_toks
#define yylhs	pgn_yylhs
#define yylen	pgn_yylen
#define yydefred pgn_yydefred
#define yydgoto	pgn_yydgoto
#define yysindex pgn_yysindex
#define yyrindex pgn_yyrindex
#define yygindex pgn_yygindex
#define yytable	 pgn_yytable
#define yycheck	 pgn_yycheck
#define yyname   pgn_yyname
#define yyrule   pgn_yyrule

#include "pgn_parse.h" // yacc's stuff

// Localify lex stuff
#undef yywrap
#define yywrap   pgn_wrap

%}

ws [ \t\n]
digit [0-9]
number {digit}+
alg_move [NBRQKa-hO][^ \t\n]+
other .

%%

{ws} // skip whitespace

{number} yylval.num = atoi(yytext); return NUM;

{alg_move} {
  try {
    yylval.move =  new ChessMove( yytext,
				  ChessMove::Algebraic,
				  pgn_game );
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

{other} return yytext[0];

%%

static int yywrap() {
  return 1;
}

void init_pgn_lex( const char *str ) {
  yy_scan_string( str );
}
