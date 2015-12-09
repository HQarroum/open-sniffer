CC = gcc

APP = sniffy

SRC_DIR = ./src

# Holds a list of every source file in the `dissectors` folder.
DISSECTORS_SRC = $(shell find ./src/dissectors/ -name '*.c')

SRC = $(SRC_DIR)/app.c \
	$(SRC_DIR)/io.c \
	$(SRC_DIR)/sync_reader.c \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/osi.c \
	$(SRC_DIR)/utils/inaddr.c \
	$(DISSECTORS_SRC)

CFLAGS = -I./includes -I./circular-linked-list -std=c99 -D_BSD_SOURCE -W -Wall -Werror -O2

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
