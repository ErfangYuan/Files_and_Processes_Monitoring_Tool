CC = gcc
CFLAGS = -Wall -g -Werror
OBJ = main.o showtables.o create_array.o
TARGET = showFDtables

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c showtables.h common.h
	$(CC) $(CFLAGS) -c $<
	
clean:
	rm -f *.o $(TARGET)