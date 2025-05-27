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

const char *time_s;
char time[6];

void convert(int h) {
  
  for (int i = 4; i >= 0; i--) {
    time[i] = '0';
  }
  
  for (int i = 4; i >= 0; i--) {
    time[i] = (char)((h%10) + 48);
    h /= 10;
  }
  
  time[5] = '\0';
  
  time_s = time;
  
  return;
}

int main(void)   
{
  int time_i = wtii();
  
  convert(time_i);
  
  printf(time_s);
  
  return 0;
}





