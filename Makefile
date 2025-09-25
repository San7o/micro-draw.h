## --- Settings ---

CFLAGS=-Wall -Werror -Wpedantic -ggdb -std=c99
LDFLAGS=-lX11 -lXrandr
CC=gcc

OUT_NAME=example
OBJ=example.o

TEST_BINS=test/fill_rect_test\
          test/fill_circle_test\
          test/fill_triangle_test\
          test/line_test\
          test/to_ppm_test\
          test/game_of_life_test

## --- Commands ---

# --- Targets ---

all: $(OUT_NAME)

run: $(OUT_NAME)
	chmod +x $(OUT_NAME)
	./$(OUT_NAME)

test: $(TEST_BINS)

$(OUT_NAME): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) $(CFLAGS) -o $(OUT_NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%: %.c
	$(CC) $< -o $@ $(LDFLAGS) $(CFLAGS)

clean:
	rm $(OBJ) 2>/dev/null || :

distclean:
	rm $(OUT_NAME) 2>/dev/null || :
