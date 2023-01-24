CC 		= g++
CFLAGS 	= -g -Wall
SRCS	= main.cpp error.cpp table.cpp parser.cpp lexer.cpp
OBJS 	= $(SRCS:%.cpp=%.o)
PROGRAM = dc

all:		$(PROGRAM)
.cpp.o:
			$(CC) -c $< -o $@
$(PROGRAM): $(OBJS)
			$(CC) $(CFLAGS) $(OBJS) -o $(PROGRAM)
clean:
			rm -f *.o $(PROGRAM)