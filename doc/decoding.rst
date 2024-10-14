decoding explained:
==================
In order to save space, they compressed scripts using zip.

To read the script, you'll have some work ...


so 0x504... is  xml_data_compressed_hex 


First we remove the "0x" prefix and convert the remaining hex string to binary data

xml_data_bin = bytes.fromhex(xml_data_compressed_hex[2:])

this data we write to disk as a zip file (and subsequently uncompress it using unzip)






.. code-block:: python

    import csv
    import subprocess
    import os
    
    def sanitize_filename(filename):
        invalid_chars = [' ', '\\', '/']
        for char in invalid_chars:
            filename = filename.replace(char, '_')
        return filename
    
    # Replace 'your_input.csv' with the actual CSV file name
    input_csv_file = 'modified_exportvida.csv'
    
    with open(input_csv_file, 'r', newline='', encoding='utf-8') as csvfile:
        csv_reader = csv.reader(csvfile)
    
        for row in csv_reader:
            # Extract fields from the CSV row
            fk_script = row[0]
            fk_language = row[1]
            display_text = row[2]
            xml_data_compressed_hex = row[6]
    
            # Sanitize the display_text for a filename
            sanitized_display_text = sanitize_filename(display_text)
    
            # Remove the "0x" prefix and convert the remaining hex string to binary data
            xml_data_bin = bytes.fromhex(xml_data_compressed_hex[2:])
    
            # Create a sanitized filename with .zip extension and write the binary data to it
            output_file_name = f'{display_text}_{row[4]}.zip'
            with open(output_file_name, 'wb') as output_file:
                output_file.write(xml_data_bin)
    
            print(f"Saved binary data for {display_text} to {output_file_name}")



volvo model logic
-----------------

mdl544yr2005eng167, mdl545yr2005eng167, mdl542yr2006eng167, mdl544yr2006eng167, mdl545yr2006eng167, mdl542yr2007eng167, mdl544yr2007eng167, mdl545yr2007eng167, mdl533yr2007eng167, mdl544yr2008eng167, mdl545yr2008eng167, mdl533yr2008eng167, mdl542yr2008eng167, mdl533yr2008eng199, mdl542yr2008eng199, mdl545yr2008eng199, mdl544yr2008eng199, mdl533yr2009eng199, mdl544yr2009eng199, mdl545yr2009eng199, mdl542yr2009eng199, mdl544yr2010eng199, mdl545yr2010eng199, mdl542yr2010eng199, mdl533yr2010eng199, mdl533yr2011eng199, mdl544yr2011eng199, mdl545yr2011eng199, mdl542yr2011eng199, mdl544yr2012eng199, mdl545yr2012eng199, mdl533yr2012eng199, mdl542yr2012eng199


The decoded XML file contains a description of all the models it applies to.

mdl545yr2008eng167

- mdl545 is a volvo estate (multiwagon)
- yr2008 is the constructionyear : 2008
- eng167 is a 1.6 diesel engine


So is it important to select only those XML files in which your model appears.



sniffing in XML files
---------------------

unzip Read_out_MOST_nodes_372302.zip

*372302 is the CEM*

after unzipping we get a file like this one : 0900c8af81d1cb6c

we can have a look:
*xmllint --format 0900c8af81d1cb6c | less*

- first we need to check if our model mdl545yr2008eng167 is in this file, if not it applies to other models

diagnostic extract CEM example 
------------------------------

.. code-block:: none

    372302,100,REID,DD06,Strömförsörjningsläge
    372302,2,BLOFF,FC,Serviceintervall
    372302,4,REID,3F98,ECM start signal
    372302,4,REID,5F30,Coolant water temp
    372302,100,REID,4007,Workshop test (TPMS) status
    372302,100,REID,4125,Batteritemperatur
    372302,100,REID,DD02,MECU spänningsmatning
    372302,100,REID,DD07,Strömförsörjningsläge
    372302,100,REID,DD00,Global tid
    372302,100,REID,DD00,Global real time
    372302,4,REID,3F02,Clutch pedal sensor


reading accelerator pedal position via CAN
------------------------------------------

<identifier ecu="284101" ecuMode="EcuType" read="4" type="REID" value="0141">



Brake light switch
------------------

<identifier ecu="284101" ecuMode="EcuType" read="4" type="REID" value="005C">


Turn indicator
--------------

641901,4,REID,0005,Turn indicator

Hidden features
---------------

There are numerous scripts, for instance about Vehcom, which could be Vehicle communication ?

<identifier ecu="372304" read="4" type="REID" value="9A00">
<name="Interior light, relay" textid="74202"/>

*this one is for activating the feature?*

<identifier ecu="372302" stop="16" type="REID" value="8F31" write="15">
<name="Rear wash motor relay" textid="13829">

<identifier ecu="372304" read="4" type="REID" value="8D04">
<name="Brake light, left" textid="72127"/>

<identifier ecu="372304" read="4" type="REID" value="8D05">
<name="Brake light, right" textid="72128"/>

<identifier ecu="372304" read="4" type="REID" value="8D01">
<name="Brake light switch" textid="4716"/>

CAN diagnostic messages HOWTO
-----------------------------



