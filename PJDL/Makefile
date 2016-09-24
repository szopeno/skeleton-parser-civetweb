CPP_FILES := $(wildcard *cpp)
OBJ_FILES := $(CPP_FILES:.cpp=.o)
CFLAGS += -std=c++11 -g

all: $(OBJ_FILES)
	ar rcs libpjdl.a $(OBJ_FILES)
	
%.o: %.cpp
	g++ $(CFLAGS) -c -o $@ $<
