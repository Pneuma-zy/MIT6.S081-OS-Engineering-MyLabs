#include "kernel/types.h"
#include "user/user.h"

int p[20][2];
int buf;

void pipe_prime(int k)  //p[k][0], p[k][1]
{
  if (k >= 19)
  {
    exit(1);
  }
  int prime = 0;
  
  //由于不需要向上一层写数据，则直接关闭写端口，防止piperead堵塞
  close(p[k][1]);
  
  if (read(p[k][0], &buf, sizeof(buf)) == sizeof(buf))
  {
    prime = buf;
    printf("prime %d\n", prime);
  }
  else 
  {
    close(p[k][0]);
    exit(1);
  }
  pipe(p[k + 1]);
  if (fork() == 0)
  {
    close(p[k][0]);
    close(p[k + 1][1]);
    pipe_prime(k + 1);
    exit(1);
  }
  else 
  {
    close(p[k + 1][0]);
    while (read(p[k][0], &buf, sizeof(buf)) == sizeof(buf))
    {
      if (buf % prime != 0)
      {
        write(p[k + 1][1], &buf, sizeof(buf));
      }
    }
    close(p[k][0]);
    close(p[k + 1][1]);
  }
  wait(0);
  exit(1);
}

int main()
{
  pipe(p[0]);
  if (fork() == 0)
  {
    pipe_prime(0);
    exit(1);
  }
  else 
  {
    close(p[0][0]);
    for (int i = 2; i <= 35; i++)
    {
      write(p[0][1], &i, sizeof(i));
    }
    close(p[0][1]);
    wait(0);
  }
  exit(0);
}
