// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license.

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "status/rot_status.h"
#include "cmd_interface/cmd_device.h"
#include "common/observable.h"
#include "system/system_observer.h"


/**
 * Manager for system-wide controls and operations.
 */
struct system {
	const struct cmd_device *device;	/**< Device API for executing hardware functions. */
	struct observable observable;		/**< Observer manager for the system. */
};


int system_init (struct system *system, const struct cmd_device *device);
void system_release (struct system *system);

int system_add_observer (struct system *system, struct system_observer *observer);
int system_remove_observer (struct system *system, struct system_observer *observer);

void system_reset (struct system *system);


#define	SYSTEM_ERROR(code)		ROT_ERROR (ROT_MODULE_SYSTEM, code)

/**
 * Error codes that can be generated by the system.
 */
enum {
	SYSTEM_INVALID_ARGUMENT = SYSTEM_ERROR (0x00),	/**< Input parameter is null or not valid. */
	SYSTEM_NO_MEMORY = SYSTEM_ERROR (0x01),			/**< Memory allocation failed. */
};


#endif /* SYSTEM_H_ */
