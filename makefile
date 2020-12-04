CFLAGS = -Wall -ansi -pedantic
TARGET = replacer

.PHONY: clean

$(TARGET).exe: employee.o schedule.o positions.o utility.o replacer.c 
	gcc $(CFLAGS) -o $(TARGET).exe employee.o \
	schedule.o positions.o utility.o replacer.c

employee.o: employee.c employee.h
	gcc $(CFLAGS) -c employee.c

schedule.o: schedule.c schedule.h
	gcc $(CFLAGS) -c schedule.c

positions.o: positions.c positions.h
	gcc $(CFLAGS) -c positions.c

utility.o: utility.c utility.h
	gcc $(CFLAGS) -c utility.c

clean:
	$(RM) *.o $(TARGET).exe *~
