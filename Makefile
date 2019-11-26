CC = gcc

# Optimization flags

# CFLAGS = -DLINUX
%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS)
OBJ = server.o routines.o common.o
COBJ = client.o common.o

all: server	client

debug: CFLAGS += -g -DDEBUG
debug: all

server: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

client: $(COBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(OBJ) $(COBJ) server client
