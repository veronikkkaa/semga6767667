CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall

TARGET = lab3
SRC = main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
