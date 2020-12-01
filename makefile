CFLAGS = -Wall -ansi -pedantic
TARGET = replacer

.PHONY: clean

$(TARGET).exe: schedule.o replacer.c
	gcc $(CFLAGS) -o $(TARGET).exe schedule.o replacer.c

schedule.o: schedule.c schedule.h
	gcc $(CFLAGS) -c schedule.c

clean:
	$(RM) *.o $(TARGET).exe *~
