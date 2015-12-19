CXX = g++
CFLAGS = -Wall -O3
all: blackjack

blackjack: blackjack.cpp
	$(CXX) $(CFLAGS) $< -o $@

clean:
	rm *~ blackjack

