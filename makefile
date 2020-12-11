CFLAGS = -Wall -ansi -pedantic -lm
TARGET = replacer

.PHONY: clean

$(TARGET).exe: employee.o schedule.o positions.o utility.o rules.o replacer.c 
	gcc $(CFLAGS) -o $(TARGET).exe employee.o \
	schedule.o positions.o utility.o rules.o replacer.c

employee.o: employee.c employee.h
	gcc $(CFLAGS) -c employee.c

schedule.o: schedule.c schedule.h
	gcc $(CFLAGS) -c schedule.c

positions.o: positions.c positions.h
	gcc $(CFLAGS) -c positions.c

utility.o: utility.c utility.h
	gcc $(CFLAGS) -c utility.c

rules.o: rules.c rules.h
	gcc $(CFLAGS) -c rules.c

clean:
	$(RM) *.o $(TARGET).exe *~
