CXX=g++

INC=src

vpath %.cc src testsrc
vpath %.h src

CXXFLAGS=-I$(INC)

.PHONY: perform_test
perform_test: test_bip_buffer
	@echo
	./test_bip_buffer
	@echo

test_bip_buffer: test_bip_buffer.cc bip_buffer.o
	$(CXX) $(CXXFLAGS) -o $@ $^

bip_buffer.o: bip_buffer.cc bip_buffer.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: clean cleaner
clean:
	rm *.o || true
cleaner: clean
	rm test_bip_buffer || true
