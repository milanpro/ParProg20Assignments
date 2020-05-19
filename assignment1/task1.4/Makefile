ERRFLAGS = -Werror -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wredundant-decls -Wshadow -Wundef -Wno-unused -Wno-variadic-macros -Wno-parentheses -fdiagnostics-show-option
CFLAGS = -g -pthread $(ERRFLAGS)
OBJ = mcpi.o 

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

mcpi: $(OBJ)
	$(CXX) -o $@ $^ $(CFLAGS)

clean:
	rm $(OBJ) mcpi

test:
	python3 "validator.task1.3.py"