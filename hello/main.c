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

char *to_str(int t) {
  
  // Inicializa o vetor de char com o caracter '0' em todas as posições (exceto na última)
  for (int i = (STR_SIZE - 2); i >= 0; i--) {
    buffer[i] = '0';
  }
  
  buffer[(STR_SIZE - 1)] = '\0'; // Atribui '\0' na última posição, determinando o fim do vetor
  
  
  // Calcula o char equivalente ao algarismo em todas as posições
  for (int i = (STR_SIZE - 2); i >= 0; i--) {
    buffer[i] = (char)((t%16) + 48);
    t /= 16;
  }
  
  return buffer;
}

int main(void)   
{
  unsigned int time_i = wtii(); // O horário retornado convertido para hexadecimal considera o fuso UTC+0 (no Brasil é UTC-3)
  
  time_s = to_str(time_i);
  
  printf(time_s);
  printf("\r\n");
  
  return 0;
}





