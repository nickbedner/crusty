#include "stdio.h"
#include "winsock.h"

int rust_double(int n);

int main(int argc, char **argv) {
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  return rust_double(2);
}