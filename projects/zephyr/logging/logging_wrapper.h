/*
 * Copyright (c) 2022 ASPEED Technology Inc.
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef LOGGING_WRAPPER_H_
#define LOGGING_WRAPPER_H_

#include <logging/logging_flash.h>

extern int logging_flash_wrapper_init (struct logging_flash *logging);
extern int debug_log_test(void);

#endif /* LOGGING_WRAPPER_H_ */
