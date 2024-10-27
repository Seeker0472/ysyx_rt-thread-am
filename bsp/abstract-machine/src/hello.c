#include <klib.h>
#include <rtthread.h>

static int hello() {
  printf("Hello RISC-V!%d\n",rand());
  return 0;
}
// #define FCEUX
#define AM_HELLO
// #define MICROBENCH
#define SNAKE
// #define TYPING_GAME
// #define COREMARK
// #define BAD_APPLE


#ifdef FCEUX
extern int __am_fceux_am_main(const char * romname);
static int fceux(){
  const char*args="mario";
  __am_fceux_am_main(args);
  return 0;
}
MSH_CMD_EXPORT_ALIAS(fceux, fceux_am, Run_fceux_am!.);

#endif
#ifdef AM_HELLO
extern int __am_hello_main(const char *args);
int hello_amam(){
  const char*args="argsss";
  __am_hello_main(args);
  return 0;
}
MSH_CMD_EXPORT_ALIAS(hello_amam, hello_am, Hello from am!.);
#endif
#ifdef MICROBENCH
extern int __am_microbench_main(const char *args);
int microbench(){
  __am_microbench_main(NULL);
  return 0;
}
MSH_CMD_EXPORT_ALIAS(microbench, microbench, Run microbench!);
#endif
#ifdef SNAKE
extern int __am_snake_main(const char *args);
int snake(){
  __am_snake_main(NULL);
  return 0;
}
MSH_CMD_EXPORT_ALIAS(snake, snake, Play a snake game!);
#endif
#ifdef TYPING_GAME
extern int __am_typing_game_main(const char *args);
int type_game(){
  __am_typing_game_main(NULL);
  return 0;
}
MSH_CMD_EXPORT_ALIAS(type_game, tgame, Play Typing-game!);
#endif

#ifdef COREMARK
extern int __am_coremark_main();
int coremark(){
  __am_coremark_main();
  return 0;
}
MSH_CMD_EXPORT_ALIAS(coremark, coremark, Run coremark!);
#endif

MSH_CMD_EXPORT_ALIAS(hello, hello_lo, Hello from local!.);
INIT_ENV_EXPORT(hello);

#ifdef BAD_APPLE
extern void __am_bad_apple_main();
static int bad(){
  __am_bad_apple_main();
  return 0;
}
MSH_CMD_EXPORT_ALIAS(bad, badapple, RunBad_apple!.);
#endif
