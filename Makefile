CXX=gcc
CXX_FLAGS=-g -std=c17
VALGRIND=valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes
.PHONY: all clean
TARGETS=test_commands

all: $(TARGETS)


commands.o: commands.c commands.h
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

test_commands.o: test_commands.c commands.o
	$(CXX) $(CXX_FLAGS) -o $@ $^

test_commands: test_commands.o
	./test_commands.o

leak_test_commands: test_commands.o
	$(VALGRIND) ./test_commands.o

clean:
	rm -f *~ *.o $(TARGETS)