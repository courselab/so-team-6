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
char buffer[STR_SIZE];

char *hour_br(char t[]) {
  int hour;

  hour = (((t[0] - '0') * 10) + (t[1] - '0')) - 3;

  for(i = 1; i >= 0, i++){
    t[i] = (char)((hour%10) + 48);
    hour /= 10;
  }

  return t;
}

char *to_str(int t, char b[]) {
  
  // Calcula o char equivalente ao algarismo em todas as posições
  for (int i = (STR_SIZE - 2); i >= 0; i--) {
    if(i != 2) {
      b[i] = (char)((t%16) + 48);
      t /= 16;
    } else {
      b[i] = ':';
    } 
  }
  
  b[(STR_SIZE - 1)] = '\0'; // Atribui '\0' na última posição, determinando o fim do vetor
  
  return b;
}

int main(void)   
{
  unsigned int time_i = wtii(); // O horário retornado convertido para hexadecimal considera o fuso UTC+0 (no Brasil é UTC-3)
  
  time_s = to_str(time_i);
  
  printf(time_s, buffer);
  printf("\r\n");

  time_s = hour_br(buffer);
  printf(time_s);
  printf("\r\n");

  return 0;
}






