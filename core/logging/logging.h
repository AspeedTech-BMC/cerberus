// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license.

#ifndef LOGGING_H_
#define LOGGING_H_

#include <stdint.h>
#include <stddef.h>
#include "status/rot_status.h"


/**
 * Marker to indicate the start of a log entry.
 */
#define LOGGING_MAGIC_START  		0xCB

/**
 * Determine if an entry marker indicates the start of a valid log entry.
 */
#define	LOGGING_IS_ENTRY_START(x)	(((x) & 0xF0) == 0xC0)

/**
 * Get the format identifier for the log entry header.
 */
#define	LOGGING_HEADER_FORMAT(x)	((x) & 0x0F)


#pragma pack(push, 1)

/**
 * Header added to every log entry.  It is important for backwards compatibility that future
 * versions of this header only add fields and not modify the order or size of existing fields.
 */
struct logging_entry_header {
	uint8_t log_magic;				/**< Start of entry marker. */
	uint16_t length;				/**< Length of the entry. */
	uint32_t entry_id;				/**< Unique entry identifier. */
};

/**
 * Placeholder for the minimum required definition for version 0xCC of the log header.  This will
 * provide forward compatibility for log parsing.
 */
struct logging_entry_header_cc {
	uint8_t log_magic;				/**< Start of entry marker. */
	uint16_t length;				/**< Length of the entry. */
	uint32_t entry_id;				/**< Unique entry identifier. */
	uint8_t data_offset;			/**< Offset within the entry where the log entry data starts. */
};

#pragma pack(pop)

/**
 * Defines the API for logging information.
 */
struct logging {
	/**
	 * Add a new entry to the log.
	 *
	 * The log will automatically flush as necessary when new entries are created and will wrap
	 * around to overwrite older entries when the log is full.
	 *
	 * @param logging The log to update with new data.
	 * @param entry The entry to add to the log.
	 * @param length The length of the entry.
	 *
	 * @return 0 if the entry was successfully added or an error code.
	 */
	int (*create_entry) (struct logging *logging, uint8_t *entry, size_t length);

#ifndef LOGGING_DISABLE_FLUSH
	/**
	 * For logs with buffered entries, flush the internal buffers.  This may cause older entries to
	 * be lost.  The newest entries that are being flushed will always be kept.
	 *
	 * @param logging The log to update.
	 *
	 * @return 0 if the log was successfully flushed or an error code.
	 */
	int (*flush) (struct logging *logging);
#endif

	/**
	 * Remove all data from the log.
	 *
	 * @param logging The log to update.
	 *
	 * @return 0 if the log was successfully cleared or an error code.
	 */
	int (*clear) (struct logging *logging);

	/**
	 * Get the amount of data currently stored in the log.  This includes all data in the log,
	 * including log entry headers.
	 *
	 * @param logging The log to query.
	 *
	 * @return The number of bytes in the log or an error code.  Use ROT_IS_ERROR to check the
	 * return value.
	 */
	int (*get_size) (struct logging *logging);

	/**
	 * Get the current contents of the log.  This is the raw log data, so it will include the log
	 * entry header data.
	 *
	 * @param logging The log to query.
	 * @param offset The offset within the log to start reading.
	 * @param contents Output buffer that will hold the contents of the log.
	 * @param length The maximum length of the contents that should be read.
	 *
	 * @return The number of bytes read from the log or an error code.  Use ROT_IS_ERROR to check
	 * the return value.
	 */
	int (*read_contents) (struct logging *logging, uint32_t offset, uint8_t *contents,
		size_t length);
};


#define	LOGGING_ERROR(code)		ROT_ERROR (ROT_MODULE_LOGGING, code)

/**
 * Error codes that can be generated by a logger.
 */
enum {
	LOGGING_INVALID_ARGUMENT = LOGGING_ERROR (0x00),		/**< Input parameter is null or not valid. */
	LOGGING_NO_MEMORY = LOGGING_ERROR (0x01),				/**< Memory allocation failed. */
	LOGGING_CREATE_ENTRY_FAILED = LOGGING_ERROR (0x02),		/**< A new entry was not added to the log. */
	LOGGING_FLUSH_FAILED = LOGGING_ERROR (0x03),			/**< The log was not stored to persistent memory. */
	LOGGING_CLEAR_FAILED = LOGGING_ERROR (0x04),			/**< The log entries were not cleared. */
	LOGGING_GET_SIZE_FAILED = LOGGING_ERROR (0x05),			/**< The log size could not be determined. */
	LOGGING_READ_CONTENTS_FAILED = LOGGING_ERROR (0x06),	/**< Could not retrieve the log entries. */
	LOGGING_INCOMPLETE_FLUSH = LOGGING_ERROR (0x07),		/**< Not all log entries were made accessible. */
	LOGGING_UNSUPPORTED_SEVERITY = LOGGING_ERROR (0x08),	/**< An entry was specified at an unsupported severity level. */
	LOGGING_STORAGE_NOT_ALIGNED = LOGGING_ERROR (0x09),		/**< Memory for the log is not aligned correctly. */
	LOGGING_BAD_ENTRY_LENGTH = LOGGING_ERROR (0x0a),		/**< The entry data is not the right size for the log. */
	LOGGING_NO_LOG_AVAILABLE = LOGGING_ERROR (0x0b),		/**< There is no log available for the operation. */
};


#endif /* LOGGING_H_ */
