# CC=clang
CFLAGS=-I. -pthread
OBJ = dinner.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

dinner: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm $(OBJ) dinner

test:
	python3 "validator.task1.2.py"