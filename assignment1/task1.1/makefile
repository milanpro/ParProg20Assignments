CC=clang
CFLAGS=-I.
OBJ = parsum.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

parsum: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm $(OBJ) parsum

test:
	python3 "validator.task1.1.py"