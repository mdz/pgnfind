CXXFLAGS += -fhandle-exceptions
YACC += -d

noinst_LIBRARIES = libmchess.a

libmchess_a_SOURCES = ChessPiece.cpp ChessPiece.h \
			ChessPosition.cpp ChessPosition.h \
			alg_parse.yy alg_parse_int.h \
			ChessMove.cpp ChessMove.h \
			ChessGame.cpp ChessGame.h \
libmchess_a_LIBADD = @LIBOBJS@
