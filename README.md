Simple WHIP Client
==================

This is a prototype implementation of a [WHIP client](https://www.ietf.org/archive/id/draft-ietf-wish-whip-01.html), developed by [Meetecho](https://www.meetecho.com). While it was initially conceived to be used mostly for testing with [Simple WHIP Server](https://github.com/meetecho/simple-whip-server) (based on [Janus](https://github.com/meetecho/janus-gateway/)), as a standard WHIP implementation it's supposed to interoperate just as well with other WHIP implementations (check [this presentation](https://github.com/IETF-Hackathon/ietf112-project-presentations/blob/main/ietf112-hackathon-whip.pdf) for some interoperability considerations).

# Building the WHIP client

The main dependencies of this client are:

* [pkg-config](http://www.freedesktop.org/wiki/Software/pkg-config/)
* [GLib](http://library.gnome.org/devel/glib/)
* [libsoup](https://wiki.gnome.org/Projects/libsoup) (~= 2.4)
* [GStreamer](https://gstreamer.freedesktop.org/) (>= 1.16)

Make sure the related development versions of the libraries are installed, before attempting to build the client, as to keep things simple the `Makefile` is actually very raw and naive: it makes use of `pkg-config` to detect where the libraries are installed, but if some are not available it will still try to proceed (and will fail with possibly misleading error messages). All of the libraries should be available in most repos (they definitely are on Fedora, which is what I use everyday, and to my knowledge Ubuntu as well).

> Notice that, while the Makefile assumes a Linux build, at least in principle it should be possible to build it on other platforms as well, as it makes us of cross-platform dependencies. In case you're willing to submit enhancements to the Makefile to build the client on Windows and/or MacOS as well, it would be more than welcome.

Once the dependencies are installed, all you need to do to build the WHIP client is to type:

	make

This will create a `whip-client` executable. Trying to launch that without arguments should display a help section:

```
$ ./whip-client
Usage:
  whip-client [OPTION?] -- Simple WHIP client

Help Options:
  -h, --help               Show help options

Application Options:
  -u, --url                Address of the WHIP endpoint (required)
  -t, --token              Authentication Bearer token to use (optional)
  -A, --audio              GStreamer pipeline to use for audio (optional, required if audio-only)
  -V, --video              GStreamer pipeline to use for video (optional, required if video-only)
  -n, --no-trickle         Don't trickle candidates, but put them in the SDP offer (default: false)
  -f, --follow-link        Use the Link headers returned by the WHIP server to automatically configure STUN/TURN servers to use (default: false)
  -S, --stun-server        STUN server to use, if any (stun://hostname:port)
  -T, --turn-server        TURN server to use, if any; can be called multiple times (turn(s)://username:password@host:port?transport=[udp,tcp])
  -F, --force-turn         In case TURN servers are provided, force using a relay (default: false)
  -l, --log-level          Logging level (0=disable logging, 7=maximum log level; default: 4)
  -o, --disable-colors     Disable colors in the logging (default: enabled)
  -L, --log-timestamps     Enable logging timestamps (default: disabled)
  -H, --http-debugging     HTTP debugging level (none, minimal, headers, body; default: none)
  -e, --eos-sink-name      GStreamer sink name for EOS signal
  -b, --jitter-buffer      Jitter buffer (latency) to use in RTP, in milliseconds (default: -1, use webrtcbin's default)
```

# Testing the WHIP client

The WHIP client requires at least two arguments:

1. the WHIP endpoint to publish to (e.g., an endpoint created in the [Simple WHIP Server](https://github.com/meetecho/simple-whip-server));
2. the partial GStreamer pipeline to use for audio, if audio needs to be sent, and or
3. the partial GStreamer pipeline to use for video, if video needs to be sent.

Audio and video are both optional, but at least one of the two must be enabled.

A simple audio/video example, that assumes the specified endpoint requires the "verysecret" token via Bearer authorization, is the following:

```
./whip-client -u http://localhost:7080/whip/endpoint/abc123 \
	-t verysecret \
	-A "audiotestsrc is-live=true wave=red-noise ! audioconvert ! audioresample ! queue ! opusenc ! rtpopuspay pt=100 ssrc=1 ! queue ! application/x-rtp,media=audio,encoding-name=OPUS,payload=100" \
	-V "videotestsrc is-live=true pattern=ball ! videoconvert ! queue ! vp8enc deadline=1 ! rtpvp8pay pt=96 ssrc=2 ! queue ! application/x-rtp,media=video,encoding-name=VP8,payload=96"
```

In case, e.g., STUN is needed too, the above command can be extended like this:

```
./whip-client -u http://localhost:7080/whip/endpoint/abc123 \
	-t verysecret \
	-A "audiotestsrc is-live=true wave=red-noise ! audioconvert ! audioresample ! queue ! opusenc ! rtpopuspay pt=100 ssrc=1 ! queue ! application/x-rtp,media=audio,encoding-name=OPUS,payload=100" \
	-V "videotestsrc is-live=true pattern=ball ! videoconvert ! queue ! vp8enc deadline=1 ! rtpvp8pay pt=96 ssrc=2 ! queue ! application/x-rtp,media=video,encoding-name=VP8,payload=96" \
	-S stun://stun.l.google.com:19302
```

You can stop the client via CTRL+C, which will automatically send an HTTP DELETE to the WHIP resource to tear down the session.

# Docker

With docker installed, you can build the image automatically and run it for yourself:

```
docker build -t simple-whip-client .
docker run -it --rm -e "URL=http://foo.com/whip/bar" simple-whip-client
```

At the moment, the parameters of the command (e.g., the audio and video pipelines) are hardcoded, other than URL which you can pass in via an env variable.
