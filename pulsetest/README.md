
# pulsetest

## related links

[code gist](https://gist.github.com/jasonwhite/1df6ee4b5039358701d2)
[pulseaudio documentation](https://freedesktop.org/software/pulseaudio/doxygen/)

## usage

- compile the program
	```sh
	make
	```
- run the executable in the background
	```sh
	./pulsetest &
	```
- set sink volume and mute using sink index
	```sh
	IFS=, read sink _ <"$XDG_CACHE_HOME"/bin/pulsetest.sink
	pactl set-sink-volume "$sink" "$1"
	pactl set-sink-mute "$sink" toggle
	```
- report sink volume and mute to statusbar
	```sh
	IFS=, read volume mute <"$XDG_CACHE_HOME"/bin/pulsetest.volume
	[ "$mute" -eq 0 ] && echo -n "墳 $volume%  " || echo -n "ﱝ $volume%  "
	```

## todo

- switch sink-inputs on sink change
- notify on sink change
