#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
  if (argc <= 1)
  {
    printf("sleep failed\n");
    exit(0);
  }

  for (int i = 1; i < argc; i++)
  {
    int time = atoi(argv[i]);
    sleep(time);
  }
  exit(0);
}
