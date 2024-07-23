#include "kernel/param.h"
#include "kernel/types.h"
#include "user/user.h"

//这个部分的意思是  前面一段命令 | 后面一段命令
//将前面一段的标准化输出 作为后面一段的标准化输入
//所以可以直接从read 0 从标准化输入中读取前面一段的输出
//后面一段命令就是存放在argv中
int main(int argc, char *argv[]) {

  //他们在这里写了一个sleep(10) 说是find 会要时间  再返回之前就结束了,
  //但是我这里没有遇到这个问题

  if (argc < 2) {
    printf("Please enter more parameters!\n");
    exit(1);
  }

  char *xargv[MAXARG];
  int xargc = argc - 1;
  // 读取后面一段命令的参数
  // argc[1]是想要调用的命令
  // argc[0] = "xargs"
  for (int i = 1; i < argc; i++) {
    // sizeof(char) = 1 可以不写
    // strlen + 1 是包括了结尾的 /0   argv[i]也有/0 所以直接复制了
    xargv[i - 1] = malloc((strlen(argv[i]) + 1) * sizeof(char));
    strcpy(xargv[i - 1], argv[i]);
  }

  //读取前面一段命令的标准化输出
  int max_len = 1024;
  char buf[max_len];
  read(0, buf, max_len);

  // 遍历标准化输出, 执行命令
  char *p = buf;
  for (int i = 0; i < max_len; i++) {
    if (buf[i] == '\n') {
      if (fork() == 0) {
        buf[i] = 0; // 将当前换行符置为结束符

        // 将命令字符串起始地址存入 xargv
        xargv[xargc++] = p;

        xargv[xargc++] = 0; // 确保参数列表以 NULL 结尾

        // 执行命令的时候
        //从p的起始位置开始,   遇到指针遇到上面那个buf[i] = 0的位置停下来
        exec(xargv[0], xargv);

        // exec执行成功就不会返回了 如果返回到这个位置说明 exec出错
        exit(0);

      } else {
        //更新 p 指针，使其指向下一个命令的起始位置
        p = &buf[i + 1];
        wait(0);
      }
    }
  }

  // 释放分配的内存
  for (int i = 0; i < argc - 1; i++) {
    free(xargv[i]);
  }

  exit(0);
}
