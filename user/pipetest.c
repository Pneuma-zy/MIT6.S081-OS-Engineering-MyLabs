#include "kernel/types.h"
#include "user/user.h"

int p[2];
char *argv[2] = {"cat", 0};

int main()
{
  pipe(p);
  if (fork() == 0) {
    close(0);
    dup(p[0]);
    close(p[0]);
    close(p[1]);
    exec("cat", argv);
  } else {
    write(p[1], "hello world\n", 12);
    close(p[0]);
    close(p[1]);
  }
  exit(0);
}
