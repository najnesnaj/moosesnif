virtual box on linux
====================

The software of the VIDA diagnostic tool is installed on windows 7.


The software comes as an oracle vbox and is a windows 7 image.

*installing this under ubuntu 22 proved challenging*

the problem is USB 3
--------------------

- install the VirtualBox Extension Pack 7.0.12 (files/tools/extension pack manager)
- sudo usermod -a -G vboxusers $USER

-Win7: you need to install the "Intel USB 3.0 eXtensible Host Controller Driver" in your Windows 7 guest:
Update 2019-11-20: Intel seems to have removed the driver from its official site, you can get it from the Archive.org site: http://web.archive.org/web/201901092235 ... Driver.zip


- now the SETEK dice adapter should be visible


