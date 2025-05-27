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

#define STR_SIZE 6

const char *time_s;
char time[STR_SIZE];

void convert(int h) {
  
  for (int i = (STR_SIZE - 2); i >= 0; i--) {
    time[i] = '0';
  }
  
  for (int i = (STR_SIZE - 2); i >= 0; i--) {
    time[i] = (char)((h%10) + 48);
    h /= 10;
  }
  
  time[(STR_SIZE - 1)] = '\0';
  
  time_s = time;
  
  return;
}

int main(void)   
{
  unsigned int time_i = wtii();
  
  convert(time_i);
  
  printf(time_s); // O horário retornado convertido para hexadecimal considera o fuso UTC+0 (no Brasil é UTC-3)
  printf("\r\n");
  
  return 0;
}





