CXX=gcc
CXX_FLAGS=-std=c17
.PHONY: all clean
TARGETS=test_commands

all: $(TARGETS)


commands.o: commands.c commands.h
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

test_commands.o: test_commands.c commands.o
	$(CXX) $(CXX_FLAGS) -o $@ $^

test_commands: test_commands.o
	./test_commands.o

clean:
	rm -f *~ *.o $(TARGETS)