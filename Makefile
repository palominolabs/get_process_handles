CC = gcc

all: get_process_handles	

clean:
	rm -rf *.o get_process_handles

get_process_handles: main.o
	$(CC) main.o -o get_process_handles

.o:
	$(CC) $< -o $@