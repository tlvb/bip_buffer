CXX=g++

INC=src

vpath %.cc src testsrc
vpath %.tcc src
vpath %.h src

CXXFLAGS += -Werror -Wall -Wextra -Wpedantic -Wshadow -Wundef -Wpointer-arith -Wcast-align -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -Wcast-qual -Wswitch-default -Wswitch-enum
CXXFLAGS += -I$(INC)

.PHONY: perform_test
perform_test: test_bip_buffer
	@echo
	./test_bip_buffer
	@echo

test_bip_buffer: bip_buffer.tcc test_bip_buffer.cc
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: clean cleaner
clean:
	rm *.o || true
cleaner: clean
	rm test_bip_buffer || true
