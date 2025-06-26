/*
 *    SPDX-FileCopyrightText: 2021 Monaco F. J. <monaco@usp.br>
 *    SPDX-FileCopyrightText: 2025 João Sena <joaopedromsena@gmail.com>
 *   
 *    SPDX-License-Identifier: GPL-3.0-or-later
 *
 *  This file is a derivative of SYSeg (https://gitlab.com/monaco/syseg)
 *  and includes modifications made by the following author(s):
 *  João Sena <joaopedromsena@gmail.com>
 */

/* This source file implements the kernel entry function 'kmain' called
   by the bootloader, and the command-line interpreter. Other kernel functions
   were implemented separately in another source file for legibility. */

#include "kernel.h" /* Essential kernel functions.  */
#include "bios1.h"  /* For kwrite() etc.            */
#include "bios2.h"  /* For kread() etc.             */
#include "kaux.h"   /* Auxiliary kernel functions.  */

/* In order to allow for the media to be bootable by BIOS, the file system
   signature starts with a jump instruction that leaps over the header data,
   and lands at the bootstrap program right next to it. In the present example,
   the signature is the instruction 'jump 0xe', follwed by the character
   sequence 'ty' (we thus jump 14 bytes). */

#define START 0x7c00  /* Memory position of code start */
#define SECTOR_SIZE 512 /* Size of a memory sector */
#define DIR_ENTRY_LEN 32  /* Max file name length in bytes.           */
#define FS_SIGNATURE "\xeb\xety" /* File system signature.                   */
#define FS_SIGLEN 4              /* Signature length.                        */

/* The file header. */

struct fs_header_t
{
  unsigned char signature[FS_SIGLEN];     /* The file system signature.              */
  unsigned short total_number_of_sectors; /* Number of 512-byte disk blocks.         */
  unsigned short number_of_boot_sectors;  /* Sectors reserved for boot code.         */
  unsigned short number_of_file_entries;  /* Maximum number of files in the disk.    */
  unsigned short max_file_size;           /* Maximum size of a file in blocks.       */
  unsigned int unused_space;              /* Remaining space less than max_file_size.*/
} __attribute__((packed)) fs_header;      /* Disable alignment to preserve offsets.  */

/* Kernel's entry function. */

void kmain(void)
{
  int i, j;

  register_syscall_handler(); /* Register syscall handler at int 0x21.*/

  splash(); /* Uncessary spash screen.              */

  shell(); /* Invoke the command-line interpreter. */

  halt(); /* On exit, halt.                       */
}

/* Tiny Shell (command-line interpreter). */

char buffer[BUFF_SIZE];
int go_on = 1;

void shell()
{
  int i;
  clear();
  kwrite("TinyDOS 1.0\n");

  while (go_on)
  {

    /* Read the user input.
       Commands are single-word ASCII tokens with no blanks. */
    do
    {
      kwrite(PROMPT);
      kread(buffer);
    } while (!buffer[0]);

    /* Check for matching built-in commands */

    i = 0;
    while (cmds[i].funct)
    {
      if (!strcmp(buffer, cmds[i].name))
      {
        cmds[i].funct();
        break;
      }
      i++;
    }

    /* If the user input does not match any built-in command name, just
       ignore and read the next command. If we were to execute external
       programs, on the other hand, this is where we would search for a
       corresponding file with a matching name in the storage device,
       load it and transfer it the execution. Left as exercise. */

    if (!cmds[i].funct)
      kwrite("Command not found\n");
  }
}

/* Array with built-in command names and respective function pointers.
   Function prototypes are in kernel.h. */

struct cmd_t cmds[] =
    {
        {"help", f_help}, /* Print a help message.       */
        {"quit", f_quit}, /* Exit TyDOS.                 */
        {"hello", f_hello}, /* Execute an example program. */
        {"list", f_list}, /* List the files in disk. */
        {0, 0}};

/* Build-in shell command: help. */

void f_help()
{
  kwrite("...me, Obi-Wan, you're my only hope!\n\n");
  kwrite("   But we can try also some commands:\n");
  kwrite("      hello   (to run a sample user program\n");
  kwrite("      quit    (to exit TyDOS)\n");
  kwrite("      list    (to list the files in disk)\n");
}

