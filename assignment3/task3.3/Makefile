TARGET = colorfilter

CFLAGS = -Wall -Wextra -std=c11 -maltivec -mabi=altivec

HDR = bmp.h constants.h
SRC = bmp.c colorfilter.c
OBJ = $(SRC:%.c=%.o)
DEP = $(SRC:%.c=%.d)

DISTFILES = $(HDR) $(SRC) Makefile
ARCHIVE = $(TARGET).tar.gz

all: $(TARGET)

-include $(DEP)

%.d: %.c
	$(CC) -MM $(CFLAGS) $< > $@

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $@
	
run: $(TARGET)
	./$(TARGET) apples.bmp

archive: $(DISTFILES)
	tar cvzf $(ARCHIVE) $(DISTFILES)

clean:
	rm -f $(OBJ) $(DEP)
	rm -f $(TARGET)

distclean: clean
	rm -f $(ARCHIVE) output.bmp

.PHONY: all run archive clean distclean
