I placed the wrong order and received stm32f103c6 instead of stm32f103c8.
Same appearance but "c6" has a limited amount of memory. I used USART2, so some reconfig.


For some reason the Baudrate needs to be defined as double the one needed.

So Baud250 to get 125Kbs.

This has a link to the clock rate of the peripheral bus, which I cannot configure easily (possible with stmcube tools)
