#include <klib.h>
#include <rtthread.h>

static int hello() {
  printf("Hello RISC-V!\n");
  return 0;
}

extern void __am_bad_apple_main();
static int bad(){
  __am_bad_apple_main();
  return 0;
}
INIT_ENV_EXPORT(hello);
// INIT_ENV_EXPORT(bad);
MSH_CMD_EXPORT_ALIAS(bad, badapple, RunBad_apple!.);

