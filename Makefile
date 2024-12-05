CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = Q1
DATAFILE = q1-10mil.txt

all: $(TARGET) output_fread.txt output_mmap.txt

$(TARGET): Q1.c
	$(CC) $(CFLAGS) -o $(TARGET) Q1.c

output_fread.txt: $(TARGET)
	./$(TARGET) -fread > output_fread.txt

output_mmap.txt: $(TARGET)
	./$(TARGET) -mmap > output_mmap.txt

clean:
	rm -f $(TARGET) output_fread.txt output_mmap.txt

