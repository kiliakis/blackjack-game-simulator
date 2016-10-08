CXX = g++
CFLAGS=-std=c++11 -O3 -Wall
LDFLAGS=
SOURCES=simulator.cpp blackjack.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=simulator

all: $(EXECUTABLE)

# .cpp.o:
# 	$(CC) $(CFLAGS) $< -o $@

$(EXECUTABLE): $(SOURCES)
	$(CXX) $(LDFLAGS) $(CFLAGS) $^ -o $@


clean:
	rm *~ simulator
