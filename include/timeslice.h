/*!
 @file timeslice.h
 @brief Cooperative timeslice scheduler implementation.
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#ifndef __TIMESLICE_H__
#define __TIMESLICE_H__

#include "list.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#endif /* __GNUC__ || __clang__ */

/*!
 @brief Instance structure for timeslice
*/
typedef struct timeslice_s
{
    list_s node[1];
    size_t slice;
    size_t timer;
    void (*exec)(void *);
    void *argv;
    int stat;
} timeslice_s;

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif /* __GNUC__ || __clang__ */

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*!
 @brief A function that requires the tick timer to execute
*/
void timeslice_tick(void);
/*!
 @brief A function that requires the cpu to execute
*/
void timeslice_exec(void);

/*!
 @brief Initialize as a cron task
 @param[in,out] ctx points to an instance of timeslice
 @param[in] exec A function that needs to be executed
 @param[in] argv Arguments to the executed function
 @param[in] slice The length of the time slice
*/
void timeslice_cron(timeslice_s *ctx, void (*exec)(void *), void *argv, size_t slice);
/*!
 @brief Initialize as a once task
 @param[in,out] ctx points to an instance of timeslice
 @param[in] exec A function that needs to be executed
 @param[in] argv Arguments to the executed function
 @param[in] delay The length of delayed execution
*/
void timeslice_once(timeslice_s *ctx, void (*exec)(void *), void *argv, size_t delay);

/*!
 @brief Set the execution function
 @param[in,out] ctx points to an instance of timeslice
 @param[in] exec A function that needs to be executed
*/
void timeslice_set_exec(timeslice_s *ctx, void (*exec)(void *));
/*!
 @brief Set the arguments
 @param[in,out] ctx points to an instance of timeslice
 @param[in] argv Arguments to the executed function
*/
void timeslice_set_argv(timeslice_s *ctx, void *argv);
/*!
 @brief Set the timer
 @param[in,out] ctx points to an instance of timeslice
 @param[in] timer Timer value
*/
void timeslice_set_timer(timeslice_s *ctx, size_t timer);
/*!
 @brief Set the slice
 @param[in,out] ctx points to an instance of timeslice
 @param[in] slice Slice value
*/
void timeslice_set_slice(timeslice_s *ctx, size_t slice);

/*!
 @brief Join a task to the time slice list
 @param[in,out] ctx points to an instance of timeslice
*/
void timeslice_join(timeslice_s *ctx);
/*!
 @brief Drop a task from the time slice list
 @param[in,out] ctx points to an instance of timeslice
*/
void timeslice_drop(timeslice_s *ctx);

/*!
 @brief Testing whether a task is in the time slice list
 @param[in] ctx points to an instance of timeslice
*/
int timeslice_exist(const timeslice_s *ctx);

/*!
 @brief Get the timer value for a task
 @param[in] ctx points to an instance of timeslice
 @return size_t The timer value
*/
size_t timeslice_timer(const timeslice_s *ctx);
/*!
 @brief Get the slice value for a task
 @param[in] ctx points to an instance of timeslice
 @return size_t The slice value
*/
size_t timeslice_slice(const timeslice_s *ctx);
/*!
 @brief Get the count of tasks in the time slice list
 @return size_t The count of tasks
*/
size_t timeslice_count(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __TIMESLICE_H__ */
