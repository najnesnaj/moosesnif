Introduction
============

I have an old volvo, and although this is probably a bad indicator for a car enthusiast, I do like car technology.

Cars have evolved, and are now full of electronics. Microcontrollers interconnected via a CAN netwerk.

Very soon my old diesel car will not be legal anymore (although EURO 5, having a particle filter, EGR valve ....)

The value of my car was estimated 500 euro, and this was two years ago ...

Still, for me it is much more valuable! Certainly since I started looking under the hood.

I bought one of those cheap chinese diagnostic tools.
At first I did not pay attention, since all I wanted is save a buck on maintenance. This is one of the reasons old cars get expensive and people will get a new one.

The diagnostic tool is called VIDA, it came with a virtual box.
As it happens you can find this easily: e.g. https://volvodiag.com/product/virtualbox-build/  (google is your friend)

This is commercial software, so any free copy you will find is of course illegal. From what I found out somebody (Gunnar?) in the US made a copy.

Suppose you're a volvo dealer, you would like to use the same software for all your volvo models, even the older ones.

The software is using an sql-database and XML files, so everything is a parameter. I'm not an expert, but it is very well made, and offers a lot of insight on how professional developers create a highly configurable package.
Watch and learn!

A word of warning : the software is not open sourced! 


Instead of buying a new electric volvo my attention got caught by Damien Maguire MuskVo. He is involved in the openinverter project : https://openinverter.org/wiki/Main_Page.

As it turns out these car enthusiasts have succesfully converted modern (ecu containing) cars into electric vehicles with modern electrics and batteries.

A big hurdle is exactly the presence of all the electronics. If replacing the ICE (internal combustion engine) by electric, you will somehow have to fool the brain (ECM) that the ICE is still there.

All these modules talk to each other via the CAN bus network. An important step in an EV conversion is getting familiar with this network.

I created a cheap STM32 CAN interface, which consists of an advanced sniffer/sender and an emulator (which will eventually emulate the Volvo ECM)

As you might have guessed Car manufacturers are not really into open source. Even figuring out what is going on the CANBUS can be challenging. This is the reason why I analysed the diagnostic software, to gain further insight into CAN messaging.

This manual is an account of my journey in the car network, car communication and car diagnostics, which might pave the way for an EV conversion....




