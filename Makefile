PRO=downloadlist
SRC=$(wildcard *.cpp)
OBJ=$(patsubst %.cpp,%.o,$(SRC))
CXXFLAGS+=-O2 -std=c++11
LIBS+=-lcpr -lcurl -ljsoncpp
LDFLAGS+=

all:$(PRO)
	
$(PRO):$(OBJ)
	g++ $(CXXFLAGS) $(LIBS) $(LDFLAGS) -o $@ $^

test:
	./${PRO} --conf=downloadlist.json --dir=download

clean:
	rm -f $(PRO)
	rm -f $(OBJ)
