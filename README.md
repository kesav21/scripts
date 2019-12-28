
# scripts

These are the scripts I use to automate miscellaneous things on my machine.

## usage

This directory is located at `~/.local/bin/` on my machine.
I added `~/.local/bin/` and all of its subdirectories to `$PATH`, so I can reference the scripts directly, such as `dwm_quit`.

## naming convention

I am trying to have a consistent naming convention within this folder.
The convention I have decided upon is not only naming the script after what it does (such as `connect` or `notifier`), but also prefixing the script with the module that it belongs to, such as (`blue` or `batt`).

There are, of course, some (a lot, like 20%) exceptions to this rule.
The scripts that do not belong to any module do not have a prefix (such as `colortest` or `windowlookup`).
There are other exceptions where I thought it would be better to leave the script name be rather than force it into a convention that doesn't work for that particular script.

## todo

- move config files out of scripts and to `$XDG_CONFIG_HOME`
	- no need to include them in my dotfiles
	- i should add documentation about where config files are located
- dwm statusbar
	- add colors
	- add short/long formats
- remove `screensaver_toggle`, streamline `screensaver_init`

