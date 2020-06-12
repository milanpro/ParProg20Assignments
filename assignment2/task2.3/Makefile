ERRFLAGS = -Werror -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wredundant-decls -Wshadow -Wundef -Wno-unused -Wno-variadic-macros -Wno-parentheses -fdiagnostics-show-option
CXXFLAGS = -g -lcrypto -fopenmp $(ERRFLAGS)
OBJ = hoi.o

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

hoi: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	rm $(OBJ) hoi

test:
	python3 "validator.task2.3.py"