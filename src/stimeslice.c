/*!
 @file stimeslice.c
 @brief Cooperative timeslice scheduler implementation.
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#include "stimeslice.h"

#define BIT(ctx, bit) ((ctx)->stat & (bit))
#define SET(ctx, bit) ((ctx)->stat |= (bit))
#define CLR(ctx, bit) ((ctx)->stat &= ~(bit))
#define HAS(ctx, bit) (((ctx)->stat & (bit)) == (bit))
#define NOT(ctx, bit) (((ctx)->stat & (bit)) != (bit))

/*!
 @brief timeslice flags
*/
enum
{
    STIMESLICE_CTRL = 0x000F, //!< Register for control
    STIMESLICE_EXEC = 1 << 0, //!< Bit that the task needs to execute
    STIMESLICE_JOIN = 1 << 1, //!< Bit which the task has been joined
    STIMESLICE_STAT = 0x00F0, //!< Register for status
    STIMESLICE_LOCK = 1 << 4, //!< Bit that the task has been locked
    STIMESLICE_TYPE = 0x0F00, //!< Register for type
    STIMESLICE_CRON = 1 << 8, //!< Bit for the cron task
    STIMESLICE_ONCE = 1 << 9, //!< Bit for the once task
};

static struct
{
    slist_s running[1];
    stimeslice_s *ctx;
    size_t counter;
} local[1] = {{
    {{
        {{local->running->head}},
        local->running->head,
    }},
    0,
    0,
}};

void stimeslice_tick(void)
{
    stimeslice_s *ctx;
    slist_u *node, *prev;
    slist_forsafe(node, prev, local->running)
    {
        ctx = slist_entry(node, stimeslice_s, node);
        if (ctx->timer && --ctx->timer == 0)
        {
            SET(ctx, STIMESLICE_EXEC);
            ctx->timer = ctx->slice;
        }
    }
}

void stimeslice_exec(void)
{
    slist_u *node, *prev;
    slist_forsafe(node, prev, local->running)
    {
        local->ctx = slist_entry(node, stimeslice_s, node);
        if (NOT(local->ctx, STIMESLICE_JOIN))
        {
            slist_del(local->running, prev);
            continue;
        }
        if (BIT(local->ctx, STIMESLICE_EXEC))
        {
            CLR(local->ctx, STIMESLICE_EXEC);
            local->ctx->exec(local->ctx->argv);
            if (BIT(local->ctx, STIMESLICE_ONCE))
            {
                stimeslice_drop(local->ctx);
            }
        }
    }
}

void stimeslice_cron(stimeslice_s *ctx, void (*exec)(void *), void *argv, size_t slice)
{
    slist_init(ctx->node);
    ctx->slice = slice;
    ctx->timer = slice;
    ctx->exec = exec;
    ctx->argv = argv;
    ctx->stat = STIMESLICE_CRON;
}

void stimeslice_once(stimeslice_s *ctx, void (*exec)(void *), void *argv, size_t delay)
{
    slist_init(ctx->node);
    ctx->slice = delay;
    ctx->timer = delay;
    ctx->exec = exec;
    ctx->argv = argv;
    ctx->stat = STIMESLICE_ONCE;
}

void stimeslice_set_exec(stimeslice_s *ctx, void (*exec)(void *))
{
    ctx = ctx ? ctx : local->ctx;
    ctx->exec = exec;
}
void stimeslice_set_argv(stimeslice_s *ctx, void *argv)
{
    ctx = ctx ? ctx : local->ctx;
    ctx->argv = argv;
}
void stimeslice_set_timer(stimeslice_s *ctx, size_t timer)
{
    ctx = ctx ? ctx : local->ctx;
    ctx->timer = timer;
}
void stimeslice_set_slice(stimeslice_s *ctx, size_t slice)
{
    ctx = ctx ? ctx : local->ctx;
    ctx->slice = slice;
}

void stimeslice_join(stimeslice_s *ctx)
{
    ctx = ctx ? ctx : local->ctx;
    if (NOT(ctx, STIMESLICE_JOIN))
    {
        SET(ctx, STIMESLICE_JOIN);
        if (slist_null(ctx->node))
        {
            slist_add(local->running, ctx->node);
        }
        ++local->counter;
    }
}

void stimeslice_drop(stimeslice_s *ctx)
{
    ctx = ctx ? ctx : local->ctx;
    if (HAS(ctx, STIMESLICE_JOIN))
    {
        CLR(ctx, STIMESLICE_CTRL);
        --local->counter;
    }
}

int stimeslice_exist(const stimeslice_s *ctx)
{
    ctx = ctx ? ctx : local->ctx;
    return HAS(ctx, STIMESLICE_JOIN);
}

size_t stimeslice_timer(const stimeslice_s *ctx)
{
    ctx = ctx ? ctx : local->ctx;
    return ctx->timer;
}
size_t stimeslice_slice(const stimeslice_s *ctx)
{
    ctx = ctx ? ctx : local->ctx;
    return ctx->slice;
}
size_t stimeslice_count(void)
{
    return local->counter;
}
