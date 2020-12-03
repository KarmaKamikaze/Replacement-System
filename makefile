CFLAGS = -Wall -ansi -pedantic
TARGET = replacer

.PHONY: clean

$(TARGET).exe: employee.o replacer.c 
	gcc $(CFLAGS) -o $(TARGET).exe employee.o \
	replacer.c

replacer.o: replacer.c replacer.h
	gcc $(CFLAGS) -c replacer.c

employee.o: employee.c employee.h
	gcc $(CFLAGS) -c employee.c

clean:
	$(RM) *.o $(TARGET).exe *~
