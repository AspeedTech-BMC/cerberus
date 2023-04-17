// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license.

#ifndef CMD_INTERFACE_H_
#define CMD_INTERFACE_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "status/rot_status.h"
#include "cerberus_protocol.h"


/**
 * Container for message data.
 */
struct cmd_interface_msg {
	uint8_t *data;					/**< The raw message data buffer.  This contains the message to
										process. If message is a request, this buffer can be updated
										with any response data. */
	size_t length;					/**< Length of the data buffer contents. */
	size_t max_response;			/**< Maximum length allowed for a response. */
	uint8_t source_eid;				/**< Endpoint ID that generated the message. */
	uint8_t source_addr;			/**< Address of device that generated the message. */
	uint8_t target_eid;				/**< Endpoint ID that should process the message. */
	bool crypto_timeout;			/**< Flag indicating if the message is a request and required
										cryptographic operations and should be granted a longer
										timeout.  This is set for every message, even when there is
										an error. */
	int channel_id;					/**< Channel on which the message is received. */
};

/**
 * A list of firmware versions.
 */
struct cmd_interface_fw_version {
	size_t count;					/**< The number of firmware identifiers. */
	const char **id;				/**< The list of firmware identifiers. */
};

/**
 * A list of device IDs.
 */
struct cmd_interface_device_id {
	uint16_t vendor_id;				/**< Vendor ID */
	uint16_t device_id;				/**< Device ID */
	uint16_t subsystem_vid;			/**< Subsystem vendor ID */
	uint16_t subsystem_id;			/**< Subsystem ID */
};


/**
 * Command interface for processing received requests.  This is just a common base type and should
 * not be instantiated directly.
 */
struct cmd_interface {
	/**
	 * Process a received request.
	 *
	 * @param intf The command interface that will process the request.
	 * @param request The request data to process.  This will be updated to contain a response, if
	 * necessary.
	 *
	 * @return 0 if the request was successfully processed or an error code.
	 */
	int (*process_request) (struct cmd_interface *intf, struct cmd_interface_msg *request);

#ifdef CMD_ENABLE_ISSUE_REQUEST
	/**
	 * Process a received response.
	 *
	 * @param intf The command interface that will process the response.
	 * @param response The response data to process.
	 *
	 * @return 0 if the response was successfully processed or an error code.
	 */
	int (*process_response) (struct cmd_interface *intf, struct cmd_interface_msg *response);
#endif

	/**
	 * Generate a message to indicate an error condition.
	 *
	 * @param intf The command interface to utilize.
 	 * @param request The request container to utilize.
	 * @param error_code Identifier for the error.
	 * @param error_data Data for the error condition.
 	 * @param cmd_set Command set to respond on.
	 *
	 * @return 0 if the packet was generated successfully or an error code.
	 */
	int (*generate_error_packet) (struct cmd_interface *intf, struct cmd_interface_msg *request,
		uint8_t error_code, uint32_t error_data, uint8_t cmd_set);

	struct session_manager *session;				/**< Session manager for channel encryption */
	bool curr_txn_encrypted;						/**< Current transaction encrypted */
};


/* Internal functions for use by derived types. */
int cmd_interface_process_cerberus_protocol_message (struct cmd_interface *intf,
	struct cmd_interface_msg *message, uint8_t *command_id, uint8_t *command_set, bool decrypt,
	bool rsvd_zero);
int cmd_interface_prepare_response (struct cmd_interface *intf, struct cmd_interface_msg *response);
int cmd_interface_generate_error_packet (struct cmd_interface *intf,
	struct cmd_interface_msg *request, uint8_t error_code, uint32_t error_data, uint8_t cmd_set);
#if defined(CONFIG_INTEL_PFR)
int cmd_interface_process_intel_pfr_protocol_message (struct cmd_interface *intf,
	struct cmd_interface_msg *message, uint8_t *command_id, uint8_t *command_set, bool decrypt,
	bool rsvd_zero);
#endif

#define	CMD_HANDLER_ERROR(code)		ROT_ERROR (ROT_MODULE_CMD_HANDLER, code)

