#include "kernel/fs.h"
#include "kernel/stat.h"
#include "kernel/types.h"
#include "user/user.h"


void find(char *path, char *filename) {
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  //参数错误，find的第一个参数必须是目录
  if (st.type != T_DIR) {
    fprintf(2, "usage: find <DIRECTORY> <filename>\n");
    return;
  }

  if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
    printf("find: path too long\n");
    return;
  }

  strcpy(buf, path);

  p = buf + strlen(buf); //  指针 p 指向缓冲区 buf 中的末尾位置（即字符串的结尾处）。
  *p++ = '/';  //p指向最后一个'/'之后

  while (read(fd, &de, sizeof(de)) == sizeof(de)) {
    if (de.inum == 0) //如果 de.inum == 0，表示该目录项无效或未分配，跳过
      continue;

    //将目录项的名字复制到 buf 中，
    //这里p是指针 memmove里面没有改变p的值 所以循环调用只是更改同一个位置的内容
    memmove(p, de.name, DIRSIZ); //添加路径名称
    p[DIRSIZ] = 0;  //字符串结束标志
 
    if (stat(buf, &st) < 0) { //并调用 stat 函数获取该条目的详细信息（类型、inode 号码、大小）。
      printf("find: cannot stat %s\n", buf);
      continue;
    }

    // 不要在“`.`”和“`..`”目录中递归
    if(st.type == T_DIR && strcmp(p, ".") != 0 && strcmp(p, "..") !=0)
    {
      find(buf, filename);
    }
    if(st.type == T_FILE && strcmp(filename, p) == 0)
    {
      printf("%s\n", buf);
    }
  }

  close(fd);
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(2, "Usage find <DIRECTORY> <filename>\n");
    exit(0);
  }

  find(argv[1], argv[2]);

  exit(0);
}
