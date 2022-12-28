# name of the program to build
#
PROG=bittorrent

# Remove -DNDEBUG during development if assert(3) is used
#
override CPPFLAGS += -DNDEBUG -DPROMPT=$(PROMPT)

CC = clang++ -std=c++20

# Replace -O1 with -g for a debug version during development
#
CFLAGS = -Wall -Werror -g

SRCS = $(wildcard *.cpp) $(wildcard */*.cpp)
OBJS = $(SRCS:.c=.o)

.PHONY : clean

$(PROG) : $(OBJS)
	$(CC) -o $@ $^ 

clean :
	$(RM) $(OBJS) $(PROG)