CXX = g++
CFLAGS = -O3 -std=c++11
# CFLAGS = -g -std=c++11

EXE = fm
OBJ = cell.o bucket.o partitionMgr.o main.o

all:$(EXE)

$(EXE): $(OBJ)
	$(CXX) $(CFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm *.o $(EXE)                