/*
 * Copyright (c) 2022 ASPEED Technology Inc.
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "platform.h"


/**
 * GCC implementation for the strdup function.
 *
 * @param s The string to duplicate.
 *
 * @return The newly allocated copy of the string or null.
 */
char *strdup(const char *s)
{
	char *str = NULL;

	if (s != NULL) {
		str = platform_malloc(strlen(s) + 1);
		if (str != NULL)
			strcpy(str, s);
	}

	return str;
}
