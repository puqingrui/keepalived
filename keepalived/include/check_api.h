/*
 * Soft:        Keepalived is a failover program for the LVS project
 *              <www.linuxvirtualserver.org>. It monitor & manipulate
 *              a loadbalanced server pool using multi-layer checks.
 *
 * Part:        Checkers arguments structures definitions.
 *
 * Author:      Alexandre Cassen, <acassen@linux-vs.org>
 *
 *              This program is distributed in the hope that it will be useful,
 *              but WITHOUT ANY WARRANTY; without even the implied warranty of
 *              MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *              See the GNU General Public License for more details.
 *
 *              This program is free software; you can redistribute it and/or
 *              modify it under the terms of the GNU General Public License
 *              as published by the Free Software Foundation; either version
 *              2 of the License, or (at your option) any later version.
 *
 * Copyright (C) 2001-2012 Alexandre Cassen, <acassen@gmail.com>
 */

#ifndef _CHECK_API_H
#define _CHECK_API_H

/* global includes */
#include <stdbool.h>

/* local includes */
#include "check_data.h"
#include "scheduler.h"
#include "layer4.h"

/* Checkers structure definition */
typedef struct _checker {
	void				(*free_func) (void *);
	void				(*dump_func) (void *);
	int				(*launch) (struct _thread *);
	bool				(*compare) (void *, void *);
	virtual_server_t		*vs;	/* pointer to the checker thread virtualserver */
	real_server_t			*rs;	/* pointer to the checker thread realserver */
	void				*data;
	checker_id_t			id;	/* Checker identifier */
	int				enabled;/* Activation flag */
	conn_opts_t			*co; /* connection options */
	unsigned long			warmup;	/* max random timeout to start checker */
} checker_t;

/* Checkers queue */
extern list checkers_queue;

/* utility macro */
#define CHECKER_ARG(X) ((X)->data)
#define CHECKER_CO(X) (((checker_t *)X)->co)
#define CHECKER_DATA(X) (((checker_t *)X)->data)
#define CHECKER_GET_CURRENT() (LIST_TAIL_DATA(checkers_queue))
#define CHECKER_GET() (CHECKER_DATA(CHECKER_GET_CURRENT()))
#define CHECKER_GET_CO() (((checker_t *)CHECKER_GET_CURRENT())->co)
#define CHECKER_VALUE_INT(X) (atoi(vector_slot(X,1)))
#define CHECKER_VALUE_UINT(X) ((unsigned)strtoul(vector_slot(X,1), NULL, 10))
#define CHECKER_VALUE_STRING(X) (set_value(X))
#define CHECKER_VHOST(C) (VHOST((C)->vs))
#define CHECKER_HA_SUSPEND(C) ((C)->vs->ha_suspend)
#define CHECKER_NEW_CO() ((conn_opts_t *) MALLOC(sizeof (conn_opts_t)))
#define FMT_CHK(C) FMT_RS((C)->rs, (C)->vs)

/* Prototypes definition */
extern void init_checkers_queue(void);
extern void free_vs_checkers(virtual_server_t *);
extern void dump_conn_opts(void *);
extern void queue_checker(void (*free_func) (void *), void (*dump_func) (void *)
			  , int (*launch) (thread_t *)
			  , bool (*compare) (void *, void *)
			  , void *
			  , conn_opts_t *);
extern bool compare_conn_opts(conn_opts_t *, conn_opts_t *);
extern void dump_checkers_queue(void);
extern void free_checkers_queue(void);
extern void register_checkers_thread(void);
extern void install_checkers_keyword(void);
extern void install_connect_keywords(void);
extern void warmup_handler(vector_t *);
extern void update_checker_activity(sa_family_t, void *, bool);

#endif
