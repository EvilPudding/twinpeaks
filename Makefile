CC = cc
LD = cc

PLUGINS = $(wildcard *.candle) candle

DIR = build

LIBS = -Wl,-rpath=$(DIR) -L$(DIR)

SRCS = $(wildcard *.c) $(wildcard components/*.c) $(wildcard systems/*.c)

RES = resauces *.vil

OBJS_REL = $(patsubst %.c, $(DIR)/%.o, $(SRCS))
OBJS_DEB = $(patsubst %.c, $(DIR)/%.debug.o, $(SRCS))
OBJS_EMS = $(patsubst %.c, $(DIR)/%.emscripten.o, $(SRCS))

PLUGINS_REL = $(patsubst %, %/$(DIR)/export.a, $(PLUGINS))
PLUGINS_DEB = $(patsubst %, %/$(DIR)/export_debug.a, $(PLUGINS))
PLUGINS_EMS = $(patsubst %, %/$(DIR)/export_emscripten.a, $(PLUGINS))

EMS_OPTS = -s USE_GLFW=3 -s ALLOW_MEMORY_GROWTH=1 -s USE_WEBGL2=1 \
		   -s FULL_ES3=1 -s EMULATE_FUNCTION_POINTER_CASTS=1 \
		   -s WASM=1 -s ASSERTIONS=1 -s SAFE_HEAP=1

CFLAGS = -Wall -I. -Icandle -DUSE_VAO -Wno-unused-function \
		 $(shell sdl2-config --cflags)

CFLAGS_REL = $(CFLAGS) -O3

CFLAGS_EMS = $(CFLAGS) $(EMS_OPTS) -O3

CFLAGS_DEB = $(CFLAGS) -g3

LIBS_REL = $(LIBS) $(PLUGINS_REL) -pthread
LIBS_DEB = $(LIBS) $(PLUGINS_DEB) -pthread
LIBS_EMS = $(LIBS) $(PLUGINS_EMS) $(EMS_OPTS)

##############################################################################

all: init $(DIR)/twinpeaks

release: all
	tar -cf $(DIR)/archive.tar -C $(DIR) $(RES) twinpeaks
	gzip -9f $(DIR)/archive.tar
	cp candle/selfextract.sh $(DIR)/twinpeaks.sh
	cat $(DIR)/archive.tar.gz >> $(DIR)/twinpeaks.sh
	rm $(DIR)/archive.tar.gz
	mkdir -p release
	mv $(DIR)/twinpeaks.sh release/twinpeaks
	chmod +x release/twinpeaks

$(DIR)/twinpeaks: $(OBJS_REL) $(PLUGINS_REL)
	cp -rvu resauces $(DIR)
	cp -vu candle/*.vil $(DIR)
	$(LD) -o $@ $(OBJS_REL) $(LIBS_REL) $(shell cat $(DIR)/deps)

%/$(DIR)/export.a:
	$(MAKE) -C $(patsubst %/$(DIR)/export.a, %, $@) PARENTCFLAGS=
	echo " " >> $(DIR)/deps
	-cat $(patsubst %/$(DIR)/export.a, %/$(DIR)/deps, $@) >> $(DIR)/deps

$(DIR)/%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS_REL)

##############################################################################

debug: init $(DIR)/twinpeaks_debug $(PLUGINS_DEB)

$(DIR)/twinpeaks_debug: $(OBJS_DEB) $(PLUGINS_DEB)
	$(LD) -o $@ $(OBJS_DEB) $(LIBS_DEB) $(shell cat $(DIR)/deps)

%/$(DIR)/export_debug.a:
	$(MAKE) -C $(patsubst %/$(DIR)/export_debug.a, %, $@) debug PARENTCFLAGS=
	echo " " >> $(DIR)/deps
	-cat $(patsubst %/$(DIR)/export_debug.a, %/$(DIR)/deps, $@) >> $(DIR)/deps

$(DIR)/%.debug.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS_DEB)

##############################################################################

emscripten: init $(DIR)/index.js $(PLUGINS_EMS)
	mkdir -p release
	cp candle/index.html build
	rm -f release/index.zip
	zip release/index.zip $(DIR)/index.*

$(DIR)/index.js: $(OBJS_EMS) $(PLUGINS_EMS)
	rm -f $(DIR)/index.data
	# --js-opts 0
	emcc -o $@ $(OBJS_EMS) $(LIBS_EMS) $(shell cat $(DIR)/deps) \
			--preload-file $(DIR)/resauces@resauces \
			--preload-file $(DIR)/default.vil@default.vil \
			--preload-file $(DIR)/transparent.vil@transparent.vil \
			--preload-file $(DIR)/parallax.vil@parallax.vil \
			--preload-file $(DIR)/decal.vil@decal.vil

%/$(DIR)/export_emscripten.a:
	$(MAKE) -C $(patsubst %/$(DIR)/export_emscripten.a, %, $@) emscripten PARENTCFLAGS=
	echo " " >> $(DIR)/deps
	-cat $(patsubst %/$(DIR)/export_emscripten.a, %/$(DIR)/deps, $@) >> $(DIR)/deps

$(DIR)/%.emscripten.o: %.c
	emcc -o $@ -c $< $(CFLAGS_EMS)

##############################################################################

init:
	# git submodule update
	rm -f $(PLUGINS_DEB)
	rm -f $(PLUGINS_EMS)
	rm -f $(PLUGINS_REL)
	mkdir -p $(DIR)
	mkdir -p $(DIR)/components
	rm -f $(DIR)/deps
	touch $(DIR)/deps
	cp -rvu resauces $(DIR)
	cp -vu candle/*.vil $(DIR)

##############################################################################

run: all
	rm -fr ~/.nv
	cp -rvu resauces $(DIR)
	$(DIR)/twinpeaks

gdb: debug
	cp -rvu resauces $(DIR)
	gdb $(DIR)/twinpeaks_debug

valgrind: debug
	cp -rvu resauces $(DIR)
	valgrind --log-fd=1 --suppressions=val_sup $(DIR)/twinpeaks_debug | tee val_log | less

callgrind: debug
	gprof2dot.py --format=callgrind --output=out.dot callgrind.out.8539 
	dot -Tpng out.dot -o graph.png
		
clean:
	rm -fr $(DIR)
	rm -fr release
	$(MAKE) -C candle clean

.PHONY: init clean release
