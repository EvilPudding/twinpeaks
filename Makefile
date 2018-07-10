CC = cc
LD = cc

PLUGINS = candle

DIR = build

LIBS = -Lcandle/build $(shell sdl2-config --libs) -lglut -lGLU -lm -lGL -lGLEW \
	   -lpng -lassimp -llua

SRCS = $(wildcard *.c) $(wildcard components/*.c) $(wildcard systems/*.c)

OBJS_REL = $(patsubst %.c, $(DIR)/%.o, $(SRCS))
OBJS_DEB = $(patsubst %.c, $(DIR)/%.debug.o, $(SRCS))

PLUGINS_REL = $(patsubst %, %/build/export.a, $(PLUGINS))
PLUGINS_DEB = $(patsubst %, %/build/export_debug.a, $(PLUGINS))

LIBS_REL = $(LIBS) $(PLUGINS_REL)
LIBS_DEB = $(LIBS) $(PLUGINS_DEB)

CFLAGS = -Wall -I. -Icandle -DUSE_VAO \
		 $(shell sdl2-config --cflags)

CFLAGS_REL = $(CFLAGS) -O2

CFLAGS_DEB = $(CFLAGS) -g3


all: init $(DIR)/twin_peaks
	rm $(PLUGINS_REL)
	cp -rvu resauces $(DIR)

$(DIR)/twin_peaks: $(OBJS_REL) $(PLUGINS_REL)
	$(LD) -o $@ $(OBJS_REL) $(LIBS_REL)

%/build/export.a:
	$(MAKE) -C $(patsubst %/build/export.a, %, $@) PARENTCFLAGS=

$(DIR)/%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS_REL)

##############################################################################

debug: init $(DIR)/twin_peaks_debug
	cp -rvu resauces $(DIR)

$(DIR)/twin_peaks_debug: $(OBJS_DEB) $(PLUGINS_DEB)
	$(LD) -o $@ $(OBJS_DEB) $(LIBS_DEB)

%/build/export_debug.a:
	$(MAKE) -C $(patsubst %/build/export_debug.a, %, $@) debug PARENTCFLAGS=

$(DIR)/%.debug.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS_DEB)

##############################################################################

init:
	mkdir -p $(DIR)
	mkdir -p $(DIR)/components

##############################################################################

run: all
	cp -rvu resauces $(DIR)
	$(DIR)/twin_peaks

gdb: debug
	cp -rvu resauces $(DIR)
	gdb $(DIR)/twin_peaks_debug

valgrind: debug
	cp -rvu resauces $(DIR)
	valgrind --log-fd=1 --suppressions=val_sup $(DIR)/twin_peaks_debug | tee val_log | less
		
clean:
	rm -r $(DIR)
	$(MAKE) -C candle clean
