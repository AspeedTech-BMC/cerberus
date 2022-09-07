1. The recovery tool creates the recover image with the recovery header and the recovery data.
2. Please configure the "cancellation_image_generator.config" and "decommission_image_generator.config" file with the valid data.
3. This tool requires the input XML file and this file has the type of the image, recovery sections details.
4. The valid values for type that can be provided in the xml are as follows,
	Key Cancellation: 4
	Decommission: 5
5.  Below is the structure of the cancellation image file.
        pubkey 256 bytes
6. To generate the cancellation/decommission image, please run the python script as follows,
	python3 key_management_tool.py cancellation_image_generator.config
    python3 key_management_tool.py decommission_image_generator.config
