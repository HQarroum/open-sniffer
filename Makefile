CC = gcc

APP = sniffy

# Holds a list of every source file in the `dissectors` folder.
DISSECTORS_SRC = $(shell find dissectors/ -name '*.c')

SRC = app.c \
	io.c \
	sync_reader.c \
	main.c \
	osi.c \
	utils/inaddr.c \
	$(DISSECTORS_SRC)

CFLAGS = -std=c99 -D_BSD_SOURCE -W -Wall -Werror -O2

LDFLAGS = -L./circular-linked-list -lcircular-linked-list

OBJ = $(SRC:.c=.o)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

all: $(OBJ)
	$(CC) -o $(APP) $^ $(LDFLAGS)

re: fclean all

clean:
	$(shell find . -name '*~' -exec rm -r {} \; -o -name '*.o' -exec rm -r {} \;)

fclean: clean
	rm -f $(APP)

.PHONY: clean fclean re
