
# pulsetest

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
