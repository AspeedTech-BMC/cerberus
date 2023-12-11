// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license.

#ifndef SIGNATURE_VERIFICATION_H_
#define SIGNATURE_VERIFICATION_H_

#include <stdint.h>
#include <stddef.h>
#include "status/rot_status.h"


/**
 * Interface for executing signature verification.  Verification implementations may not be
 * thread-safe.
 */
struct signature_verification {
	/**
	 * Verify that a calculated digest matches a signature.
	 *
	 * @param verification The verification context to use for checking the signature.
	 * @param digest The digest to verify.
	 * @param digest_length The length of the digest.
	 * @param signature The signature to compare against the digest.
	 * @param sig_length The length of the signature.
	 *
	 * @return 0 if the digest matches the signature or an error code.  If no key has been provided
	 * for verification, SIG_VERIFICATION_NO_KEY will be returned.  If the signature does not match
	 * the specified digest, SIG_VERIFICATION_BAD_SIGNATURE will be returned.
	 */
	int (*verify_signature) (const struct signature_verification *verification,
		const uint8_t *digest, size_t length, const uint8_t *signature, size_t sig_length);

	/**
	 * Set the key to use for signature verification.
	 *
	 * It cannot be assumed that the verification context will copy the key data to internal
	 * storage.  Therefore, the buffer containing the key data must remain valid while it may be
	 * used for signature verification.
	 *
	 * @param verification The verification context to update.
	 * @param key The key to use for future signature verification requests.  Setting a null buffer
	 * will remove any active key.
	 * @param length The length of the verification key.
	 *
	 * @return 0 if the verification key was successfully updated or an error code.  If the provided
	 * key is not compatible with the verification context, SIG_VERIFICATION_INVALID_KEY will be
	 * returned.  If it is not possible to change the verification key, SIG_VERIFICATION_UNSUPPORTED
	 * will be returned.
	 */
	int (*set_verification_key) (const struct signature_verification *verification,
		const uint8_t *key, size_t length);

	/**
	 * Determine if a specified key is valid to use for signature verification.
	 *
	 * This will only check if the key could be used.  It will not change the key being used for
	 * signature verification.
	 *
	 * @param verification The verification context to check for key compatibility.
	 * @param key The key that should be checked.
	 * @param length The length of the verification key.
	 *
	 * @return 0 if the key is compatible with the verification context or an error code.
	 * SIG_VERIFICATION_INVALID_KEY will indicate an incompatible key.  If it is not possible to
	 * change the verification key, SIG_VERIFICATION_UNSUPPORTED will be returned.
	 */
	int (*is_key_valid) (const struct signature_verification *verification, const uint8_t *key,
		size_t length);
};


#define	SIG_VERIFICATION_ERROR(code)		ROT_ERROR (ROT_MODULE_SIG_VERIFICATION, code)

/**
 * Error codes that can be generated by an observer manager.
 */
enum {
	SIG_VERIFICATION_INVALID_ARGUMENT = SIG_VERIFICATION_ERROR (0x00),	/**< Input parameter is null or not valid. */
	SIG_VERIFICATION_NO_MEMORY = SIG_VERIFICATION_ERROR (0x01),			/**< Memory allocation failed. */
	SIG_VERIFICATION_VERIFY_SIG_FAILED = SIG_VERIFICATION_ERROR (0x02),	/**< There was a failure during signature verification. */
	SIG_VERIFICATION_SET_KEY_FAILED = SIG_VERIFICATION_ERROR (0x03),	/**< Failed to set a key for verification. */
	SIG_VERIFICATION_NO_KEY = SIG_VERIFICATION_ERROR (0x04),			/**< There is no key available to use for verification. */
	SIG_VERIFICATION_BAD_SIGNATURE = SIG_VERIFICATION_ERROR (0x05),		/**< The signature failed verification. */
	SIG_VERIFICATION_UNSUPPORTED = SIG_VERIFICATION_ERROR (0x06),		/**< The operation is not supported by the implementation. */
	SIG_VERIFICATION_CHECK_KEY_FAILED = SIG_VERIFICATION_ERROR (0x07),	/**< Failed to determine if the key is valid for verification. */
	SIG_VERIFICATION_INVALID_KEY = SIG_VERIFICATION_ERROR (0x08),		/**< The key cannot be used for verification. */
};


#endif /* SIGNATURE_VERIFICATION_H_ */
