%{

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef STDC_HEADERS
#include <string.h>
#endif

#include "ChessGame.h"
#include "alg_parse_int.h"

#define PGN_MAX_LENGTH 2048

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

#define YYERROR_VERBOSE
#define YYDEBUG 1

  static int pgn_index;
  static int pgn_strlen;
  static char pgn_data[ PGN_MAX_LENGTH ];
  static char *err_msg;
  static ChessGame *game;

  static int yyerror(char *);


%}

%union {
  int num;
  ChessMove *move;
}

%token <num> NUM
%token <move> MOVE

%type <game> moveseq

%%

   moveseq: NUM MOVE MOVE	{ }
	| moveseq NUM MOVE MOVE
	;

%%

int yyerror( char *s ) {
  err_msg = s;
  return 1;
}

int pgn_parse( const char *_pgn_data, ChessGame *_game ) {
  game = _game;
  pgn_strlen = strlen( pgn_data );
  strncpy( pgn_data, _pgn_data, pgn_strlen );
  pgn_index = 0;

  //yydebug = 1;

  return yyparse();
}

