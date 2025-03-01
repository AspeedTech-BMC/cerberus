// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license.

#ifndef MODULE_ID_H_
#define MODULE_ID_H_


/**
 * The IDs for core modules that can generate errors.
 */
enum {
	ROT_MODULE_INIT = 0x0000,							/**< Top-level application that initializes the system. */
	ROT_MODULE_AES_ENGINE = 0x0001,						/**< An AES crypto engine.  All engines use the same ID. */
	ROT_MODULE_ECC_ENGINE = 0x0002,						/**< An ECC crypto engine.  All engines use the same ID. */
	ROT_MODULE_HASH_ENGINE = 0x0003,					/**< A hash crypto engine.  All engines use the same ID. */
	ROT_MODULE_RSA_ENGINE = 0x0004,						/**< An RSA crypto engine.  All engines use the same ID. */
	ROT_MODULE_X509_ENGINE = 0x0005,					/**< An X.509 crypto engine.  All engines use the same ID. */
	ROT_MODULE_BASE64_ENGINE = 0x0006,					/**< A base64 crypto engine.  All engines use the same ID. */
	ROT_MODULE_FLASH_MASTER = 0x0007,					/**< A driver for SPI flash.  All drivers us the same ID. */
	ROT_MODULE_SPI_FLASH = 0x0008,						/**< The interface to accessing SPI flash. */
	ROT_MODULE_FLASH_COMMON = 0x0009,					/**< Common components for SPI flash commands. */
	ROT_MODULE_FLASH_UTIL = 0x000a,						/**< Utilities for programming and verifying flash. */
	ROT_MODULE_APP_IMAGE = 0x000b,						/**< Utilities for loading and verifying application images. */
	ROT_MODULE_FIRMWARE_IMAGE = 0x000c,					/**< Interface to the firmware image format for the platform. */
	ROT_MODULE_KEY_MANIFEST = 0x000d,					/**< Manifest containing authenticated keys. */
	ROT_MODULE_FIRMWARE_UPDATE = 0x000e,				/**< Firmware updater. */
	ROT_MODULE_I2C_MASTER = 0x000f,						/**< A driver for an I2C master.  All drivers use the same ID. */
	ROT_MODULE_I2C_SLAVE = 0x0010,						/**< A driver for an I2C slave.  All drivers use the same ID. */
	ROT_MODULE_SPI_FILTER = 0x0011,						/**< A driver for the SPI filter.  All drivers use the same ID. */
	ROT_MODULE_MFG_FILTER_HANDLER = 0x0012,				/**< Configuration interface to set the flash device for the filter. */
	ROT_MODULE_SPI_FILTER_IRQ = 0x0013,					/**< IRQ handler for the SPI filter. */
	ROT_MODULE_LOGGING = 0x0014,						/**< Logger for system or security events. */
	ROT_MODULE_CMD_HANDLER = 0x0015,					/**< Handler for received commands. */
	ROT_MODULE_MCTP_BASE_PROTOCOL = 0x0016,				/**< MCTP base protocol handler. */
	ROT_MODULE_RIOT_CORE = 0x0017,						/**< Implementation of RIoT Core. */
	ROT_MODULE_HOST_FW_UTIL = 0x0018,					/**< Host firmware validation functions. */
	ROT_MODULE_BMC_RECOVERY = 0x0019,					/**< BMC recovery manager. */
	ROT_MODULE_HOST_CONTROL = 0x001a,					/**< Driver interface for controlling the host processor. */
	ROT_MODULE_HOST_IRQ_CTRL = 0x001b,					/**< Driver interface for controlling host state IRQs. */
	ROT_MODULE_HOST_IRQ_HANDLER = 0x001c,				/**< Handler for host state IRQs. */
	ROT_MODULE_HOST_PROCESSOR = 0x001d,					/**< Manager for the host processor being protected. */
	ROT_MODULE_HOST_FLASH_MGR = 0x001e,					/**< Manager for host flash state. */
	ROT_MODULE_MOCK = 0x001f,							/**< Mock objects for unit testing. */
	ROT_MODULE_PLATFORM_TIMEOUT = 0x0020,				/**< Platform abstraction for a timeout. */
	ROT_MODULE_PLATFORM_MUTEX = 0x0021,					/**< Platform abstraction for mutexes. */
	ROT_MODULE_PLATFORM_TIMER = 0x0022,					/**< Platform abstraction for a timer. */
	ROT_MODULE_STATE_MANAGER = 0x0023,					/**< Storage for state information. */
	ROT_MODULE_MANIFEST = 0x0024,						/**< Manifest files used for provisioning. */
	ROT_MODULE_PFM = 0x0025,							/**< PFM files for host provisioning. */
	ROT_MODULE_CFM = 0x0026,							/**< CFM files for component provisioning. */
	ROT_MODULE_MANIFEST_MANAGER = 0x0027,				/**< Manager for manifest files. */
	ROT_MODULE_KEYSTORE = 0x0028,						/**< Persistent storage for device keys. */
	ROT_MODULE_RIOT_KEY_MANAGER = 0x0029,				/**< Management of RIoT keys and certificates. */
	ROT_MODULE_AUX_ATTESTATION = 0x002a,				/**< Handler for auxiliary attestation flows. */
	ROT_MODULE_FIRMWARE_HEADER = 0x002b,				/**< Header information on firmware images. */
	ROT_MODULE_ATTESTATION = 0x002c,					/**< Attestation manager. */
	ROT_MODULE_RNG_ENGINE = 0x002d,						/**< A RNG crypto engine.  All engines use the same ID. */
	ROT_MODULE_DEVICE_MANAGER = 0x002e,					/**< Device manager. */
	ROT_MODULE_PCR = 0x002f,							/**< PCR manager. */
	ROT_MODULE_CMD_BACKGROUND = 0x0030,					/**< Command handler background context. */
	ROT_MODULE_OBSERVABLE = 0x0031,						/**< Manager for observer notifications. */
	ROT_MODULE_PFM_OBSERVER = 0x0032,					/**< Observers for PFM management. */
	ROT_MODULE_CFM_OBSERVER = 0x0033,					/**< Observers for CFM management. */
	ROT_MODULE_SIG_VERIFICATION = 0x0034,				/**< Verification for signatures. */
	ROT_MODULE_MANIFEST_VERIFICATION = 0x0035,			/**< Verification and key management for manifests. */
	ROT_MODULE_SPI_FLASH_SFDP = 0x0036,					/**< SFDP parsing for SPI flash devices. */
	ROT_MODULE_HOST_FLASH_INIT = 0x0037,				/**< Delayed host flash initialization manager. */
	ROT_MODULE_FLASH = 0x0038,							/**< Flash device. */
	ROT_MODULE_RECOVERY_IMAGE_HEADER = 0x0039,			/**< Header information on a recovery image. */
	ROT_MODULE_IMAGE_HEADER = 0x003a,					/**< Header information on an image. */
	ROT_MODULE_RECOVERY_IMAGE_SECTION_HEADER = 0x003b,	/**< Header information on a recovery section image. */
	ROT_MODULE_CMD_CHANNEL = 0x003c,					/**< Communication channel for commands. */
	ROT_MODULE_FIRMWARE_COMPONENT = 0x003d,				/**< Firmware image component wrapper. */
	ROT_MODULE_SPI_SLAVE = 0x003e,						/**< A driver for a SPI slave.  All drivers use the same ID. */
//	ROT_MODULE_RESERVED_3F = 0x003f,					/**< Reserved. */
	ROT_MODULE_FLASH_UPDATER = 0x0040,					/**< Flash update management. */
	ROT_MODULE_CERT_DEVICE_HW = 0x0041,					/**< Device hardware for certificate management. */
	ROT_MODULE_APP_CONTEXT = 0x0042,					/**< Running context storage. */
//	ROT_MODULE_RESERVED_43 = 0x0043,					/**< Reserved. */
	ROT_MODULE_TPM = 0x0044,							/**< TPM implementation. */
//	ROT_MODULE_RESERVED_45 = 0x0045,					/**< Reserved. */
	ROT_MODULE_AUTHORIZATION = 0x0046,					/**< Authorization management for operations. */
	ROT_MODULE_CONFIG_RESET = 0x0047,					/**< Manager for clearing device configuration. */
	ROT_MODULE_CMD_AUTHORIZATION = 0x0048,				/**< Command authorization handler. */
	ROT_MODULE_RECOVERY_IMAGE = 0x0049,					/**< Recovery image. */
	ROT_MODULE_RECOVERY_IMAGE_OBSERVER = 0x004a,		/**< Observers for recovery image management. */
	ROT_MODULE_RECOVERY_IMAGE_MANAGER = 0x004b,			/**< Recovery image manager. */
	ROT_MODULE_PCD = 0x004c,							/**< PCD files for platform configuration. */
	ROT_MODULE_PCD_OBSERVER = 0x004d,					/**< Observers for PCD management. */
	ROT_MODULE_CONFIG_CMD_TASK = 0x004e,				/**< Command configuration task context. */
	ROT_MODULE_CMD_DEVICE = 0x004f,						/**< Command handler for device-specific workflows. */
	ROT_MODULE_HOST_PROCESSOR_OBSERVER = 0x0050,		/**< Observers for host processor management. */
	ROT_MODULE_COUNTER_MANAGER = 0x0051,				/**< Counter operation management. */
	ROT_MODULE_SESSION_MANAGER = 0x0052,				/**< Encrypted session management. */
	ROT_MODULE_FLASH_STORE = 0x0053,					/**< Block data storage in flash. */
	ROT_MODULE_KDF = 0x0054,							/**< Key derivation function. */
	ROT_MODULE_HOST_STATE_OBSERVER = 0x0055,			/**< Observers for host state changes. */
	ROT_MODULE_SYSTEM = 0x0056,							/**< Main system manager. */
	ROT_MODULE_SYSTEM_OBSERVER = 0x0057,				/**< Observers for system events. */
	ROT_MODULE_PLATFORM_SEMAPHORE = 0x0058,				/**< Platform abstraction for semaphores. */
	ROT_MODULE_INTRUSION_STATE = 0x0059,				/**< Chassis intrusion state detection. */
	ROT_MODULE_INTRUSION_STATE_OBSERVER = 0x005a,		/**< Observers for intrusion state changes. */
	ROT_MODULE_INTRUSION_MANAGER = 0x005b,				/**< Manager for intrusion detection. */
	ROT_MODULE_CMD_HANDLER_MCTP_CTRL = 0x005c,			/**< Handler for received MCTP control protocol messages. */
	ROT_MODULE_CERBERUS_PROTOCOL_OBSERVER = 0x005d,		/**< Cerberus protocol observer. */
	ROT_MODULE_ECC_DER_UTIL = 0x005e,					/**< Utilities for handling DER encoded ECC information. */
	ROT_MODULE_BUFFER_UTIL = 0x005f,					/**< General buffer handling utilities. */
	ROT_MODULE_OCP_RECOVERY_DEVICE = 0x0060,			/**< Device handler for the OCP Recovery protocol. */
	ROT_MODULE_OCP_RECOVERY_SMBUS = 0x0061,				/**< SMBus layer for the OCP Recovery protocol. */
	ROT_MODULE_MCTP_CONTROL_PROTOCOL_OBSERVER = 0x0062,	/**< MCTP control command interface observer. */
	ROT_MODULE_CMD_HANDLER_SPDM = 0x0063,				/**< Handler for received SPDM protocol commands. */
	ROT_MODULE_SPDM_PROTOCOL_OBSERVER = 0x0064,			/**< SPDM protocol observer. */
	ROT_MODULE_ASN1_UTIL = 0x0065,						/**< ASN.1 operations. */
	ROT_MODULE_EVENT_TASK = 0x0066,						/**< Task for event handling. */
	ROT_MODULE_PERIODIC_TASK = 0x0067,					/**< Task for periodic execution. */
	ROT_MODULE_FIRMWARE_LOADER = 0x0068,				/**< Handler to load firmware images into memory. */
	ROT_MODULE_DEVICE_MANAGER_OBSERVER = 0x0069,		/**< Observers for device manager events. */
	ROT_MODULE_ECC_HW = 0x006a,							/**< Driver interface for ECC accelerator hardware. */
	ROT_MODULE_COMMON_MATH = 0x006b,					/**< Common math operations. */
	ROT_MODULE_HEAP_WITH_DEFRAG = 0x006c,				/**< Heap allocator with defragmentation.*/
	ROT_MODULE_PLATFORM_OS = 0x006d,					/**< Platform abstraction for OS and task control. */
	ROT_MODULE_X509_EXTENSION = 0x006e,					/**< Extension handler for X.509 certificates. */
	ROT_MODULE_DICE_TCBINFO_EXTENSION = 0x006f,			/**< Extension handler for TCG DICE TcbInfo extensions. */
	ROT_MODULE_DICE_UEID_EXTENSION = 0x0070,			/**< Extension handler for TCG DICE Ueid extensions. */
	ROT_MODULE_DME_EXTENSION = 0x0071,					/**< Extension handler for DME extensions. */
	ROT_MODULE_DME_STRUCTURE = 0x0072,					/**< Parsing and management of the DME structure. */
	ROT_MODULE_I2C_FILTER = 0x0010,
};


#endif /* MODULE_ID_H_ */
