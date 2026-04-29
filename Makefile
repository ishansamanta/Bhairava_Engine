CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS = -lssl -lcrypto

SRC = src/bhairava_engine.c
TEST = test/main.c

TARGET = bhairav

# ---------------- BUILD MAIN TOOL ----------------
all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) $(TEST) $(SRC) -o $(TARGET) $(LDFLAGS)

# ---------------- CLEAN ----------------
clean:
	rm -f $(TARGET)

# ---------------- RUN TEST ----------------
run: all
	./$(TARGET)

# ---------------- INSTALL LIBRARY ----------------
install:
	sudo cp include/bhairava_engine.h /usr/local/include/
	sudo cp $(SRC) /usr/local/lib/libbhairava_engine.a || true

# ---------------- UNINSTALL ----------------
uninstall:
	sudo rm -f /usr/local/include/bhairava_engine.h
	sudo rm -f /usr/local/lib/libbhairava_engine.a
