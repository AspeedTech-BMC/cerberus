// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license.

#ifndef SPDM_PROTOCOL_OBSERVER_H_
#define SPDM_PROTOCOL_OBSERVER_H_

#include "status/rot_status.h"
#include "cmd_interface/cmd_interface.h"


/**
 * Interface for notifying observers of SPDM protocol responses.  Unwanted event notifications will
 * be set to null.
 */
struct spdm_protocol_observer {
	/**
	 * Notification that a SPDM get version response message has been received.
	 *
	 * Arguments passed with the notification will never be null.
	 *
	 * @param observer The observer instance being notified.
	 * @param reponse The response container received.
	 */
	void (*on_spdm_get_version_response) (const struct spdm_protocol_observer *observer,
		const struct cmd_interface_msg *response);

	/**
	 * Notification that a SPDM get capabilities response message has been received.
	 *
	 * Arguments passed with the notification will never be null.
	 *
	 * @param observer The observer instance being notified.
	 * @param reponse The response container received.
	 */
	void (*on_spdm_get_capabilities_response) (const struct spdm_protocol_observer *observer,
		const struct cmd_interface_msg *response);

	/**
	 * Notification that a SPDM negotiate algorithms response message has been received.
	 *
	 * Arguments passed with the notification will never be null.
	 *
	 * @param observer The observer instance being notified.
	 * @param reponse The response container received.
	 */
	void (*on_spdm_negotiate_algorithms_response) (const struct spdm_protocol_observer *observer,
		const struct cmd_interface_msg *response);

	/**
	 * Notification that a SPDM get digests response message has been received.
	 *
	 * Arguments passed with the notification will never be null.
	 *
	 * @param observer The observer instance being notified.
	 * @param reponse The response container received.
	 */
	void (*on_spdm_get_digests_response) (const struct spdm_protocol_observer *observer,
		const struct cmd_interface_msg *response);

	/**
	 * Notification that a SPDM get certificate response message has been received.
	 *
	 * Arguments passed with the notification will never be null.
	 *
	 * @param observer The observer instance being notified.
	 * @param reponse The response container received.
	 */
	void (*on_spdm_get_certificate_response) (const struct spdm_protocol_observer *observer,
		const struct cmd_interface_msg *response);

	/**
	 * Notification that a SPDM challenge response message has been received.
	 *
	 * Arguments passed with the notification will never be null.
	 *
	 * @param observer The observer instance being notified.
	 * @param reponse The response container received.
	 */
	void (*on_spdm_challenge_response) (const struct spdm_protocol_observer *observer,
		const struct cmd_interface_msg *response);

	/**
	 * Notification that a SPDM get measurements response message has been received.
	 *
	 * Arguments passed with the notification will never be null.
	 *
	 * @param observer The observer instance being notified.
	 * @param reponse The response container received.
	 */
	void (*on_spdm_get_measurements_response) (const struct spdm_protocol_observer *observer,
		const struct cmd_interface_msg *response);

	/**
	 * Notification that a SPDM ResponseNotReady error message has been received.
	 *
	 * Arguments passed with the notification will never be null.
	 *
	 * @param observer The observer instance being notified.
	 * @param reponse The response container received.
	 */
	void (*on_spdm_response_not_ready) (const struct spdm_protocol_observer *observer,
		const struct cmd_interface_msg *response);
};


#define	SPDM_PROTOCOL_OBSERVER_ERROR(code)		ROT_ERROR (ROT_MODULE_SPDM_PROTOCOL_OBSERVER, code)

/**
 * Error codes that can be generated by a SPDM protocol observer.
 */
enum {
	SPDM_PROTOCOL_OBSERVER_INVALID_ARGUMENT = SPDM_PROTOCOL_OBSERVER_ERROR (0x00),	/**< Input parameter is null or not valid. */
	SPDM_PROTOCOL_OBSERVER_NO_MEMORY = SPDM_PROTOCOL_OBSERVER_ERROR (0x01),			/**< Memory allocation failed. */
};


#endif /* SPDM_PROTOCOL_OBSERVER_H_ */
