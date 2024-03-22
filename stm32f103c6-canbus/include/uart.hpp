#ifndef CONTROLLER_UART_H
#define CONTROLLER_UART_H

#include <stdarg.h>
#include <stdint.h>
#include <string.h>

namespace Uart {

void init();
void put(char c);
bool get(char *c);
void flush();
void isr();

} // namespace UART

#endif // ifndef CONTROLLER_UART_H
