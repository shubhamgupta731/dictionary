OBJ=main.o\
	 Subject.o \
	 Observer.o \
	 Node.o

CXX=clang++

#OPTIONS=-g -fsanitize=address
OPTIONS= -fcolor-diagnostics -O0 -g -stdlib=libstdc++ -fno-omit-frame-pointer #-fsanitize=address,bool,enum,integer-divide-by-zero,null,return,shift,signed-integer-overflow,unreachable,,vla-bound
LIBS=  -L. -lloki -ltcmalloc #-lc++abi -fsanitize=address,bool,enum,integer-divide-by-zero,null,return,shift,signed-integer-overflow,unreachable,vla-bound

all: $(OBJ)
	$(CXX) $(OPTIONS) -o a.out $(OBJ) $(LIBS)

clean:
	rm -f *.o
	rm -f a.out
main.o: main.cxx
	$(CXX) $(OPTIONS) -c main.cxx

Subject.o: Subject.cxx
	$(CXX) $(OPTIONS) -c Subject.cxx

Observer.o: Observer.cxx
	$(CXX) $(OPTIONS) -c Observer.cxx -o Observer.o

Node.o: Node.cxx
	$(CXX) $(OPTIONS) -c Node.cxx -o Node.o
