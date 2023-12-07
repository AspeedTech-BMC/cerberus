/*
 * Copyright (c) 2022 ASPEED Technology Inc.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <stdint.h>
#include <zephyr/kernel.h>
#include "platform_compiler.h"


/* Zephyr memory management. */
#define	platform_malloc		malloc
#define	platform_calloc		calloc
#define	platform_realloc	realloc
#define	platform_free		free

#define platform_htons _platform_htons

/* Zephyr internet operations. */
uint32_t platform_htonl(uint32_t host_long);
uint16_t _platform_htons(uint16_t host_short);

/* Zephyr msleep. */
void platform_msleep(uint32_t msec);

/* Zephyr timing. */
typedef int64_t platform_clock;

int platform_init_timeout(uint32_t msec, platform_clock *timeout);
int platform_increase_timeout(uint32_t msec, platform_clock *timeout);
int platform_init_current_tick(platform_clock *currtime);
int platform_has_timeout_expired(const platform_clock *timeout);
uint64_t platform_get_time(void);
uint32_t platform_get_duration(const platform_clock *start, const platform_clock *end);

/* Zephyr mutex. */
typedef struct k_sem platform_mutex;

int platform_mutex_init(platform_mutex *mutex);
int platform_mutex_free(platform_mutex *mutex);
int platform_mutex_lock(platform_mutex *mutex);
int platform_mutex_unlock(platform_mutex *mutex);

/* Recursive mutex. */
#define	platform_recursive_mutex_init		platform_mutex_init
#define	platform_recursive_mutex_free		platform_mutex_free
#define	platform_recursive_mutex_lock		platform_mutex_lock
#define	platform_recursive_mutex_unlock		platform_mutex_unlock

/* Zephyr timer. */
typedef void (*timer_callback) (void *context);
typedef struct {
	struct k_timer timer;
	struct k_work timer_worker;
	struct k_sem disarm_lock;
	timer_callback callback;
	void *context;
	uint8_t disarm;
	uint8_t init;
} platform_timer;

int platform_timer_create(platform_timer *timer, timer_callback callback, void *context);
int platform_timer_arm_one_shot(platform_timer *timer, uint32_t ms_timeout);
int platform_timer_disarm(platform_timer *timer);
void platform_timer_delete(platform_timer *timer);

/* Zephyr semaphore. */
typedef struct k_sem platform_semaphore;
int platform_semaphore_init(platform_semaphore *sem);
void platform_semaphore_free(platform_semaphore *sem);
int platform_semaphore_post(platform_semaphore *sem);
int platform_semaphore_wait(platform_semaphore *sem, uint32_t ms_timeout);
int platform_semaphore_try_wait(platform_semaphore *sem);
int platform_semaphore_reset(platform_semaphore *sem);

