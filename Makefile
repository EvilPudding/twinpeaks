
all: main.c
	make -C candle SAUCES=resauces
	cc -o twinpeaks main.c -O3 `candle/candle-config --libs`

debug: main.c
	make -C candle debug SAUCES=resauces
	cc -o twinpeaks_debug main.c -g3 `candle/candle-config --debug --libs`

emscripten: main.c
	make -C candle emscripten SAUCES=resauces
	emcc -o index.js main.c `candle/candle-config --emscripten --libs`
	cp candle/index.html .

clean:
	make -C candle clean
	rm -f twinpeaks
	rm -f twinpeaks_debug
	rm -f index.js index.html index.data

.PHONY: clean release
