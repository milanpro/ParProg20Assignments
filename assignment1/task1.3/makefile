# CC=clang
CFLAGS=-g -pthread -Wall
OBJ = heatmap.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

heatmap: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm $(OBJ) heatmap

test:
	python3 "validator.task1.3.py"