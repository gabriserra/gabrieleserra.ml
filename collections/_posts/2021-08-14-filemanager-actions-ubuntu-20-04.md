---
layout: post
title:  "How to add filemanager actions in Ubuntu 20.04"
date:   2021-08-14 13:26:00 +0200
categories: ubuntu bash
style: blog
---

Long story short: yesterday, I worked on an old C project and refactor the entire structure.
That project counts hundreds of files and, when deleting the unused `.c` files, I was thinking about an auto-magical method to delete also the coupled `.h` file.

I thought the solution was to add a right-click action on Nautilus to enable deleting all files with the same name (but with different extensions).

Then I wrote this post to help people save the "10 minutes googling"!

## Disclaimer

I tested this method on my Desktop machine. Hence I cannot guarantee that it works on different setups.

- Desktop environment: GNOME 3.*
- Distro: Ubuntu 20.04
- Filemanager: Nautilus

I have not tested it, but it may also work with Caja and Nemo.

## Filemanager actions on GNOME

At first, you need to install `FileManager-Actions`.

> FileManager-Actions (formerly Nautilus-actions) is an extension for Nautilus file manager which allows the user to add arbitrary program to be launched through the Nautilus file manager popup menu of selected files.

From what I read on the documentation, the community changed the name from `Nautilus-actions` to `FileManager-actions` because currently, it also supports Caja and Nemo (in addition to Nautilus). However, I have not tested it (please let me know if anyone is using them!).

So, first, update and install the package.

```bash
sudo apt update
sudo apt install filemanager-actions
```

Then run `fma-config-tool` and create a new action.
I called it `Delele files with same name`.

![FileManagerAction window](/assets/postdata/{{ page.slug }}/images/fma-action-config.png)

When creating an action, please ensure that:
- [v] Display item in selection context menu

is flagged; otherwise, you will not be able to see the context menu during the file selection.
Then prepare a script in `/usr/bin/` that does what you need.


In my case, I needed to delete the file that had the same name but different extensions.

```bash
touch /tmp/delwsamename
# edit it with your editor
```

My script is the following one:

```bash
#/bin/bash

NAMEWEXT=$1 # the argument will be the selected file name
NAME=${NAMEWEXT%.*} # uses the % to extract substr till .
rm -f $NAME.*
```

Then mv it in `/usr/bin` and give it execute permissions:

```bash
sudo mv /tmp/delwsamename /usr/bin/
sudo chmod +x /usr/bin/delwsamename
```

Finally, insert the script path and the argument string.

![FileManagerAction window](/assets/postdata/{{ page.slug }}/images/fma-action-command.png)

To let Nautilus pass the filename as a script argument, you have to specify `%f`.
In my case, unfortunately, the "Legend" button is broken. However, I discovered that it uses the same `Exec` key format of `Desktop Entry` specification.

At this point, quit the Nautilus instance and open it again:

```bash
nautilus -q
nautilus
```

Now, let's have a try! Right-click and check out the result!

![FileManagerAction window](/assets/postdata/{{ page.slug }}/images/context-w-fma.png)


## Appendix: Desktop Entry Exec keys and FMA extension

- `%f` A single file name, even if multiple files are selected.
- `%F` A list of files. Each file is passed as a separate argument to the executable program.
- `%u` A single URL. Local files may either be passed as file: URLs or as file path.
- `%U` A list of URLs. Each URL is passed as a separate argument to the executable program. 
- `%d` Base directory

[Here](https://askubuntu.com/a/783313/940068) you can find a comprehensive list.