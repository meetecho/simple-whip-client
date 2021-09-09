CC = gcc
STUFF = $(shell pkg-config --cflags "gstreamer-webrtc-1.0 >= 1.16" "gstreamer-sdp-1.0 >= 1.16" gstreamer-video-1.0 libsoup-2.4 json-glib-1.0) -D_GNU_SOURCE
STUFF_LIBS = $(shell pkg-config --libs "gstreamer-webrtc-1.0 >= 1.16" "gstreamer-sdp-1.0 >= 1.16" gstreamer-video-1.0 libsoup-2.4 json-glib-1.0)
OPTS = -Wall -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wunused #-Werror #-O2
GDB = -g -ggdb
OBJS = src/whip-client.o

all: whip-client

%.o: %.c
	$(CC) $(ASAN) $(STUFF) -fPIC $(GDB) -c $< -o $@ $(OPTS)

whip-client: $(OBJS)
	$(CC) $(GDB) -o whip-client $(OBJS) $(ASAN_LIBS) $(STUFF_LIBS)

clean:
	rm -f whip-client src/*.o
