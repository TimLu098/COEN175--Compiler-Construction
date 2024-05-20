CXX		= c++ -std=c++17
CXXFLAGS	= -g -Wall
EXTRAS		= lexer.cpp
OBJS		= Scope.o Symbol.o Type.o checker.o lexer.o parser.o string.o
PROG		= scc


all:		$(PROG)

$(PROG):	$(EXTRAS) $(OBJS)
		$(CXX) -o $(PROG) $(OBJS)

clean:;		$(RM) $(PROG) core *.o

clobber:;	$(RM) $(EXTRAS) $(PROG) core *.o

lexer.cpp:	lexer.l
		$(LEX) $(LFLAGS) -t lexer.l > lexer.cpp
