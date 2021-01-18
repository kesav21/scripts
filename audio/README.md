
# PulseTest

PulseTest is a collection of scripts I use to control anything relating to audio on my machines.
The daemon, written in C, interfaces with PulseAudio directly and writes its results to certain files.
The scripts, written in lua and sh, read and interpret these files.

## Daemon

The daemon is responsible for the following:
- Maintaining an always up-to-date reference to the newest sink
- Maintaining always up-to-date information on all sinks
- Notifying on sink change
- Switching sink-inputs on sink change

## Scripts

The scripts can do the following:
- [Set volume](chvol)
- [Toggle mute](togglemute)
- [Pretty-print volume and mute](volume)

## Usage

- Compile the daemon
	```sh
	make
	```
- Run the daemon as a background process
	```sh
	./bin/pulsetest &
	```

## Related Reading

- [Code gist](https://gist.github.com/jasonwhite/1df6ee4b5039358701d2)
- [Official PulseAudio documentation](https://freedesktop.org/software/pulseaudio/doxygen/)

## TODO

- [create a custom pulseaudio module](https://www.freedesktop.org/wiki/Software/PulseAudio/Documentation/Developer/Modules/)

