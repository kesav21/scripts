
# scripts

These are the scripts I use to automate miscellaneous things on my machine.
I use the dash shell, but (almost) all scripts are POSIX compliant.

## recommended usage

Create a folder (I use `~/.local/bin`).
Add these scripts to that folder.
Add that folder to `$PATH`.

## configuration

Some scripts require config files.
These config files are expected to be in `$XDG_CONFIG_HOME/bin`.

These are all the scripts that require config files, and the files they require:
- `cronbat`: `$XDG_CONFIG_HOME/bin/dunst_ids`
- `sshot`: `$XDG_CONFIG_HOME/bin/dunst_ids`
- `lockrc`: `$XDG_CONFIG_HOME/bin/lock_vars`
- `wifibar`: `$XDG_CONFIG_HOME/bin/wifi_interface`

## logs

Logs are stored in `$XDG_CACHE_HOME/bin`.

## todo

- figure out why lock screen freezes
- integrate bitwarden into workflow
- integrate xdg-open into workflow
- eliminate the need for pavucontrol
	* switch all sink-inputs to newest sink automatically
	* display volume of newest sink
	* change volume of newest sink
