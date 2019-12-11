SOURCES := main.cpp
# Objs are all the sources, with .cpp replaced by .o
OBJS := $(SOURCES:.cpp=.o)

all: main
# Compile the binary 't' by calling the compiler with cflags, lflags, and any libs (if defined) and the list of objects.

main: main.cpp
	g++ main.cpp -o main
clean:
	$(RM) *.o *.d main