/**
 * Error codes that can be generated by the command handler.
 *
 * Note: Commented error codes have been deprecated.
 */
enum {
	CMD_HANDLER_INVALID_ARGUMENT = CMD_HANDLER_ERROR (0x00),		/**< Input parameter is null or not valid. */
	CMD_HANDLER_NO_MEMORY = CMD_HANDLER_ERROR (0x01),				/**< Memory allocation failed. */
	CMD_HANDLER_PROCESS_FAILED = CMD_HANDLER_ERROR (0x02),			/**< A general error while processing the request. */
	CMD_HANDLER_PAYLOAD_TOO_SHORT = CMD_HANDLER_ERROR (0x03),		/**< The request does not contain the minimum amount of data. */
	CMD_HANDLER_BAD_LENGTH = CMD_HANDLER_ERROR (0x04),				/**< The payload length is wrong for the request. */
	CMD_HANDLER_OUT_OF_RANGE = CMD_HANDLER_ERROR (0x05),			/**< A request argument is not within the valid range. */
	CMD_HANDLER_UNKNOWN_REQUEST = CMD_HANDLER_ERROR (0x06),			/**< A command does not represent a known request. */
	//CMD_HANDLER_UNSUPPORTED_EID = CMD_HANDLER_ERROR (0x07),		/**< The request was sent to an unsupported endpoint. */
	CMD_HANDLER_UNSUPPORTED_INDEX = CMD_HANDLER_ERROR (0x08),		/**< Request for information with an unsupported index was received. */
	CMD_HANDLER_UNSUPPORTED_LEN = CMD_HANDLER_ERROR (0x09),			/**< Request for information with an unsupported length was received. */
	CMD_HANDLER_INVALID_DEVICE_MODE = CMD_HANDLER_ERROR (0x0A),		/**< Invalid device mode. */
	CMD_HANDLER_BUF_TOO_SMALL = CMD_HANDLER_ERROR (0x0B),			/**< Provided buffer too small for output. */
	CMD_HANDLER_UNSUPPORTED_COMMAND = CMD_HANDLER_ERROR (0x0C),		/**< The command is valid but is not supported by the device. */
	CMD_HANDLER_UNSUPPORTED_MSG = CMD_HANDLER_ERROR (0x0D),			/**< Message type not supported. */
	CMD_HANDLER_UNSUPPORTED_CHANNEL = CMD_HANDLER_ERROR (0x0E),		/**< The command is received on a channel not supported by the device. */
	CMD_HANDLER_UNSUPPORTED_OPERATION = CMD_HANDLER_ERROR (0x0F),	/**< The requested operation is not supported. */
	CMD_HANDLER_RESPONSE_TOO_SMALL = CMD_HANDLER_ERROR (0x10),		/**< The maximum allowed response is too small for the output. */
	CMD_HANDLER_ENCRYPTION_UNSUPPORTED = CMD_HANDLER_ERROR (0x11),	/**< Channel encryption not supported on this interface. */
	CMD_HANDLER_CMD_SHOULD_BE_ENCRYPTED = CMD_HANDLER_ERROR (0x12),	/**< Secure command received unencrypted after establishing an encrypted channel. */
	CMD_HANDLER_RSVD_NOT_ZERO = CMD_HANDLER_ERROR (0x13),			/**< Reserved field is non-zero. */
	// CMD_HANDLER_ERROR_MESSAGE = CMD_HANDLER_ERROR (0x14),			/**< The handler received an error message for processing. */
	CMD_HANDLER_ISSUE_FAILED = CMD_HANDLER_ERROR (0x15),			/**< Failed to generate the request message. */
	CMD_HANDLER_ERROR_MSG_FAILED = CMD_HANDLER_ERROR (0x16),		/**< Failed to generate an error message. */
	CMD_HANDLER_UNKNOWN_RESPONSE = CMD_HANDLER_ERROR (0x17),		/**< A command does not represent a known response. */
	CMD_HANDLER_INVALID_ERROR_MSG = CMD_HANDLER_ERROR (0x18),		/**< The handler received an invalid error message. */
};


#endif /* CMD_INTERFACE_H_ */
