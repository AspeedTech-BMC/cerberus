1. The recovery tool creates the recover image with the recovery header and the recovery data.
2. Please configure the config files with the valid data.
3. This tool requires the input XML file and this file has the type of the image, recovery sections details.
4. The valid values for type that can be provided in the xml are as follows,
        BMC: 00 00
        PCH: 01 00
        ROT: 02 00
        Provisioning: 03 00
        Key Cancellation: 04 00
        Decommission: 05 00
5.  Below is the structure of the recovery image file.
        header -
                header_length=2 bytes(Total header length)
                type=2 bytes
                magic=4 bytes(magic number to identify the header)
                version_id=32 bytes(coming from xml file)
                image_length=4 bytes(total file length)
                sig_length=4 bytes
                platform_id_length=1 byte
                platform_id=length varies based on xml file
        recoveryimagesectionlist-
                Headderlength=2 byte(recovery image section header length)
                format=2 byte(reserved)
                magic=4 byte(magicnumber to identify the recovery header)
                address=4 byte(coming from xml file)
                imagelength=4 bytes(encoded data coming from xml file)
                imagedata=data(data coming from xml )
        Signature=256 bytes
6. To generate the recovery image, please run the python script as follows,
        python3 recovery_image_generator.py
