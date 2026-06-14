CC = gcc
CFLAGS = -Wall -Wextra -g

CFILE = main.c command.c student.c file_io.c

ADMIN_TARGET = admin_shell
CLIENT_TARGET = client_shell

all: admin client


admin: $(SRCS)
	$(CC) $(CFLAGS) -DADMIN_MODE $(CFILE) -o $(ADMIN_TARGET)


client: $(SRCS)
	$(CC) $(CFLAGS) -DCLIENT_MODE $(CFILE) -o $(CLIENT_TARGET)


clean:
	rm -f $(ADMIN_TARGET) $(CLIENT_TARGET)
	rm -rf *.dSYM

.PHONY: all admin client clean
