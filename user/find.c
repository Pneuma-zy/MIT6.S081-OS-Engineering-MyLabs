#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

/*
char* fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}
*/

void find(char* path, char* file)
{
  char buf[256];
  struct stat st;
  struct dirent de;
  int fd;
  if ((fd = open(path, 0)) < 0)
  {
    fprintf(2, "find failed1\n");
    close(fd);
    return;
  }
  if (fstat(fd, &st) < 0)
  {
    fprintf(2, "find stat failed\n");
    close(fd);
    return;
  }
  if (st.type == T_DIR)
  {
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf))
    {
      printf("find: path too long\n");
      close(fd);
      return;
    }
    strcpy(buf, path);
    char *p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
      if (de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      char *name = "";
      strcpy(name, de.name);
      name[strlen(name)] = 0;
      //坑：fmtname函数中使用的是static缓冲区，如果都用fmtname比较，地址都会指向同一块固定的内存，因此strcmp比较永远是相等的，不能用两个fmtname比较
      if (strcmp(name, "..") == 0 || strcmp(name, ".") == 0)
      {
        continue;
      }
      if (!strcmp(name, file))
        printf("%s\n", buf);
      int fd_tmp = open(buf, 0);
      if (fd_tmp < 0)
      {
        printf("buf: %s\n", buf);
        fprintf(2, "find failed2\n");
        close(fd);
        close(fd_tmp);
        return;
      }
      if (fstat(fd_tmp, &st) < 0)
      {
        fprintf(2, "find stat failed\n");
        close(fd);
        close(fd_tmp);
        return;
      }
      if (st.type == T_DIR)
      {
        find(buf, file);
      }
      close(fd_tmp);
    }
  }
  else 
  {
    fprintf(2, "find in a directory failed\n");
    close(fd);
    return;
  }
  close(fd);
}

int main(int argc, char* argv[])
{
  if (argc <= 2)
  {
    printf("find failed\n");
    exit(0);
  }
  for (int i = 2; i < argc; i++)
  {
    find(argv[1], argv[i]);
  }
  
  exit(0);
}
