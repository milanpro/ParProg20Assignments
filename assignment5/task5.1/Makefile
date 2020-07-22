ERRFLAGS = -Werror -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-include-dirs -Wredundant-decls -Wshadow -Wundef -Wno-unused -Wno-variadic-macros -Wno-parentheses -fdiagnostics-show-option
CFLAGS = -O3 $(ERRFLAGS)
OBJ = heatmap.o csv.o
MPICC?=mpicc


%.o: %.c $(DEPS)
	$(MPICC) -c -o $@ $< $(CFLAGS)

heatmap: $(OBJ)
	$(MPICC) -o $@ $^ $(CFLAGS)

clean:
	rm $(OBJ) heatmap

test:
	python3 "validator.task5.1.py"