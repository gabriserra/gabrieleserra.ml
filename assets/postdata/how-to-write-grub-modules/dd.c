/* dd.c - command to copy disk to disk  */
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

#define SECTORS_NUM 128                   // working with 128 sector cluster, 64 KiB

static void print_progress(grub_uint64_t cur, grub_uint64_t total)
{  
  if (!total)
    return;

  grub_printf("\r Wrote sectors %lu / %lu \n", cur, total);
  grub_refresh();
}

static void print_written_mib(grub_disk_addr_t curr_sector)
{
  static grub_disk_addr_t last_mib = -1;
  grub_disk_addr_t mib;
  
  mib = (curr_sector << GRUB_DISK_SECTOR_BITS) >> 20;
  
  if (mib != last_mib)
  {
    last_mib = mib;
    grub_printf("\r%luMiB written", mib);
  }

}

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
grub_cmd_dd (grub_extcmd_context_t ctxt __attribute__ ((unused)), int argc, char **args)
{
  grub_disk_t idisk;
  grub_disk_t odisk;
  grub_off_t src_size;
  grub_off_t dst_size;
  
  grub_err_t err = 0;
  grub_uint8_t* buf = 0;

  if (argc < 1)
    return grub_error (GRUB_ERR_BAD_ARGUMENT, "input device name required");
  if (argc < 2)
    return grub_error (GRUB_ERR_BAD_ARGUMENT, "output device name required");

  idisk = grub_disk_open_helper(args[0]);
  
  if (!idisk)
  {
    return grub_errno;
  }

  odisk = grub_disk_open_helper(args[1]);

  if (!odisk)
  {
    return grub_errno;
  }
  
  src_size = grub_disk_native_sectors (idisk) << GRUB_DISK_SECTOR_BITS;
  dst_size = grub_disk_native_sectors (odisk) << GRUB_DISK_SECTOR_BITS;

  grub_printf("Input disk size %lu \n", src_size);
  grub_printf("Output disk size %lu \n", dst_size);

  if (src_size != GRUB_DISK_SIZE_UNKNOWN && dst_size != GRUB_DISK_SIZE_UNKNOWN && src_size > dst_size)
  {
    err = grub_error (GRUB_ERR_BAD_ARGUMENT, "source disk bigger than capacity of destination");
    goto end;
  }

  for (;;)
  {
    grub_ssize_t buf_size = GRUB_DISK_SECTOR_SIZE * SECTORS_NUM;
    grub_disk_addr_t sector = 0;
    
    buf = grub_malloc(buf_size);

    if (!buf)
    {
      err = grub_error(GRUB_ERR_OUT_OF_MEMORY, "failed to allocate buffer");
      break;
    }

    for (;;)
    {
      err = grub_disk_read (idisk, sector, 0, buf_size, buf);

      if (err)
        goto end;

      err = grub_disk_write (odisk, sector, 0, buf_size, buf); 
      
      if (err)
        goto end;

      if (src_size != GRUB_FILE_SIZE_UNKNOWN)
      {
        print_progress(sector, src_size >> GRUB_DISK_SECTOR_BITS);
      }
      else
      {
        print_written_mib(sector);
      }

      if (src_size != GRUB_DISK_SIZE_UNKNOWN && (sector << GRUB_DISK_SECTOR_BITS) + buf_size >= src_size)
      {
        // end of copy
        goto end;
      } 
      
      sector += SECTORS_NUM;
    }

  }

end:
  grub_disk_close (idisk);
  grub_disk_close (odisk);
  grub_free(buf);

  grub_printf(err ?
                 "\rFAILED!                                \n":
                 "\rdone.                                  \n");

  return err;
}

static grub_extcmd_t cmd;

GRUB_MOD_INIT(dd)
{
  cmd = grub_register_extcmd ("dd", grub_cmd_dd, 0,
      N_("if-device of-device"),
      N_("Copies data from the input device to the output device. "), 0);
}

GRUB_MOD_FINI(dd)
{
  grub_unregister_extcmd (cmd);
}
