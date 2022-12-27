CC= 	 gcc
CFLAGS=  -O2 -Wall
LDFLAGS= -lm

BIN =	  xterm2ansi
SOURCES = xterm2ansi.c utils.c
OBJECTS = $(SOURCES:.c=.o)

$(BIN): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: test
test:
	cd tests && bash ./run_tests.sh

.PHONY: install
install: $(BIN)
	install -d $(DESTDIR)/usr/local/bin
	install -m 0755 $(BIN) $(DESTDIR)/usr/local/bin

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)/usr/local/bin/$(BIN)

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(BIN)

