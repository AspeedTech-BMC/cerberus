// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license.

#ifndef I2C_FILTER_INTERFACE_H_
#define I2C_FILTER_INTERFACE_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "status/rot_status.h"

/**
 * Defines the device to an I2C filter
 */
struct i2c_filter_device_info {
	uint8_t device_id;
	uint8_t slave_addr;
	/**
	 * white-list/allow-list data to pass through I2C filter . This should provided 256 bytes buffer to
	 * decide which I2C slave offset should be allowed/blocked by filter.
	 * 
	 * bit position as I2C slave offset data. ex. bit0 for offset 0x00 and bit32 for offset 0x20.
	 * And bit value as 1'b1 to I2C slave offset data for that slave data will be allowed by
	 * white-list/allow-list, otherwise will be blocked. 
	 */
	void* whitelist_elements;
};

/**
 * Defines the interface to an I2C filter
 */
struct i2c_filter_interface {

	struct i2c_filter_device_info filter;
	/**
	 * Initializes the I2C filter.
	 *
	 * @param filter The I2C filter to initialization.
	 *
	 * @return 0 if the I2C filter was configured successfully or an error code.
	 */

	int (*init_filter) (struct i2c_filter_interface *filter);

	/**
	 * Enable or disable the I2C filter.  A disabled I2C filter will block all access from the host
	 * to slave device.
	 *
	 * @param filter The I2C filter to update.
	 * @param enable Flag indicating if the I2C filter should be enabled or disabled.
	 *
	 * @return 0 if the I2C filter was configured successfully or an error code.
	 */

	int (*enable_filter) (struct i2c_filter_interface *filter, bool enable);

	/**
	 * Set the white-list/allow-list for the I2C filter.
	 *
	 * @param filter The I2C filter instance to use
	 * @param index The white-list/allow-list index to update
	 */

	int (*set_filter) (struct i2c_filter_interface *filter, uint8_t index);
};

#define	I2C_FILTER_ERROR(code)		ROT_ERROR (ROT_MODULE_I2C_FILTER, code)

/**
 * Error codes that can be generated by an I2C master driver.
 */
enum {
	I2C_FILTER_INVALID_ARGUMENT = I2C_FILTER_ERROR (0x00),	/**< Input parameter is null or not valid. */
	I2C_FILTER_NO_MEMORY = I2C_FILTER_ERROR (0x01),			/**< Memory allocation failed. */
	I2C_FILTER_WRITE_REG_FAILED = I2C_FILTER_ERROR (0x02),	/**< Could not write to a register. */
	I2C_FILTER_READ_REG_FAILED = I2C_FILTER_ERROR (0x03),	/**< Could not read from a register. */
	I2C_FILTER_BUSY = I2C_FILTER_ERROR (0x04),				/**< The I2C master is busy executing a transaction. */
	I2C_FILTER_TIMEOUT = I2C_FILTER_ERROR (0x05),			/**< The I2C transaction timed out. */
};


#endif /* I2C_FILTER_INTERFACE_H_ */
