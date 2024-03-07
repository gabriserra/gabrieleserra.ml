/* disksize.c - command to print disk size  */
/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2008  Free Software Foundation, Inc.
 *
 *  GRUB is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  GRUB is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GRUB.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <grub/dl.h>
#include <grub/file.h>
#include <grub/disk.h>
#include <grub/term.h>
#include <grub/misc.h>
#include <grub/extcmd.h>
#include <grub/i18n.h>

GRUB_MOD_LICENSE ("GPLv3+");

/* Opens a disk. Disk name can also be provided using prefix/postfix parenthesis  */
static grub_disk_t grub_disk_open_helper(char* name)
{
  char* first;
  char* last;
  grub_disk_t disk;

  first = name;
  last = &(name[grub_strlen (name) - 1]);

  if (*first == '(' && *last == ')')
  {
    *last = '\0';
    first += 1;
    disk = grub_disk_open (first);
    *last = ')';
  }
  else
  {
    disk = grub_disk_open (first);
  }

  if (!disk)
  {
    return 0;
  }

  return disk;
}


static grub_err_t
grub_cmd_disksize (grub_extcmd_context_t ctxt __attribute__ ((unused)), int argc, char **args)
{
  grub_disk_t disk;
  grub_disk_addr_t sectors;
  grub_off_t size;
  
  grub_err_t err = 0;

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

static grub_extcmd_t cmd;

GRUB_MOD_INIT(disksize)
{
  cmd = grub_register_extcmd ("disksize", grub_cmd_disksize, 0,
      N_("disk"),
      N_("Output the disk size (both in bytes and sectors). "), 0);
}

GRUB_MOD_FINI(disksize)
{
  grub_unregister_extcmd (cmd);
}

