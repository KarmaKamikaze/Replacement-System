CFLAGS = -Wall -ansi -pedantic
TARGET = replacer

.PHONY: clean

$(TARGET).exe: replacer.c
	gcc $(CFLAGS) -o $(TARGET).exe replacer.c


clean:
	$(RM) *.o $(TARGET).exe *~
