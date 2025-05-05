.PHONY: clean all
CXX = g++
CXXFLAGS = -I../include -std=c++11 -Wall -Wextra -O2
LDFLAGS = -lmysqlcppconn 
SRC = $(wildcard *.cpp)
OBJ = $(SRC:%.cpp=../build/%.o)
# BASENAMES = $(basename $(OBJ))
TARGET = ../bin/main

all: $(TARGET)
	echo $(OBJ)
	
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) $(LDFLAGS) -o $@

../build/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f ../build/*.o $(TARGET)
	rm -f *.o main
