/*!
 @file stimeslice.h
 @brief Cooperative timeslice scheduler implementation.
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#ifndef __STIMESLICE_H__
#define __STIMESLICE_H__

#include "slist.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#endif /* __GNUC__ || __clang__ */

/*!
 @brief Instance structure for timeslice
*/
typedef struct stimeslice_s
{
    slist_u node[1];
    size_t slice;
    size_t timer;
    void (*exec)(void *);
    void *argv;
    int stat;
} stimeslice_s;

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif /* __GNUC__ || __clang__ */

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*!
 @brief A function that requires the tick timer to execute
*/
void stimeslice_tick(void);
/*!
 @brief A function that requires the cpu to execute
*/
void stimeslice_exec(void);

/*!
 @brief Initialize as a cron task
 @param[in,out] ctx points to an instance of timeslice
 @param[in] exec A function that needs to be executed
 @param[in] argv Arguments to the executed function
 @param[in] slice The length of the time slice
*/
void stimeslice_cron(stimeslice_s *ctx, void (*exec)(void *), void *argv, size_t slice);
/*!
 @brief Initialize as a once task
 @param[in,out] ctx points to an instance of timeslice
 @param[in] exec A function that needs to be executed
 @param[in] argv Arguments to the executed function
 @param[in] delay The length of delayed execution
*/
void stimeslice_once(stimeslice_s *ctx, void (*exec)(void *), void *argv, size_t delay);

/*!
 @brief Set the execution function
 @param[in,out] ctx points to an instance of timeslice
 @param[in] exec A function that needs to be executed
*/
void stimeslice_set_exec(stimeslice_s *ctx, void (*exec)(void *));
/*!
 @brief Set the arguments
 @param[in,out] ctx points to an instance of timeslice
 @param[in] argv Arguments to the executed function
*/
void stimeslice_set_argv(stimeslice_s *ctx, void *argv);
/*!
 @brief Set the timer
 @param[in,out] ctx points to an instance of timeslice
 @param[in] timer Timer value
*/
void stimeslice_set_timer(stimeslice_s *ctx, size_t timer);
/*!
 @brief Set the slice
 @param[in,out] ctx points to an instance of timeslice
 @param[in] slice Slice value
*/
void stimeslice_set_slice(stimeslice_s *ctx, size_t slice);

/*!
 @brief Join a task to the time slice list
 @param[in,out] ctx points to an instance of timeslice
*/
void stimeslice_join(stimeslice_s *ctx);
/*!
 @brief Drop a task from the time slice list
 @param[in,out] ctx points to an instance of timeslice
*/
void stimeslice_drop(stimeslice_s *ctx);

/*!
 @brief Testing whether a task is in the time slice list
 @param[in] ctx points to an instance of timeslice
*/
int stimeslice_exist(const stimeslice_s *ctx);

/*!
 @brief Get the timer value for a task
 @param[in] ctx points to an instance of timeslice
 @return size_t The timer value
*/
size_t stimeslice_timer(const stimeslice_s *ctx);
/*!
 @brief Get the slice value for a task
 @param[in] ctx points to an instance of timeslice
 @return size_t The slice value
*/
size_t stimeslice_slice(const stimeslice_s *ctx);
/*!
 @brief Get the count of tasks in the time slice list
 @return size_t The count of tasks
*/
size_t stimeslice_count(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __STIMESLICE_H__ */
