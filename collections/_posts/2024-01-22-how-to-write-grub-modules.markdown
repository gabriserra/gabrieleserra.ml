---
layout: post
title:  "How to write a GRUB module: dd, disksize and load examples"
date:   2024-01-22 10:15:15 +0100
categories: low-level c
style: blog
---

In these days I was working with x86-64 and I write a few modules for an older bootloader.
Therefore I decided to mess with GRUB loadable modules!

The first step is to clone GRUB repository (or, just download the ZIP).

```console
git clone https://git.savannah.gnu.org/git/grub.git
```

The change into the root directory. 
Loadable module APIs can be found in `include/grub/dl.h` while the list of GRUB commands can be found in `grub-core/commands`.

Let suppose to write a command which retrieves the hard disk size in bytes and sector.
The module (and therefore the command), will be called `disksize` and it will take the disk as an input.

Let's start using `GRUB_MOD_INIT` and `GRUB_MOD_FINI` APIs to register our command.
GRUB Module should adopt a compatible license (similar to what happens for Linux Kernel module), therefore, let's declare it at the beginning of our source code.

```c
GRUB_MOD_LICENSE ("GPLv3+");

static grub_extcmd_t cmd;

GRUB_MOD_INIT(disksize)
{
  cmd = grub_register_extcmd (
        "disksize",
        grub_cmd_disksize,
        0,
        N_("disk"),
        N_("Output the disk size (both in bytes and sectors). "),
        0
    );
}

GRUB_MOD_FINI(disksize)
{
  grub_unregister_extcmd (cmd);
}
```

The code snippet is straightforward. The function `grub_register_extcmd` is defined as follows:

```c
grub_extcmd_t grub_register_extcmd (
    const char*                     name,       // name of the command
    grub_extcmd_func_t              func,       // function to be executed
    grub_command_flags_t            flags,      // command flags
    const char*                     summary,    // command usage summary
    const char*                     description,// command brief description
    const struct grub_arg_option*   parser      // argument parser
);
```

All translatable strings should be enclosed with the `N_(str)` macro.
Now let's start to implement our `grub_cmd_disksize` function.

```c
static grub_err_t
grub_cmd_disksize (grub_extcmd_context_t ctxt __attribute__ ((unused)), int argc, char **args)
{
    grub_disk_t disk;
    grub_disk_addr_t sectors;
    grub_off_t size;

    grub_err_t err = 0;

    // disk name argument is required
    if (argc < 1)
        return grub_error (GRUB_ERR_BAD_ARGUMENT, "disk name required");

    disk = grub_disk_open_helper(args[0]);

    if (!disk)
    {
        return grub_errno;
    }

    sectors = grub_disk_native_sectors (disk);
    size = sectors << GRUB_DISK_SECTOR_BITS;

    if (sectors != GRUB_DISK_SIZE_UNKNOWN)
    {
        grub_printf("Disk: %s - Number of native sectors: %lu - Log sector size: %u - Bytes size: %lu \n", args[0], sectors, disk->log_sector_size, size);
    }
    else
    {
        err = grub_error (GRUB_ERR_BAD_ARGUMENT, "unknown disk size");
    }

    grub_disk_close (disk);

    return err;
}
```

The code above uses GRUB disk APIs (`include/grub/disk.h.`) to open a disk with a given name and to retrieve info about that disk (in this case only the number of sectors).

Since last decade, modern disks such NVME are adopting advanced format, which means they are using 4k sector size instead of classical 512 bytes block size.
A brief explanation taken from [Wikipedia](https://en.wikipedia.org/wiki/Disk_sector):

> In 1998 the traditional 512-byte sector size was identified as one impediment to increasing capacity which at that time was growing at a rate exceeding Moore's Law. Increasing the length of the data field through the implementation of Advanced Format using 4096-byte sectors removed this impediment; it increased the efficiency of the data surface area by five to thirteen percent while increasing the strength of the ECC which in turn allowed higher capacity. The format was standardized by an industry consortium in 2005 and by 2011 incorporated in all new products of all hard drive manufacturers. 

GRUB disk `total_sectors` is expressed as number sectors, unregarding the sector size and the `log_sector_size` property then helps in understanding the disk native block size. Therefore, to help GRUB developers, in each and every place, it's common to reason using `grub_native_sectors`, which is 512 bytes sector (basically `total_sectors` is equivalent to GRUB native sectors when disk native sectors are 512 bytes wide, `total_sectors << 3` when disk native sectors are 4k wide).

Then, to compute the disk size, it's enough to multiply the number of native sector by 512 (`size = sectors << GRUB_DISK_SECTOR_BITS`) when sectors are expressed in GRUB native.

Then, last but not least, it's necessary to add the loadable command module in the Makefile (`Makefile.core.def`).

```
module = {
  name = disksize;
  common = commands/disksize.c;
};
```

Here below I will add a few other examples which you can use as a reference (they may contain bugs, are just there to make some trials!).

- [`disksize`](/assets/postdata/{{ page.slug }}/disksize.c)
- [`dd`](/assets/postdata/{{ page.slug }}/dd.c) - **be careful!**
- [`binload`](/assets/postdata/{{ page.slug }}/binload.c)