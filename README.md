
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

- move config files out of scripts and to `$XDG_CONFIG_HOME`
	- no need to include them in my dotfiles
	- i should add documentation about where config files are located
- dwm statusbar
	- add colors
	- add short/long formats
- remove `screensaver_toggle`, streamline `screensaver_init`

