#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int i;

  // 输出到屏幕
  for(i = 1; i < argc; i++){
    write(1, argv[i], strlen(argv[i]));
    // 最后一个参数输出后换行，否则空格间隔
    if(i + 1 < argc){
      write(1, " ", 1);
    } else {
      write(1, "\n", 1);
    }
  }
  exit(0);
}
