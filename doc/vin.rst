Reading the VIN (Vehicle Identification Number)
===============================================

VIN stands for Vehicle Identification Number. It is a unique code assigned to every motor vehicle when it's manufactured. VINs are composed of 17 characters (digits and capital letters) and serve as a vehicle's fingerprint, providing information about its manufacturer, model, features, and more. VINs are used for various purposes including vehicle registration, tracking recalls, insurance records, and theft prevention.



At the start of the VIDA communication, it does not know your Volvo. (VIDA does not know nothing)

sniffing vida dice
------------------

- notice ID = 0x726
- DB 0 = 0x03 (meaning 3 bytes of data?)
- DB 1 = 0x22
- DB 2 = 0xf1 (address 0xF114 as specified in xml)
- DB 3 = 0x14


.. image:: images/vin.png



The following script I got from the internet, but it shows  that it loads a script 'Read VIN first readout (Odometer Value) (No immo check)' 
*'Read_VIN_first_readout_(Odometer_Value)_(No_immo_check)':
0900c8af8184c004*



.. code-block:: none

      [CarStatus       ][00F][Info]    Init CarStatus
      [ScriptProvider  ][00F][Event]   Database: DiagSwdlRepository, SP: GetScript,  Type: 'ReadVin' ScriptId: '' Language: 'en-US' EcuType: '-1' Profile: 'EMPTYPROFILE,'
      [ScriptProvider  ][00F][Info]    Fetched script: 'VCC-225200-1 1.8' title: 'Read VIN first readout (Odometer Value) (No immo check)'
      [Script          ][00F][Info]    Running script: 'VCC-225200-1 1.8' title: 'Read VIN first readout (Odometer Value) (No immo check)'
      [CarComRepository][011][Event]   Database: CarCom, SP: general_GetEcuId, EcuId: 30728270 AA, Result: 821
      [DiagnosticVehCom][011][Info]    Using diagnostic part number '30728270 AA' for system type: '0'
      [CarComRepository][011][Event]   Database: CarCom, SP: vadis_GetEcuVariantData, EcuVariant: 821



Fetched script
--------------

The script with title 'VCC-225200' got fetched.
As it happens this script can be found in a VIDA database table.

*fkScript,fkLanguage,DisplayText,XmlDataCompressed,checksum*

'Read_VIN_first_readout_(Odometer_Value)_(No_immo_check)'

uncompressing this : 0900c8af8184c004

which is an XML file

xml content
-----------

as it happens this part of the script applies to my car
There is a value : 0xF114, sending this to the ECU should get some VIN values.

.. code-block:: none

      <node class="Components.ReadVinComponent" id="nev9333590-nev15368413n1" nodeid="EuCD" x="620" y="275">
        <property class="Int" id="nev9333590-nev15368414n1" name="CanHiSpeedValue" type="In" value="500000" visible="0"/>
        <property class="Int" id="nev9333590-nev15368415n1" name="UseRelay" type="In" value="0" visible="0"/>
        <property class="Byte" id="nev9333590-nev15368416n1" name="BusId" type="In" value="1" visible="0"/>
        <property class="String" id="nev9333590-nev15368417n1" name="Identifier" type="In" value="F114" visible="0"/>
        <property class="String" id="nev9333590-nev15368418n1" name="Mode" type="Internal" value="EcuVariant" visible="1"/>
        <property class="Int" id="nev9333590-nev15368419n1" name="ServiceId" type="In" value="100" visible="0"/>
        <property class="String" id="nev9333590-nev15368420n1" name="EcuName" type="Out" value="" visible="1"/>
        <property class="Response" id="nev9333590-nev15368421n1" name="Result" type="Out" value="" visible="1"/>
        <property class="String" id="nev9333590-nev15368422n1" name="EcuVariant" type="In" value="30728270 AA" visible="0"/>
        <extension id="nev9333590-nev15368423n1"/>
      </node>





Now what happens next ?

.. code-block:: none

       [CarComRepository][010][Event] Database: CarCom, SP: general_GetEcuId,
       EcuId: 30728270 AA, Result: 821


It gets to the CarCom database (just a microsoft sql mdf file-somewhere at the back of this manual there is a procedure to access these files under linux)

*SP: general_getEcuID*

- SP = stored procedure 
- Carcom = database

*select id from T100_Ecuvariant where identifier = @ecuIdentifier*

so it will get the Ecu variant in the T100 table where EcuId="30728270 AA"

- it comes up with ecuvariant 821
- next a stored procedure vadis_GetEcuVariantData is executed

the following is an example of such a stored procedure, it might not be entirely correct, since I used ocr software ..

.. code-block:: none


    SELECT
    T100_EcuVariant. identifier AS DiagNumber,
    102_EcuType.identifier AS EcuTypeldentifier,
    T101_Ecu.identifier AS Eculdentifier,
    T121_Config.commAddress AS EcuAddress,
    T100_EcuVariant.id AS EcuVariantid,
    T102_EcuType.description AS EcuName,
    T121_Config.id As Configid,
    T121_Config.fkt121_Config_Gateway As GatewayConfigId,
    T122_Protocol.id AS Protocolld
    FROM
    T100_EcuVariant
    INNER JOIN
    T101_Ecu ON T100_EcuVariant.fkT101_Ecu = T101_Ecu.id
    INNER JOIN
    T102_EcuType ON T101_Ecu.fkT102_EcuType
    INNER JO1N
    T120_Config_EcuVariant ON T100_EcuVariant.id = T120_Config_EcuVariant.fkT100_Ecu
    Variant
    INNER JOIN
    T121_Config ON T120_Config_EcuVariant.fkT121_Config = T121_Config.id
    INNER JOIN
    T122_Protocol ON T121_Config.fkT122_pratocol
    WHERE (T100_EcuVariant.id = @ecuVariantid)
    ORDER BY Priority
    
Stored procedure : general_GetEcuId
-----------------------------------

