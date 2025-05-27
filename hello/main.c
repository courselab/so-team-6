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
 
#include <stdio.h>

int main(void)   
{
  
  char time_s[2];
  // printf ("Duda");
  
  int time_i = wtii();
  
  // while (time_i[i] != '\0') {
  time_s[0] = (char)time_i;
    // i++;
  //}
  
  printf(time_s);
  
  return 0;
}





