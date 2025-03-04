CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude  # Add -Iinclude to find header files
LIBS = -lncurses

SRC = src/main.cpp src/cpu.cpp src/memory.cpp src/process.cpp src/display.cpp
OBJ = $(SRC:.cpp=.o)

TARGET = system_monitor

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
