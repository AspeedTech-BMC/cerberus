1. The recovery tool creates the recovery and update image with the recovery header and the recovery data.
   The recovery and update image is signed by CSK key.
2. Please configure the config files with the valid data.
3. This tool requires the input XML file and this file has the type of the image, recovery sections details.
4. The valid values for type that can be provided in the xml are as follows,
        BMC: 00 00
        PCH: 01 00
        ROT: 02 00
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
                Headderlength=2 bytes(recovery image section header length)
                format=2 bytes(reserved)
                magic=4 bytes(magicnumber to identify the recovery header)
                address=4 bytes(coming from xml file)
                imagelength=4 bytes(encoded data coming from xml file)
                imagedata=data(data coming from xml)
        Signature=256 bytes
        csk_pub_key=520 bytes

    5.1 Recovery and Update Image structure:
        ```
        struct recovery_image {
            struct recovery_header
            struct recovery_image_section_lists {
                struct recovery_section
                u8 imagedata[imagelength]
            }
            u8 signature[sig_length]
            struct rsa_public_key {
                uint8_t modulus[RSA_MAX_KEY_LENGTH]; // 512 bytes
                size_t mod_length;
                uint32_t exponent;
            } csk_pub_key;
        }
        ```

6. To generate the recovery image, please run the python script as follows,
        python3 recovery_image_generator.py
