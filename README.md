# moosesnif
canbus volvo reverse engineering

In this repository you can find a manual in pdf format, it is work in progress but could offer some insight in howto deal with Volvo CANBUS traffic.

I try to reverse engineer canbus traffic on a Volvo P1, but my effort is usable for all pre-2015 vehicles. 

The ultimate goal is an EV conversion. 


I modified the OpenMoose program, to send messages over the canbus using the Volvo DICE cable. (this already works!) Unfortunately I'm a lousy programmer and not really at ease in the Microsoft world, so if anybody can help out, send me a mail. I'm working on sniffing canbustraffic using the J2534 DLL. Should work, I guess, and it would be really useful since no extra hardware would be needed.

You can find a modified stm32 openlib3 program, which ultimate goal is to simulate an ECU. But this is of course work in progress.

