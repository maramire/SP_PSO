SOURCES := main.cpp parallel_main.cpp
# Objs are all the sources, with .cpp replaced by .o
OBJS := $(SOURCES:.cpp=.o)

all: main parallel_main
# Compile the binary 't' by calling the compiler with cflags, lflags, and any libs (if defined) and the list of objects.

main: main.cpp
	g++ main.cpp -o main 

parallel_main: parallel_main.cpp
	g++ parallel_main.cpp -o parallel_main -lpthread 

clean:
	$(RM) *.o *.d main parallel_main
