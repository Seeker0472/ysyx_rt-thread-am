#include <am.h>
#include <klib.h>
#include <rtthread.h>

// void yield_with_func() {
// #ifdef __riscv_e
//   asm volatile("li a5, -1; ecall");
// #else
//   asm volatile("li a7, -1; ecall");
// #endif
// }
#define ADDR_ALIGN_BASE sizeof(uintptr_t)
#define ADDR_ALIGNED(addr) (((addr) + ADDR_ALIGN_BASE - 1)&(~((uintptr_t)(ADDR_ALIGN_BASE-1))))
// #define STACK_SIZE 

// static rt_ubase_t* context_switch_from=NULL;
// static rt_ubase_t* context_switch_to=NULL;

typedef struct{
  void * (*task_entry)(void * args);
  void * parameter;
  void * (*task_exit)();
} w_args;
//ecall调用
static Context* ev_handler(Event e, Context *c) {
  switch (e.event) {
  case EVENT_YIELD:
  {
    rt_thread_t cp = rt_thread_self();
    rt_ubase_t to = cp->user_data;
    c = *(Context **)to;
    // c->mepc += 4;
    break;
  }
  case EVENT_IRQ_TIMER://中断？？
  case EVENT_IRQ_IODEV:
    break;
  default:
    printf("Unhandled event ID = %d\n", e.event);
    assert(0);
  }
  return c;
}
void __am_cte_init() {
  cte_init(ev_handler);
}

//rt_ubase_t类型其实是unsigned long, to和from都是指向上下文指针变量的指针(二级指针). 
//TODO:上下文的切换,切换到to指向的上下文指针变量所指向的上下文
//传递参数->如果使用栈来传递参数似乎要修改yield函数，先用全局变量解决

void rt_hw_context_switch_to(rt_ubase_t to) {
  rt_thread_t pcb = rt_thread_self();
  rt_ubase_t user_data_saver = pcb->user_data;
  pcb->user_data = to;
  yield();
  pcb->user_data = user_data_saver;
}
//TODO:上下文的切换,,,还需要额外将当前上下文的指针写入from指向的上下文指针变量中.
//在rt_hw_context_switch()中调用yield(), 然后在ev_handler()中获得from和to
void rt_hw_context_switch(rt_ubase_t from, rt_ubase_t to) {
  rt_thread_t pcb = rt_thread_self();
  rt_ubase_t user_data_saver = pcb->user_data;
  pcb->user_data = to;
  from = (rt_ubase_t)pcb->sp;
  yield();
  pcb->user_data = user_data_saver;
}
//Ignore_temporarily
void rt_hw_context_switch_interrupt(void *context, rt_ubase_t from, rt_ubase_t to, struct rt_thread *to_thread) {
  assert(0);
}

void wrap(void *arg) {
  // parse paramater
  w_args *stack_bottom = (w_args *)arg;

  void * exit_func=stack_bottom->task_exit;
  
  ((void(*)())stack_bottom->task_entry) (stack_bottom->parameter);// 调用任务入口函数
  ((void(*)())exit_func) ();
}
// stack_addr为栈底创建一个入口为tentry, 参数为parameter的上下文, 并返回这个上下文结构的指针.
//   tentry -> task_entry
//   texit  -> task_exit
//   CTE的kcontext()要求不能从入口返回,要wrapper-function
//   让包裹函数来调用tentry, 并在tentry返回后调用texit, 然后将这个包裹函数作为kcontext()的真正入口
//   要把tentry,parameter，texit传递给包裹函数！！！！！！
//   如何实现？-使用结构体保存函数的指针，然后在wrapperfunction里面调用
rt_uint8_t *rt_hw_stack_init(void *tentry, void *parameter, rt_uint8_t *stack_addr, void *texit) {
  //  对齐
  stack_addr = (rt_uint8_t*)((uintptr_t)stack_addr & ~(sizeof(uintptr_t) - 1));
  // create context
  Area stack;
  stack.start = stack_addr - 0;
  stack.end = stack_addr;

  w_args *stack_bottom = (w_args *)(stack.end - sizeof(Context) - sizeof(w_args));

  Context *cp = kcontext(stack, wrap, (void *)stack_bottom);
  
  // 设置参数
  stack_bottom->parameter=parameter;
  stack_bottom->task_entry=tentry;
  stack_bottom->task_exit=texit;

  return (rt_uint8_t *)cp;
}
