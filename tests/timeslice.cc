/*!
 @file timeslice.cc
 @brief Tesing cooperative scheduler timeslice.
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#include "timeslice.h"

#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <thread>

static size_t step = 0;
static size_t ref[5] = {0};
static timeslice_s timeslice[5];

static void timeslice1_exec(void *arg)
{
    size_t *p = static_cast<size_t *>(arg) + 0;
    if (++*p % 2 == 0)
    {
        timeslice_drop(timeslice + 0);
        timeslice_drop(timeslice + 0);
    }
}

static void timeslice2_exec(void *arg)
{
    size_t *p = static_cast<size_t *>(arg) + 1;
    if (++*p % 2 == 0)
    {
        timeslice_join(timeslice + 0);
        timeslice_join(timeslice + 0);
    }
}

static void timeslice3_exec(void *arg)
{
    ++*(static_cast<size_t *>(arg) + 2);
}

static void timeslice4_exec(void *arg)
{
    ++*(static_cast<size_t *>(arg) + 3);
}

static void timeslice5_exec(void *arg)
{
    ++*(static_cast<size_t *>(arg) + 4);
}

static void timeslice_show(void *arg)
{
    printf("\r\n");
    printf("\rtask1 %zu\n", *(static_cast<size_t *>(arg) + 0));
    printf("\rtask2 %zu\n", *(static_cast<size_t *>(arg) + 1));
    printf("\rtask3 %zu\n", *(static_cast<size_t *>(arg) + 2));
    printf("\rtask4 %zu\n", *(static_cast<size_t *>(arg) + 3));
    printf("\rtask5 %zu\n", *(static_cast<size_t *>(arg) + 4));
    printf("\r\033[6A");
}

[[noreturn]] static void timeslice_tick_thread(void)
{
    for (size_t n = 0; n != step; ++n)
    {
        timeslice_tick();
        if (n % 1000)
        {
            continue;
        }
        printf("\rcount %zu tick %zu", timeslice_count(), n);
        timeslice_show(ref);
    }
    printf("\n\n\n\n\n\n");
    exit(0);
}

[[noreturn]] static void timeslice_exec_thread(void)
{
    timeslice_cron(timeslice + 0, timeslice1_exec, ref, 10);
    timeslice_cron(timeslice + 1, timeslice2_exec, ref, 20);
    timeslice_cron(timeslice + 2, timeslice3_exec, ref, 30);
    timeslice_cron(timeslice + 3, timeslice4_exec, ref, 40);
    timeslice_cron(timeslice + 4, timeslice5_exec, ref, 50);
    timeslice_join(timeslice + 0);
    timeslice_join(timeslice + 1);
    timeslice_join(timeslice + 2);
    timeslice_join(timeslice + 3);
    timeslice_join(timeslice + 4);

    timeslice_drop(timeslice + 1);
    if (timeslice_exist(timeslice + 1))
    {
        printf("failure in %s %i\n", __FILE__, __LINE__);
    }

    timeslice_drop(timeslice + 4);
    if (timeslice_count() != 3)
    {
        printf("failure in %s %i\n", __FILE__, __LINE__);
    }
    if (timeslice_exist(timeslice + 2) == 0)
    {
        printf("failure in %s %i\n", __FILE__, __LINE__);
    }
    timeslice_join(timeslice + 1);
    if (timeslice_exist(timeslice + 1) == 0)
    {
        printf("failure in %s %i\n", __FILE__, __LINE__);
    }
    timeslice_join(timeslice + 4);

    while (true)
    {
        timeslice_exec();
    }
}

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        step = static_cast<size_t>(atoi(argv[1]));
    }

    std::thread thread_1(timeslice_exec_thread);
    std::thread thread_2(timeslice_tick_thread);

    thread_1.join();
    thread_2.join();

    return 0;
}
