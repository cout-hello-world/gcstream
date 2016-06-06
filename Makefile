PREFIX = /usr/local/
LDLIBS = -lcurl
CFLAGS = -std=gnu90 -pedantic -Wall -Wextra -Werror
OBJECTS = gcstream.o str_utils.o
EXECUTABLE = gcstream

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LDLIBS)

gcstream.o str_utils.o: str_utils.h

install:
	cp $(EXECUTABLE) $(PREFIX)/bin/

clean:
	$(RM) $(OBJECTS) $(EXECUTABLE)
