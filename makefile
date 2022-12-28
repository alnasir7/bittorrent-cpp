# name of the program to build
#
PROG=bittorrent

# Remove -DNDEBUG during development if assert(3) is used
#
override CPPFLAGS += -DNDEBUG -DPROMPT=$(PROMPT)

# Replace -O1 with -g for a debug version during development
#
CFLAGS = -Wall -Werror -g

SRCS = $(wildcard *.cpp) $(wildcard */*.cpp)
OBJS = $(SRCS:.c=.o)
LDFLAGS = -L/Users/mabaker/Penn/cpp/libs/cpr/build/lib
LDLIBS = -lcpr

CC = clang++ -std=c++20 $(LDFLAGS) $(LDLIBS)

.PHONY : clean

$(PROG) : $(OBJS)
	$(CC) -o $@ $^ 

clean :
	$(RM) $(OBJS) $(PROG)