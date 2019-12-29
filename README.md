
# scripts

These are the scripts I use to automate miscellaneous things on my machine.

## usage

This directory is located at `~/.local/bin` on my machine.
I added `~/.local/bin` and all of its subdirectories to `$PATH`, so I can reference the scripts directly (`dwm_quit` instead of `~/.local/bin/dwm/dwm_quit`).

## configuration

Config files must be located in `$XDG_CONFIG_HOME/bin` (which is usually `~/.config/`).

These are all the config files required:
- `bm_files`, `bm_dirs`: bookmarks
- `dunst_ids`: notifications
- `screensaver_vars`: screensaver

## logs

Logs are stored in `$XDG_CACHE_HOME/bin` (usually `~/.cache`).

## todo

- dwm statusbar
	- add short/long formats
- switch to faster sh (like dash)

