PROG = minicamelot
CC = g++
CPPFLAGS = -g -Wall -std=c++11
OBJS = main.o Board.o CPU.o Game.o Human.o Player.o

$(PROG) : $(OBJS)
	$(CC) $(OBJS) -o $(PROG)

main.o : 
	$(CC) $(CPPFLAGS) -c main.cpp

Board.o : Board.h
	$(CC) $(CPPFLAGS) -c Board.cpp

CPU.o : CPU.h
	$(CC) $(CPPFLAGS) -c CPU.cpp

Game.o : Game.h
	$(CC) $(CPPFLAGS) -c Game.cpp

Human.o : Human.h
	$(CC) $(CPPFLAGS) -c Human.cpp

Player.o : Player.h
	$(CC) $(CPPFLAGS) -c Player.cpp

clean :
	$(RM) $(PROG) $(OBJS)
