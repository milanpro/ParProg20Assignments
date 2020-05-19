ERRFLAGS = -Werror -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wredundant-decls -Wshadow -Wundef -Wno-unused -Wno-variadic-macros -Wno-parentheses -fdiagnostics-show-option
CFLAGS = -g -pthread $(ERRFLAGS)
OBJ = heatmap.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

heatmap: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm $(OBJ) heatmap

test:
	python3 "validator.task1.3.py"