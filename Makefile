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
          test/game_of_life_test\
          test/mandelbrot_test\
          test/upscale_nn_test

EMCC_FLAGS=-sEXPORTED_RUNTIME_METHODS=["HEAPU8","stringToNewUTF8"]\
           -sEXPORT_ALL=1\
           --shell-file wasm/wasm_template.html
EMCC_ENV=/home/$(shell whoami)/sources/emsdk/emsdk_env.sh

### Website
INDEX_FILE=README
HTML_INTRO=html/intro.html
HTML_OUTRO=html/outro.html
OUT_INDEX_FILE=html/index.html
TMP_FILE=.tmp
PANDOC_FLAGS=--standalone

## --- Commands ---

# --- Targets ---

all: $(OUT_NAME)

run: $(OUT_NAME)
	chmod +x $(OUT_NAME)
	./$(OUT_NAME)

test: $(TEST_BINS)

wasm: test/game_of_life_test.c game-of-life.h
	source ${EMCC_ENV} && emcc test/game_of_life_test.c -o html/game-of-life.html $(EMCC_FLAGS)

.PHONY: html
html: wasm
	pandoc ${INDEX_FILE} -o ${TMP_FILE} ${PANDOC_FLAGS}
	cp ${HTML_INTRO} ${OUT_INDEX_FILE}
	cat ${TMP_FILE} >> ${OUT_INDEX_FILE}
	cat html/game-of-life.html >> ${OUT_INDEX_FILE}
	cat ${HTML_OUTRO} >> ${OUT_INDEX_FILE}
	rm ${TMP_FILE}

$(OUT_NAME): $(OBJ) game-of-life.h
	$(CC) $(OBJ) $(LDFLAGS) $(CFLAGS) -o $(OUT_NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%: %.c
	$(CC) $< -o $@ $(LDFLAGS) $(CFLAGS)

clean:
	rm $(OBJ) $(TEST_BINS) 2>/dev/null || :

distclean:
	rm $(OUT_NAME) 2>/dev/null || :
