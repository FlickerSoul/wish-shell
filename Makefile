CXX=gcc
CXX_FLAGS=-g -std=c17
VALGRIND=valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes
.PHONY: all clean
TARGETS=wish

all: $(TARGETS)


commands.o: commands.c commands.h
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

parallel_commands.o: parallel_commands.c parallel_commands.h
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

wish_utils.o: wish_utils.c wish_utils.h
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

test_utils.o: test_utils.c test_utils.h
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

test_commands.o: test_commands.c commands.o test_utils.o
	$(CXX) $(CXX_FLAGS) -o $@ $^

test_parallel_commands.o: test_parallel_commands.c parallel_commands.o commands.o test_utils.o
	$(CXX) $(CXX_FLAGS) -o $@ $^

test_wish_utils.o: test_wish_utils.c wish_utils.o parallel_commands.o commands.o test_utils.o
	$(CXX) $(CXX_FLAGS) -o $@ $^

test_commands: test_commands.o
	./test_commands.o

test_parallel_commands: test_parallel_commands.o
	./test_parallel_commands.o

test_wish_utils: test_wish_utils.o
	./test_wish_utils.o < wish_utils_test.in
	rm a.txt

wish: wish.c wish_utils.o commands.o parallel_commands.o
	$(CXX) $(CXX_FLAGS) -o $@ $^

leak_test: test_commands.o test_parallel_commands.o test_wish_utils.o
	$(VALGRIND) ./test_commands.o 
	$(VALGRIND) ./test_parallel_commands.o 
	$(VALGRIND) ./test_wish_utils.o < wish_utils_test.in
	rm a.txt

test_wish: wish
	./test-wish.sh
	rm -rf tests-out

run: wish
	./wish

vrun: wish
	$(VALGRIND) ./wish

clean:
	rm -rf *~ *.o $(TARGETS) *.dSYM