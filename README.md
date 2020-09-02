
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
- `bmgen`: `$XDG_CONFIG_HOME/bin/bm_dirs`, `$XDG_CONFIG_HOME/bin/bm_files`

## logs

Logs are stored in `$XDG_CACHE_HOME/bin`.

## todo

- figure out why lock screen freezes
