/*!
 @file slist.h
 @brief Circular singly linked list implementation.
 @see https://github.com/tqfx/liba/blob/master/include/misc/a_slist.h
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#pragma once
#ifndef __SLIST_H__
#define __SLIST_H__

#include <stddef.h>

/*!
 @brief Instance structure for circular singly linked list node
*/
typedef union slist_u
{
    union slist_u *next;
} slist_u;

/*!
 @brief Instance structure for circular singly linked list head
*/
typedef struct slist_s
{
    slist_u head[1];
    slist_u *tail;
} slist_s;

/*!
 @brief Get the struct for this entry
 @param ptr the &slist_u pointer
 @param type the type of the struct this is embedded in
 @param member the name of the slist_u within the struct
*/
#define slist_entry(ptr, type, member) ((type *)((size_t)(ptr)-offsetof(type, member)))

/*!
 @brief Iterate over a list
 @param ptr the &slist_u to use as a loop counter
 @param ctx points to circular singly linked list head
*/
#define slist_foreach(ptr, ctx) \
    for ((ptr) = (ctx)->head->next; (ptr) != (ctx)->head; (ptr) = (ptr)->next)

/*!
 @brief Iterate over a list safe against removal of list entry
 @param ptr the &slist_u to use as a loop counter
 @param pre another &slist_u to use as temporary storage
 @param ctx points to circular singly linked list head
*/
#define slist_forsafe(ptr, pre, ctx)                   \
    for ((pre) = (ctx)->head, (ptr) = (pre)->next;     \
         (ptr) != (ctx)->head && (ptr) != (ptr)->next; \
         (pre) = ((pre)->next == (ptr) ? (ptr) : (pre)), (ptr) = (pre)->next)

/*!
 @brief initialize for circular singly linked list node
 @param[in,out] ctx points to circular singly linked list node
*/
static inline void slist_init(slist_u *ctx) { ctx->next = ctx; }

/*!
 @brief Testing whether a list node is null
 @param[in] ctx points to circular singly linked list node
 @return int bool
  @retval 0 non-null
  @retval 1 null
*/
static inline int slist_null(const slist_u *ctx) { return ctx->next == ctx; }
/*!
 @brief Testing whether a list node is used
 @param[in] ctx points to circular doubly linked list
 @return int bool
  @retval 0 unused
  @retval 1 used
*/
static inline int slist_used(const slist_u *ctx) { return ctx->next != ctx; }
/*!
 @brief Testing whether a list head is none
 @param[in,out] ctx points to circular singly linked list head
 @return int bool
  @retval 0 exist
  @retval 1 none
*/
static inline int slist_none(const slist_s *ctx) { return ctx->head == ctx->head->next; }
/*!
 @brief Testing whether a list node is a tail node
 @param[in] ctx points to circular singly linked list head
 @param[in] node a list node
 @return int bool
  @retval 0 it's not a tail node
  @retval 1 it is a tail node
*/
static inline int slist_tail(const slist_s *ctx, const slist_u *node) { return ctx->head == node->next; }

/*!
 @brief Link head node and tail node node
 @param[in,out] head The head node of a list
 @param[in,out] tail The tail node of a list
*/
static inline void slist_link(slist_u *head, slist_u *tail) { head->next = tail; }

/*!
 @brief Insert a node to a list tail
 @param[in,out] ctx points to circular singly linked list head
 @param[in] node a list node
*/
static inline void slist_add(slist_s *ctx, slist_u *node)
{
    slist_link(node, ctx->head);
    slist_link(ctx->tail, node);
    ctx->tail = node;
}

/*!
 @brief Delete a node from a list
 @param[in,out] ctx points to circular singly linked list head
 @param[in] prev previous list node
*/
static inline void slist_del(slist_s *ctx, slist_u *prev)
{
    slist_u *node = prev->next;
    slist_link(prev, node->next);
    if (slist_tail(ctx, node))
    {
        ctx->tail = prev;
    }
    slist_init(node);
}

#endif /* __SLIST_H__ */
