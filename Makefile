CC = g++
CFLAGS = -Wall -g

# ****************************************************
# Targets needed to bring the executable up to date

dictionary: dictionary.o classes.o char_stack.o
	$(CC) $(CFLAGS) -o dictionary dictionary.o classes.o char_stack.o -lstdc++fs

dictionary.o: dictionary.cpp classes.h char_stack.h
	$(CC) $(CFLAGS) -c dictionary.cpp -o dictionary.o

classes.o: classes.h

char_stack.o: char_stack.h

clean :
	-rm *.o $(objects)
