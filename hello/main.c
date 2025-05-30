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

char *to_str(int num, char b[]) {
  
  // Calcula o char equivalente ao algarismo em todas as posições
  for (int i = (STR_SIZE - 2); i >= 0; i--) {
    if(i != 2) {
      b[i] = (char)((num%16) + 48);
      num /= 16;
    } else {
      b[i] = ':';
    } 
  }
  
  b[(STR_SIZE - 1)] = '\0'; // Atribui '\0' na última posição, determinando o fim do vetor
  
  return b;
}

char *hour_br(char t[]) {
  int hour = (((t[0] - '0') * 10) + (t[1] - '0')) - 3;
  
  if (hour < 0) {
    hour += 24;
  }

  for(int i = 1; i >= 0; i--){
    t[i] = (char)((hour%10) + 48);
    hour /= 10;
  }

  return t;
}

int main(void)   
{
  unsigned int time_i = wtii(); // O horário retornado convertido para hexadecimal considera o fuso UTC+0 (no Brasil é UTC-3)
  
  time_s = to_str(time_i, buffer);
  
  /*
  printf("Horario (UTC+0): ");
  printf(time_s);
  printf("\r\n");
  */
  
  printf("Horario (UTC-3): ");
  time_s = hour_br(buffer);
  printf(time_s);
  printf("\r\n");

  return 0;
}






