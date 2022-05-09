/*!
 @file list.h
 @brief Simple circular doubly linked list implementation.
 @see https://github.com/tqfx/liba/blob/master/include/misc/a_list.h
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#pragma once
#ifndef __LIST_H__
#define __LIST_H__

#include <stddef.h>

/*!
 @brief Instance structure for circular doubly linked list
*/
typedef struct list_s
{
    struct list_s *next, *prev;
} list_s;

/*!
 @brief Get the struct for this entry
 @param ptr the &list_s pointer
 @param type the type of the struct this is embedded in
 @param member the name of the list_s within the struct
*/
#define list_entry(ptr, type, member) ((type *)((size_t)(ptr)-offsetof(type, member)))

/*!
 @brief Iterate over a list
 @param ptr the &list_s to use as a loop counter
 @param ctx points to circular doubly linked list
*/
#define list_foreach(ptr, ctx) \
    for ((ptr) = (ctx)->next; (ptr) != (ctx); (ptr) = (ptr)->next)

/*!
 @brief Iterate over a list safe against removal of list entry
 @param ptr the &list_s to use as a loop counter
 @param pre another &list_s to use as temporary storage
 @param ctx points to circular doubly linked list
*/
#define list_forsafe(ptr, pre, ctx)                \
    for ((ptr) = (ctx)->next, (pre) = (ptr)->next; \
         (ptr) != (ctx) && (ptr) != (ptr)->next;   \
         (ptr) = (pre), (pre) = (ptr)->next)

/*!
 @brief initialize for circular doubly linked list
 @param[in,out] ctx points to circular doubly linked list
*/
static inline void list_init(list_s *ctx) { ctx->prev = ctx->next = ctx; }

/*!
 @brief Testing whether a list is null
 @param[in] ctx points to circular doubly linked list
 @return int bool
  @retval 0 non-null
  @retval 1 null
*/
static inline int list_null(const list_s *ctx) { return ctx->next == ctx; }
/*!
 @brief Testing whether a list is used
 @param[in] ctx points to circular doubly linked list
 @return int bool
  @retval 0 unused
  @retval 1 used
*/
static inline int list_used(const list_s *ctx) { return ctx->next != ctx; }

/*!
 @brief Link head node and tail node node
 @param[in,out] head The head node of a list
 @param[in,out] tail The tail node of a list
*/
static inline void list_link(list_s *head, list_s *tail)
{
    head->next = tail;
    tail->prev = head;
}

/*!
 @brief Insert a node to a list backward
 @param[in,out] ctx points to circular doubly linked list
 @param[in] node a list node
*/
static inline void list_add(list_s *ctx, list_s *node)
{
    list_link(ctx->prev, node);
    list_link(node, ctx);
}

/*!
 @brief Delete a node from a list
 @param[in] node a list node
*/
static inline void list_del(list_s *node)
{
    list_link(node->prev, node->next);
    list_init(node);
}

#endif /* __LIST_H__ */
