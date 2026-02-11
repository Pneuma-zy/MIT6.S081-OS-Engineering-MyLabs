#include "kernel/types.h"
#include "user/user.h"

int p[2];
char *buf;

int main()
{
  pipe(p);
  int pid = fork();
  if (pid == 0)
  {
    pid = getpid();
    read(p[0], buf, sizeof(buf));
    printf("%d: received ping\n", pid);
    write(p[1], "p", sizeof("p"));
    close(p[0]);
    close(p[1]);
    exit(1);
  }
  else 
  {
    pid = getpid();
    write(p[1], "p", sizeof("p"));
    read(p[0], buf, sizeof(buf));
    printf("%d: received pong\n", pid);
    close(p[0]);
    close(p[1]);
  }
  exit(0);
}
