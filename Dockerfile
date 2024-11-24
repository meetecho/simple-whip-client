FROM debian:bookworm AS builder

RUN apt-get update --allow-releaseinfo-change && \
    apt-get install -y --no-install-recommends \
        gdb \
        git \
        build-essential \
        devscripts \
        pkg-config \
        sudo \
        libc6-dev \
        gstreamer1.0-tools \
        gstreamer1.0-nice \
        gstreamer1.0-plugins-bad \
        gstreamer1.0-plugins-ugly \
        gstreamer1.0-plugins-good \
        libglib2.0-dev \
        libgstreamer-plugins-bad1.0-dev \
        libsoup-3.0-dev \
        libjson-glib-dev \
        libgstreamer1.0-dev \
        libgstreamer-plugins-base1.0-dev \
        gstreamer1.0-plugins-base \
        gstreamer1.0-libav && \
        apt-get clean && \
        rm -rf /var/lib/apt/lists/*

WORKDIR /opt

RUN git clone https://github.com/meetecho/simple-whip-client.git && cd simple-whip-client && make

WORKDIR /opt/simple-whip-client

ENV URL=http://localhost:3000/whip/foo

ENTRYPOINT ./whip-client -u $URL -A "audiotestsrc is-live=true wave=red-noise ! audioconvert ! audioresample ! queue ! opusenc ! rtpopuspay pt=100 ssrc=1 ! queue ! application/x-rtp,media=audio,encoding-name=OPUS,payload=100" -V "videotestsrc is-live=true pattern=ball ! videoconvert ! queue ! vp8enc deadline=1 ! rtpvp8pay pt=96 ssrc=2 ! queue ! application/x-rtp,media=video,encoding-name=VP8,payload=96" -S stun://stun.l.google.com:19302
