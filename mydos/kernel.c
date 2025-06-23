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
    {
      kwrite("Command executed: ");
      kwrite(buffer);
      kwrite("\n");
    }
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
  kwrite("      list    (to list the files in a disk\n");
}

void f_quit()
{
  kwrite("Program halted. Bye.\r\n");
  go_on = 0;
}

/* List files in the volume.
 * Arguments: (none)
 */

void f_list()
{
  char a1 = *((int *)(0x7c00 + 0));
  char a2 = *((int *)(0x7c00 + 1));
  char a3 = *((int *)(0x7c00 + 2));
  char a4 = *((int *)(0x7c00 + 3));
  buffer[0] = a1;
  buffer[1] = a2;
  buffer[2] = a3;
  buffer[3] = a4;
  buffer[4] = '\0';
  kwrite(buffer);
  kwrite("\n");
  // unsigned short total_number_of_sectors = *((int *)(0x7c00 + 4));   /* Number of 512-byte disk blocks.         */
  // int *number_of_boot_sectors = (int *)(0x7c00 + 2);    /* Sectors reserved for boot code.         */
  // int *number_of_file_entries = (int *)(0x7c00 + 2);    /* Maximum number of files in the disk.    */
  // int *max_file_size = (int *)(0x7c00 + 2);             /* Maximum size of a file in blocks.       */
  // int *unused_space = (int *)(0x7c00 + 2);              /* Remaining space less than max_file_size.*/
  //for (int i = 0; i < total_number_of_sectors; i++)
    //kwrite("oi");
  return;
}

extern int main();
void f_hello()
{
  main(); /* Call the user program's 'main' function. */
}
