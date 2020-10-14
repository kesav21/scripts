
# pulsetest

`pulsetest` is a collection of scripts I use to control anything relating to audio on my machines.
The core, written in C++, interfaces with pulseaudio directly and writes its results to certain files.
The peripherals, written in sh, read and interpret these files to performs various tasks on my machines.

## related links

- [code gist](https://gist.github.com/jasonwhite/1df6ee4b5039358701d2)
- [pulseaudio documentation](https://freedesktop.org/software/pulseaudio/doxygen/)

## usage

- compile the program
	```sh
	make
	```
- run the executable in the background
	```sh
	./pulsetest &
	```
- [set sink volume](chvol)
- [toggle sink mute](togglemute)
- [pretty print sink volume and mute](volume)

## todo

- switch sink-inputs on sink change
- notify on sink change
- [create a custom pulseaudio module](https://www.freedesktop.org/wiki/Software/PulseAudio/Documentation/Developer/Modules/)
