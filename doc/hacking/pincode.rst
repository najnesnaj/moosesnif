Pincodes
========


CEM PIN, ECM PIN, IMMO code, and other codes in Volvo cars: why some units can't be easily cloned via OBD and can't be used from another vehicle without modifications.

Every Volvo since 1998 has two basic security levels: CEM and ECM. In newer vehicles, additional units like IEM, VGM, etc., are integrated into this security system.

The content of the programming memory in these units is essentially divided into two parts:

- The first programming part of the memory requires a unique PIN code to overwrite the software, dictating how the unit behaves. For example, the software version and configuration table in the CEM unit or, typically from 2008 onwards, the engine maps in the ECM unit.

- The second programming part is memory requiring an IMMO PIN, which protects the EEPROM memory where synchronization codes are stored, typically essential for starting the vehicle. It involves synchronizing a unique number between the central CEM unit and the ECM engine unit. If this code does not match, the vehicle will not start. In newer vehicles, additional units like the BCM brake unit, the SCL steering wheel lock, ignition keys, remote controls, and lastly, audio system units that communicate over the MOST optical network, are integrated into the synchronization system.

For most vehicles, the CEM/ECM PIN and CEM IMMO can be decoded using VDASH or VDD. This allows most vehicle operations to be performed without the need to remove the unit and physically connect it to a specific programmer. Units that cannot be decoded via OBD can usually be decoded by removing and reading the unit, known as "on the bench."

Another way to obtain the PIN and IMMO code is to upload software from the official VIDA diagnostic and capture the programming key during the upload. However, Volvo considers this method illegal and in direct violation of the Volvo VIDA licensing agreement.

some examples:
---------------

- CEM PIN – for changing vehicle configuration (e.g., activating navigation, changing the US>EU market, removing the speed limiter, etc.), uploading a newer software version

    decoding via VDASH over OBD – P3, SPA 2015-2018, SPA 2021 if we know the VGM PIN, P2GGD (V8, 3.2i, D5 147kW), P1 GGD (typically 2011+)
    decoding via VDD – P1, P2 from MY2005+, P3, SPA 2015-2018, SPA 2021 if we know the VGM PIN

- CEM IMMO – adding ignition keys, changing CEM-ECM synchronization codes, pairing a new SCL or SCU, pairing audio system components

    decoding via VDASH P2 except for V8, 3.2i, D5 147kW, P3 except for V40/V40CC

- ECM PIN – performance optimization, EGR / DPF removal, deactivation of error codes

    not needed for P2, not needed for P1 until approximately MY2011
    decoding via OBD at P3 DENSO units, SPA 2015-2018 using UCBP
    cannot be decoded at P3 5-cylinder diesel engines

- ECM IMMO – changing the CEM-ECM synchronization key, pairing the BCM unit

    decoding via VDASH P1 1.6D 80kW and D5 and 2.4D 5-cylinder, P2 except for V8, 3.2i, D5 147kW, DENSO VEA P3 and SPA 2014 – 2018

- VGM PIN – only in vehicles with the iCUP (Android) system for erasing diagnostic codes throughout the vehicle and for any other vehicle operations

    Cannot be decoded via OBD, must be obtained from the official Volvo VIDA diagnostics
