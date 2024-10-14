Response
--------

suppose you send a request for information over the canbus, you want to listen for a certain respons as well.

How is this done ?

In the request message, there the "type of operation" is defined : 

for example:

- A1 No Operation Performed (keep alive)
- A3 Security Access Mode
- A5 Read Current Data By Offset
- A6 Read Current Data By Identifier
- A7 Read Current Data By Address
- A8 Set Data Transmission
- A9 Stop Data Transmission


In the vida msql database (T111 service table) these values are defined.
In this table is a field (definition) that describes how this value should "behave".



example for code B9 (Read Data Block By Offset)
-----------------------------------------------


.. code-block:: xml

    <request>      
        <item name="Service" length="8" />      
        <item name="Identifier" length="8" />     
    </request>
    <response>      
        <block id="ID2_1" type="BLOFF" length="8" name="Bloff" />      
        <dynamiclist type="PARAM" name="Parameters" blockName="Parameter" parent="ID2_1" />     
    </response>


This XML snippet describes a structure for a request and a response. 

In the request section:

- There are three items:
  - "Service" with a length of 8 units.
  - "Identifier" length of 8 units.

In the response section:
- BLOFF (T142 blocktype) this refers to metaTable 146 (is not in database, but guess it is constructed out of XML file data depending on volvo model)
- ID2_1 (cannot find any reference in database ...)
- PARAM (T142 blocktype) refers to metaTable 148 (T148_metaPara) which gives all the parameters for an ECUvariant 


VIDA diagnostic logs
--------------------
When using VIDA diagnostic tools, at first it tries to get vehicle data.
As it happens, this might give some insight.


(extract)
<Request EcuAddress=”0B” DiagnosticPartNo=”30670330 A” CompletePartNo=”0008621154 A” Message=”B9F0” OrderPosition>
<Response Message=”F9F0000862115420204130670330202041”/>

In the Response Message I can see some of the request

*30670330202041* similar to ”30670330 A”
*0008621154202041* similar to ”0008621154 A”

apply this to real life example
-------------------------------

.. code-block:: none

    CAN_HS ---> ID=000ffffe data=cb 50 b9 f0 00 00 00 00
    CAN_HS <--- ID=01000003 data=8f 50 f9 f0 00 08 68 85
    CAN_HS <--- ID=01000003 data=09 13 20 20 41 30 65 70
    CAN_HS <--- ID=01000003 data=4c 24 20 20 41 00 00 00

- First byte in reply is a technical field (8F)
- Second byte is CEM ID (50)
- Third (F9) is like reply to B9
- Fourth (F0)
- Fifth 00
- PARTNUMBER  08688513  (08688513  A)
- SPACES 20 20 
- ASCII A
- DIAGNOSTICPARTNUMBER  (30657024  A)
- SPACES 20 20
- ASCII A           


- PARTNUMBER cannot be found in the database *maybe something like a serialnumber*
- in (T100_ecuvariant) the DIAGNOSTICPARTNUMBER is found and refers to  CEM 372302 


using the DHA (diagnostic host application) to get a clue
---------------------------------------------------------

this was easier to understand, and performs likely the same action
   
.. code-block:: none

        DIAG_ITEM [NAME=ECU: HW partnumber and DIA diagnostic number][SEND_ABLE=TRUE][NOTE=] [FORMAT=OTHER:2] [BASE=HEX] [VALUE=F0] [TYPE=BLOFF][INPUT=FALSE]
        BEGIN
          RESPONSE_ITEM [NAME=ECU Complete Part Number .][SEND_ABLE=FALSE][NOTE=ECU Complete Part Number] [NO_OF_BYTES=8] [OFFSET=2] [MASK=] [UNIT=.] [PRECISION=2] [SIGNED=U] [BASE=5 BCD 3 ASCII] [FORMULA=x] [COMP_VALUE=] [DEP_RESPITEM=CHECKOK] [DEP_RESPITEM_CHECK=TRUE]
          RESPONSE_ITEM [NAME=ECU Diagnostic Software number .][SEND_ABLE=FALSE][NOTE=ECU Diagnostic Software number] [NO_OF_BYTES=7] [OFFSET=10] [MASK=] [UNIT=.] [PRECISION=2] [SIGNED=U] [BASE=4 BCD 3 ASCII] [FORMULA=x] [COMP_VALUE=] [DEP_RESPITEM=CHECKOK] [DEP_RESPITEM_CHECK=TRUE]
        END



the response is defined : 

- offset 2 and 8 databytes 
- offset 10 and 7 databytes

according to this the PARTNUMBER would be *00*08688513  
