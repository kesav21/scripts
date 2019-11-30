
# scripts

These are the scripts I use to automate miscellaneous things on my laptop.

<!-- TODO: add screenshot script -->
<!-- TODO: fix brightness control -->
<!-- TODO: fix audio control -->

## `~/.local/bin/`

There is one problem that I face with this scripts directory.
As this directory grows, I sometimes move some files around within the file structure.
I also make new directories to accommodate some of the new files I create.
When I do this, the references to some of these files become obsolete, and need to be changed wherever they are referenced.

Therefore, I have decided to keep all of my scripts within `~/.local/scripts/` with a proper directory structure separating all of the scripts.
In addition, I have also decided to create a script that flattens the directory structure of all of my scripts and moves them to `~/.local/bin/`.

Currently, I do this by literally creating a new file in the `~/.local/bin/` which is a "clone" or "copy" of the original.
In the future, I think it might be better to create, in `~/.local/bin/`, a symbolic link to the original file.

## usage

Whenever I need to use any of my scripts, I reference the script through the `~/.local/bin/` directory.

## naming convention

I am trying to have a consistent naming convention within this folder.
The convention I have decided upon is not only naming the script after what it does (such as `connect` or `notifier`), but also prefixing the script with the module that it belongs to, such as (`blue` or `batt`).

There are, of course, some (a lot, like 20%) exceptions to this rule.
The scripts that do not belong to any module do not have a prefix (such as `colortest` or `windowlookup`).
There are other exceptions where I thought it would be better to leave the script name be rather than force it into a convention that doesn't work for that particular script.

## todo

- [add zathura window to tabbed through dmenu](https://www.reddit.com/r/suckless/comments/adfllb/how_to_create_named_tabs_using_the_suckless/)
