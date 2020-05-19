ERRFLAGS = -Werror -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wredundant-decls -Wshadow -Wstrict-overflow=5 -Wundef -Wno-unused -Wno-variadic-macros -Wno-parentheses -fdiagnostics-show-option
CFLAGS = -g -pthread $(ERRFLAGS)
OBJ = parsum.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

parsum: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm $(OBJ) parsum

test:
	python3 "validator.task1.1.py"