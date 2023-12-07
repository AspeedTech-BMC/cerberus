/*
 * Copyright (c) 2022 ASPEED Technology Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#include <zephyr/kernel.h>

#pragma once

#define MCTP_BASE_PROTOCOL_MAX_TRANSMISSION_UNIT 64
#define MCTP_BASE_PROTOCOL_MAX_MESSAGE_BODY 1024

extern int handle_spdm_mctp_message(uint8_t bus, uint8_t src_eid, void *buffer, size_t *length);
