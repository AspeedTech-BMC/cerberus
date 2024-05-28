/*
 * Copyright (c) 2022 ASPEED Technology Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#include <zephyr/kernel.h>

#pragma once

#define MCTP_BASE_PROTOCOL_MAX_TRANSMISSION_UNIT 64
#define MCTP_BASE_PROTOCOL_MAX_MESSAGE_BODY 1024

extern struct spdm_context *find_spdm_context(uint8_t bus, uint8_t src_eid);
extern int handle_spdm_mctp_message(void *context, void *buffer, size_t *length, uint32_t *session_id);
extern int decrypt_secure_content(void *context, void *buffer, size_t *length, uint32_t *session_id);
extern int encrypt_secure_content(void *context, uint8_t *rsp_hdr, uint8_t *rsp_body, size_t rsp_body_len, void *buffer, size_t *length, uint32_t *session_id);
