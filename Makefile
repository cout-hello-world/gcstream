PREFIX = /usr/local/
LDLIBS = -lcurl
CFLAGS = -std=c90 -pedantic -Wall -Wextra -Werror
OBJECTS = gcstream.o
EXECUTABLE = gcstream

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LDLIBS)

install:
	cp $(EXECUTABLE) $(PREFIX)/bin/

clean:
	$(RM) $(OBJECTS) $(EXECUTABLE)
