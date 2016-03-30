CC = gcc
CFLAGS = -Wall
DEPS = ralf.h
OBJ = ralf.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<
	
lsRalF: $(OBJ)
	gcc $(CFLAGS) -o $@ $<
