CXXFLAGS = -std=c++11 -Ofast -Wall -Werror
#CXXFLAGS = -std=c++11 -O0 -Wall -Werror -g -fsanitize=address -fno-omit-frame-pointer
LDFLAGS = -lSDL2
OS = $(shell uname -s)
SRC = $(wildcard *.cpp)
OBJECTS = $(patsubst %.cpp, %.o, $(SRC))
HEADERS = $(patsubst %.cpp, %.h, $(SRC))
DEPS = $(patsubst %.cpp, %.d, $(SRC))
ELFNAME = raycaster

ifeq ($(OS), Darwin)
	CXX = clang++
	#CXX = ~/Desktop/build/Debug+Asserts/bin/clang++
endif
ifeq ($(OS), Linux)
	LDFLAGS += -lpthread
	CXX = g++
endif

all: $(ELFNAME)

$(ELFNAME): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o$@ $^ $(LDFLAGS) 

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -MMD -MP $< -o $@

-include $(DEPS)

clean:
	rm -f *.d *.o $(ELFNAME)
