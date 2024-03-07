/* binload.c - command to load a file to physical memory address  */
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
#include <grub/err.h>
#include <grub/misc.h>

GRUB_MOD_LICENSE ("GPLv3+");

static grub_err_t
grub_cmd_binload (grub_extcmd_context_t ctxt __attribute__ ((unused)), int argc, char **args)
{
  grub_ssize_t size;
  grub_addr_t target;

  grub_file_t file = 0;
  grub_err_t err = 0;

  if (argc < 1)
    return grub_error (GRUB_ERR_BAD_ARGUMENT, "binary name required");
  if (argc < 2)
    return grub_error (GRUB_ERR_BAD_ARGUMENT, "physical address start required");

  file = grub_file_open (args[0], GRUB_FILE_TYPE_MULTIBOOT_MODULE | GRUB_FILE_TYPE_NO_DECOMPRESS);
  if (! file)
    return grub_errno;

  size = grub_file_size (file);
  if (size)
  {
    target = grub_strtoul(args[1], 0, 0);
  }
  else
  {
    grub_error (GRUB_ERR_FILE_READ_ERROR, N_("file size is zero"));
    err = grub_errno;
  }

  if (size && grub_file_read (file, (void*)target, size) != size)
  {
    grub_file_close (file);
      if (!grub_errno)
	      grub_error (GRUB_ERR_FILE_READ_ERROR, N_("premature end of file %s"), args[0]);
      return grub_errno;
    }

  if (err)
  {
    grub_file_close (file);
    return err;
  }

  grub_file_close (file);
  return GRUB_ERR_NONE;
}

static grub_extcmd_t cmd;

GRUB_MOD_INIT(binload)
{
  cmd = grub_register_extcmd ("binload", grub_cmd_binload, 0,
      N_("binload"),
      N_("Command to load a file to physical memory address. "), 0);
}

GRUB_MOD_FINI(binload)
{
  grub_unregister_extcmd (cmd);
}

