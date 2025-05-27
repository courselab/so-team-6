#include <stdio.h>

#define PRINT_TYPE_INFO(var) \
    printf("Variável: %s, Tamanho: %zu bytes\n", #var, sizeof(typeof(var)))

int main() {
  char time[2];
  time[0] = 's';
  time[1] = '\0';
  printf("%s", time);
  PRINT_TYPE_INFO(time);
  return 0;
}
