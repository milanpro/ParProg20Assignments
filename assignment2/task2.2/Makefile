ERRFLAGS = -Werror -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wredundant-decls -Wshadow -Wundef -Wno-unused -Wno-variadic-macros -Wno-parentheses -fdiagnostics-show-option
CFLAGS = -g -fopenmp $(ERRFLAGS)
OBJ = decrypt.o

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

decrypt: $(OBJ)
	$(CXX) -o $@ $^ $(CFLAGS)

clean:
	rm $(OBJ) decrypt

test:
	python3 "validator.task2.2.py"