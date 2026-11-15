CC      = gcc
CFLAGS  = -Wall -Wextra -O2 $(shell pkg-config --cflags openssl)
LDFLAGS = $(shell pkg-config --libs openssl)

SRC = src/gluon.c
BIN = bin/gluon

all: $(BIN)

$(BIN): $(SRC)
	@mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) -o $(BIN) $(LDFLAGS)

clean:
	rm -f $(BIN)

install: $(BIN)
	install -Dm755 $(BIN) /usr/local/bin/gluon

uninstall:
	rm -f /usr/local/bin/gluon

.PHONY: all clean install uninstall