void f_quit()
{
  kwrite("Program halted. Bye.\r\n");
  go_on = 0;
}

void print_int_d(int value)
{
  for (int p = 10; p >= 0; p--) {
    buffer[p] = value%10 + '0';
    value /= 10;
  }
  buffer[11] = '\0';

  kwrite(buffer);
  kwrite("\n");
}

void print_int_h(int value)
{
  for (int p = 10; p >= 0; p--) {
    buffer[p] = value%16;
    buffer[p] = (buffer[p] < 10) ? (buffer[p] + '0') : (buffer[p] - 10 + 'A');
    value /= 16;
  }
  buffer[11] = '\0';

  kwrite(buffer);
  kwrite("\n");
}

/* List files in the volume.
 * Arguments: (none)
 */

void f_list()
{ 
  
  // fs_header = *((struct fs_header_t *)START);
  unsigned short header_offset = 0;

  char signature[4];
  for (int p = 0; p < 4; p++) {
    unsigned char s = *((unsigned char *)(START + header_offset));
    signature[p] = s;
    header_offset += sizeof(unsigned char);
  }

  unsigned short total_number_of_sectors = *((unsigned short *)(START + header_offset));  /* Number of 512-byte disk blocks.         */
  header_offset += sizeof(unsigned short);

  unsigned short number_of_boot_sectors = *((unsigned short *)(START + header_offset));   /* Sectors reserved for boot code.         */
  header_offset += sizeof(unsigned short);

  unsigned short number_of_file_entries = *((unsigned short *)(START + header_offset));   /* Maximum number of files in the disk.    */
  header_offset += sizeof(unsigned short);

  unsigned short max_file_size = *((unsigned short *)(START + header_offset));            /* Maximum size of a file in blocks.       */
  header_offset += sizeof(unsigned short);

  int unused_space = *((int *)(START + header_offset));                                   /* Remaining space less than max_file_size.*/
  
  // Copy entries to RAM
  // __asm__(
  //   "mov boot_drive, %dl \n"	              /* Select the boot drive (from rt0.o). */
  //   "mov $0x2, %ah \n"		                  /* BIOS disk service: op. read sector. */
  //   "mov $0x5, %al \n"                     /* Number of sectors to read.          */
  //   "mov $0x0, %ch \n"		                  /* Cylinder coordinate (starts at 0).  */
  //   "addb $0xb, %cl \n"                    /* Sector coordinate - adds the number of boot sectors already read (starts at 1).  */
  //   "mov $0x0, %dh \n"		                  /* Head coordinage     (starts at 0).  */
  //   "mov $_ENTRIES_ADDR, %bx \n"           /* Where to load the kernel (rt0.o).   */
  // );
  
  // Print entries
  unsigned char* entries_addr = START + (number_of_boot_sectors * SECTOR_SIZE);
  
  for (int e = 0; e < number_of_file_entries; e++)
  {
    unsigned char v = 0;
    for (int i = 0; i < DIR_ENTRY_LEN; i++)
    {
      unsigned char entry_letter = *((unsigned char *)(entries_addr + (DIR_ENTRY_LEN * e) + i)); /* Memomy position of a letter 'i' in the entry file 'f' */
      buffer[i] = entry_letter;
      if (entry_letter == '\0')
        break;
      v = 1;
    }
    if (v == 1)
    {
      kwrite(buffer);
      kwrite("\n");
    }
  }
}

/* Built-in shell command: example.

   Execute an example user program which invokes a syscall.

   The example program (built from the source 'prog.c') is statically linked
   to the kernel by the linker script (tydos.ld). In order to extend the
   example, and load and external C program, edit 'f_exec' and 'prog.c' choosing
   a different name for the entry function, such that it does not conflict with
   the 'main' function of the external program.  Even better: remove 'f_exec'
   entirely, and suppress the 'example_program' section from the tydos.ld, and
   edit the Makefile not to include 'prog.o' and 'libtydos.o' from 'tydos.bin'.

  */

extern int main();
void f_hello()
{
  main(); /* Call the user program's 'main' function. */
}
