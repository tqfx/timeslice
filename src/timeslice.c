/*!
 @file timeslice.c
 @brief Cooperative timeslice scheduler implementation.
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#include "timeslice.h"

#define BIT(ctx, bit) ((ctx)->stat & (bit))
#define SET(ctx, bit) ((ctx)->stat |= (bit))
#define CLR(ctx, bit) ((ctx)->stat &= ~(bit))

/*!
 @brief timeslice flags
*/
enum
{
    TIMESLICE_CTRL = 0x000F, //!< Register for control
    TIMESLICE_EXEC = 1 << 0, //!< Bit that the task needs to execute
    TIMESLICE_STAT = 0x00F0, //!< Register for status
    TIMESLICE_LOCK = 1 << 4, //!< Bit that the task has been locked
    TIMESLICE_TYPE = 0x0F00, //!< Register for type
    TIMESLICE_CRON = 1 << 8, //!< Bit for the cron task
    TIMESLICE_ONCE = 1 << 9, //!< Bit for the once task
};

static struct
{
    list_s running[1];
    timeslice_s *ctx;
    size_t counter;
} local[1] = {{
    {{local->running, local->running}},
    0,
    0,
}};

static inline void timeslice_join_(timeslice_s *ctx)
{
    list_add(local->running, ctx->node);
    ++local->counter;
}

static inline void timeslice_drop_(timeslice_s *ctx)
{
    list_del(ctx->node);
    --local->counter;
}

void timeslice_tick(void)
{
    timeslice_s *ctx;
    list_s *node, *next;
    list_forsafe(node, next, local->running)
    {
        ctx = list_entry(node, timeslice_s, node);
        if (ctx->timer && --ctx->timer == 0)
        {
            SET(ctx, TIMESLICE_EXEC);
            ctx->timer = ctx->slice;
        }
    }
}

void timeslice_exec(void)
{
    list_s *node, *next;
    list_forsafe(node, next, local->running)
    {
        local->ctx = list_entry(node, timeslice_s, node);
        if (BIT(local->ctx, TIMESLICE_EXEC))
        {
            CLR(local->ctx, TIMESLICE_EXEC);
            local->ctx->exec(local->ctx->argv);
            if (BIT(local->ctx, TIMESLICE_ONCE))
            {
                timeslice_drop_(local->ctx);
            }
        }
    }
}

void timeslice_cron(timeslice_s *ctx, void (*exec)(void *), void *argv, size_t slice)
{
    list_init(ctx->node);
    ctx->slice = slice;
    ctx->timer = slice;
    ctx->exec = exec;
    ctx->argv = argv;
    ctx->stat = TIMESLICE_CRON;
}

void timeslice_once(timeslice_s *ctx, void (*exec)(void *), void *argv, size_t delay)
{
    list_init(ctx->node);
    ctx->slice = delay;
    ctx->timer = delay;
    ctx->exec = exec;
    ctx->argv = argv;
    ctx->stat = TIMESLICE_ONCE;
}

void timeslice_set_exec(timeslice_s *ctx, void (*exec)(void *))
{
    ctx = ctx ? ctx : local->ctx;
    ctx->exec = exec;
}
void timeslice_set_argv(timeslice_s *ctx, void *argv)
{
    ctx = ctx ? ctx : local->ctx;
    ctx->argv = argv;
}
void timeslice_set_timer(timeslice_s *ctx, size_t timer)
{
    ctx = ctx ? ctx : local->ctx;
    ctx->timer = timer;
}
void timeslice_set_slice(timeslice_s *ctx, size_t slice)
{
    ctx = ctx ? ctx : local->ctx;
    ctx->slice = slice;
}

void timeslice_join(timeslice_s *ctx)
{
    ctx = ctx ? ctx : local->ctx;
    if (list_null(ctx->node))
    {
        timeslice_join_(ctx);
    }
}

void timeslice_drop(timeslice_s *ctx)
{
    ctx = ctx ? ctx : local->ctx;
    if (list_used(ctx->node))
    {
        timeslice_drop_(ctx);
    }
}

int timeslice_exist(const timeslice_s *ctx)
{
    ctx = ctx ? ctx : local->ctx;
    return list_used(ctx->node);
}

size_t timeslice_timer(const timeslice_s *ctx)
{
    ctx = ctx ? ctx : local->ctx;
    return ctx->timer;
}
size_t timeslice_slice(const timeslice_s *ctx)
{
    ctx = ctx ? ctx : local->ctx;
    return ctx->slice;
}
size_t timeslice_count(void)
{
    return local->counter;
}
