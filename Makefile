CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = -lm

SRC = q1.c q2.c q3.c q4.c
OBJ = q1.o q2.o q3.o q4.o

OUT = disk_simulator

all: $(OUT)

$(OUT): $(OBJ)
	$(CC) $(OBJ) -o $(OUT) $(LDFLAGS)

q1.o: q1.c
	$(CC) $(CFLAGS) -c q1.c

q2.o: q2.c
	$(CC) $(CFLAGS) -c q2.c

q3.o: q3.c
	$(CC) $(CFLAGS) -c q3.c

q4.o: q4.c
	$(CC) $(CFLAGS) -c q4.c

clean:
	rm -f $(OBJ) $(OUT)

run: $(OUT)
	./$(OUT)
