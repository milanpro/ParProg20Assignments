ERRFLAGS = -Werror -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-include-dirs -Wredundant-decls -Wshadow -Wundef -Wno-unused -Wno-variadic-macros -Wno-parentheses -fdiagnostics-show-option
CFLAGS = -O3 $(ERRFLAGS)
OBJ = mpiavg.o
MPICC?=mpic++


%.o: %.cpp $(DEPS)
	$(MPICC) -c -o $@ $< $(CFLAGS)

mpiavg: $(OBJ)
	$(MPICC) -o $@ $^ $(CFLAGS)

clean:
	rm $(OBJ) mpiavg

test:
	python3 "validator.task5.1.py"