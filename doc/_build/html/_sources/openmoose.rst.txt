Openmoose
=========

Since I have a Dice cable (usb to OBD2), I wondered if this is usable to send/receive custom messages. Openmoose is a project to re-write the ECU using the dice cable, so figured it should be usable for diagnostic purposes.

http://www.openmoose.net/

https://github.com/rlinewiz/OpenMoose


Read and write your Volvo ECU (Bosch ME7) using a DiCE cable. 

modding
-------

jetbrains dotPeek was used to have a look at the J2534.dll.

I used a vbox which contained all the microsoft development, but it crashed often (my fault?).

https://developer.microsoft.com/en-us/windows/downloads/virtual-machines/

code
----

The following code was found in the J2534.dll, and shows how a can data-message is composed. This can be used to send custom messages.


.. code-block:: none

       public static readonly CANPacket msgCANReadECMConfig = new CANPacket(new byte[8]
    {
      (byte) 203,
      (byte) 122,
      (byte) 185,
      (byte) 245,
      (byte) 0,
      (byte) 0,
      (byte) 0,
      (byte) 0
    });



The extended ID, if not provided is a standard diagnostic EID like 0xFFFFFE.

.. code-block:: none


    public CANPacket(byte[] data)
    {
      this.data = new byte[data.Length + 4];
      this.data[0] = (byte) 0;
      this.data[1] = (byte) 15;
      this.data[2] = byte.MaxValue;
      this.data[3] = (byte) 254;
      for (int index = 0; index < data.Length; ++index)
        this.data[index + 4] = data[index];
      this.protocolType = PROTOCOL_TYPE.CAN_XON_XOFF;
    }
    

The EID can be given as a parameter as well.

.. code-block:: none

    public CANPacket(byte[] data, int eid)
    {
      byte[] eid1 = new byte[4]
      {
        (byte) 0,
        (byte) 0,
        (byte) 0,
        (byte) eid
      };
      eid1[2] = (byte) (eid >> 8);
      eid1[1] = (byte) (eid >> 16);
      eid1[0] = (byte) (eid >> 24);
      this.setupCANPacketEID(data, eid1);
    }

    
