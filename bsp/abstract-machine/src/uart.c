/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 */

#include <rtdevice.h>
#include <rtthread.h>
#include <am.h>
#include <klib.h>

#define UART_DEFAULT_BAUDRATE 115200

struct device_uart {
  rt_ubase_t hw_base;
  rt_uint32_t irqno;
};

static void *uart0_base = (void*)0x10000000;
static struct rt_serial_device serial0;
static struct device_uart uart0;

static rt_err_t _uart_configure(struct rt_serial_device *serial, struct serial_configure *cfg) {
  return (RT_EOK);
}

static rt_err_t _uart_control(struct rt_serial_device *serial, int cmd, void *arg) {
  return (RT_EOK);
}

static int _uart_putc(struct rt_serial_device *serial, char c) {
  putch(c);
  return 1;
}
# define SERIAL_PORT     0x10000000L
static inline uint8_t  inb(uintptr_t addr) { return *(volatile uint8_t  *)addr; }

static int _uart_getc(struct rt_serial_device *serial) {
  // static const char *p = "help\ndate\nversion\nfree\nps\npa\npwd\nls\nmemtrace\nmemcheck\nutest_list\nversion\nhello\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nbadapple\n";
  // static const char *p = "hello_am\nfceux_am\n";
  // return (*p != '\0' ? *(p ++) : -1);
  bool have_code = (inb(SERIAL_PORT + 5) & 0x1) == 1;
  return have_code ? inb(SERIAL_PORT) : 0xff;
}

const struct rt_uart_ops _uart_ops = {
  _uart_configure,
  _uart_control,
  _uart_putc,
  _uart_getc,
  // TODO: add DMA support
  RT_NULL};

/*
 * UART Initiation
 */
int rt_hw_uart_init(void) {
  struct rt_serial_device *serial;
  struct device_uart *uart;
  struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;
  // register device
  serial = &serial0;
  uart = &uart0;

  serial->ops = &_uart_ops;
  serial->config = config;
  serial->config.baud_rate = UART_DEFAULT_BAUDRATE;
  uart->hw_base = (rt_ubase_t)uart0_base;
  uart->irqno = 0x0a;

  rt_hw_serial_register(serial,
      RT_CONSOLE_DEVICE_NAME,
      RT_DEVICE_FLAG_STREAM | RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX,
      uart);
  return 0;
}

/* WEAK for SDK 0.5.6 */
rt_weak void uart_debug_init(int uart_channel)
{
}
