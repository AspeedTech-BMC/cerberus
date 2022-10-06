// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license.

#ifndef CMD_DEVICE_H_
#define CMD_DEVICE_H_

#include <stdint.h>
#include <stddef.h>
#include "status/rot_status.h"


#pragma pack(push, 1)
/**
 * Heap statistics being reported.
 *
 * Unknown or unsupported values will be set to 0xffffffff.
 */
struct cmd_device_heap_stats {
	uint32_t total;				/**< Total size of heap memory. */
	uint32_t free;				/**< Amount of the heap that is currently free. */
	uint32_t min_free;			/**< The mimimun amount of free heap space. */
	uint32_t free_blocks;		/**< Number of blocks currently free. */
	uint32_t max_block;			/**< Size of the largest free block. */
	uint32_t min_block;			/**< Size of the smallest free block. */
};
#pragma pack(pop)

/**
 * A hardware-independent API to handle operations that require device-specific workflows.
 */
struct cmd_device {
	/**
	 * Retrieve the device unique chip identifier.
	 *
	 * @param device The device command handler.
	 * @param buffer The output buffer to store the chip ID.
	 * @param buf_len The size of the output buffer.
	 *
	 * @return The length of the chip ID or an error code.  Use ROT_IS_ERROR to check the return
	 * value.
	 */
	int (*get_uuid) (struct cmd_device *device, uint8_t *buffer, size_t buf_len);

	/**
	 * Reset the device.
	 *
	 * @param device The device command handler.
	 *
	 * @return A error code if the device could not be reset.  If the device is reset, this will not
	 * return.
	 */
	int (*reset) (struct cmd_device *device);

	/**
	 * Retrieve the reset counter.
	 *
	 * @param device The device command handler.
	 * @param type Reset counter type.
	 * @param port The port identifier.
	 * @param counter The output buffer to store the reset counter data.
	 *
	 * @return 0 if the reset counter was successfully retrieved or an error code.
	 */
	int (*get_reset_counter) (struct cmd_device *device, uint8_t type, uint8_t port,
		uint16_t *counter);

#ifdef CMD_ENABLE_HEAP_STATS
	/**
	 * Retrieve current heap usage statistics.
	 *
	 * @param device The device command handler.
	 * @param heap Output for the current heap statistics.
	 *
	 * @return 0 if the heap statistics were successfully retrieved or an error code.
	 */
	int (*get_heap_stats) (struct cmd_device *device, struct cmd_device_heap_stats *heap);
#endif
};


#define	CMD_DEVICE_ERROR(code)		ROT_ERROR (ROT_MODULE_CMD_DEVICE, code)


/**
 * Error codes that can be generated by a device command handler.
 */
enum {
	CMD_DEVICE_INVALID_ARGUMENT = CMD_DEVICE_ERROR (0x00),			/**< Input parameter is null or not valid. */
	CMD_DEVICE_NO_MEMORY = CMD_DEVICE_ERROR (0x01),					/**< Memory allocation failed. */
	CMD_DEVICE_UUID_BUFFER_TOO_SMALL = CMD_DEVICE_ERROR (0x02),		/**< A buffer for the uuid output data was too small. */
	CMD_DEVICE_RESET_FAILED = CMD_DEVICE_ERROR (0x03),				/**< Failed to trigger a device reset. */
	CMD_DEVICE_INVALID_COUNTER = CMD_DEVICE_ERROR (0x04),			/**< Invalid counter type. */
	CMD_DEVICE_HEAP_FAILED = CMD_DEVICE_ERROR (0x05),				/**< Failed to get heap statistics. */
};


#endif /* CMD_DEVICE_H_ */
