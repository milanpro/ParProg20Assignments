ERRFLAGS = -Werror -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wredundant-decls -Wshadow -Wundef -Wno-unused -Wno-variadic-macros -Wno-parentheses -fdiagnostics-show-option -O3
CFLAGS = $(ERRFLAGS)
CXXFLAGS = $(CFLAGS) -pthread
a_OBJS = experiment_a.o
b_fit_OBJS = experiment_b_flt.o
b_fix_OBJS = experiment_b_fix.o
c_OBJS = experiment_c.o
d_OBJS = experiment_d.o

all_TARGETS = experiment_a experiment_b_flt experiment_b_fix experiment_c experiment_d

all: $(all_TARGETS)

experiment_a: $(a_OBJS)
	$(CC) -o $@ $^ $(CFLAGS)
experiment_b_flt: $(b_fit_OBJS)
	$(CC) -o $@ $^ $(CFLAGS)
experiment_b_fix: $(b_fix_OBJS)
	$(CC) -o $@ $^ $(CFLAGS)
experiment_c: $(c_OBJS)
	$(CC) -o $@ $^ $(CFLAGS)
experiment_d: $(d_OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	rm $(a_OBJS) $(b_fit_OBJS) $(b_fix_OBJS) $(c_OBJS) $(d_OBJS) $(all_TARGETS)
