ERRFLAGS = -Werror -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wredundant-decls -Wshadow -Wundef -Wno-unused -Wno-variadic-macros -Wno-parentheses -fdiagnostics-show-option
CXXFLAGS = -lcrypt -fopenmp $(ERRFLAGS)
OBJ = decrypt.o

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

decrypt: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	rm $(OBJ) decrypt

test:
	python3 "validator.task2.2.py"