#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{

  if (argc != 2)
  {
    fprintf(2, "Usage: sleep <time>\n");
    exit(1);
  }
  fprintf(1, "(nothing happens for a little while)\n");
  sleep(atoi(argv[1]));
  exit(0);
}
