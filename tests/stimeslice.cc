/*!
 @file stimeslice.cc
 @brief Tesing cooperative scheduler timeslice.
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#include "stimeslice.h"

#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <thread>

static size_t step = 0;
static size_t ref[5] = {0};
static stimeslice_s stimeslice[5];

static void stimeslice1_exec(void *arg)
{
    size_t *p = static_cast<size_t *>(arg) + 0;
    if (++*p % 2 == 0)
    {
        stimeslice_drop(stimeslice + 0);
        stimeslice_drop(stimeslice + 0);
    }
}

static void stimeslice2_exec(void *arg)
{
    size_t *p = static_cast<size_t *>(arg) + 1;
    if (++*p % 2 == 0)
    {
        stimeslice_join(stimeslice + 0);
        stimeslice_join(stimeslice + 0);
    }
}

static void stimeslice3_exec(void *arg)
{
    ++*(static_cast<size_t *>(arg) + 2);
}

static void stimeslice4_exec(void *arg)
{
    ++*(static_cast<size_t *>(arg) + 3);
}

static void stimeslice5_exec(void *arg)
{
    ++*(static_cast<size_t *>(arg) + 4);
}

static void stimeslice_show(void *arg)
{
    printf("\r\n");
    printf("\rtask1 %zu\n", *(static_cast<size_t *>(arg) + 0));
    printf("\rtask2 %zu\n", *(static_cast<size_t *>(arg) + 1));
    printf("\rtask3 %zu\n", *(static_cast<size_t *>(arg) + 2));
    printf("\rtask4 %zu\n", *(static_cast<size_t *>(arg) + 3));
    printf("\rtask5 %zu\n", *(static_cast<size_t *>(arg) + 4));
    printf("\r\033[6A");
}

[[noreturn]] static void stimeslice_tick_thread(void)
{
    for (size_t n = 0; n != step; ++n)
    {
        stimeslice_tick();
        usleep(1);
        if (n % 1000)
        {
            continue;
        }
        printf("\rcount %zu tick %zu", stimeslice_count(), n);
        stimeslice_show(ref);
    }
    printf("\n\n\n\n\n\n");
    exit(0);
}

[[noreturn]] static void stimeslice_exec_thread(void)
{
    stimeslice_cron(stimeslice + 0, stimeslice1_exec, ref, 10);
    stimeslice_cron(stimeslice + 1, stimeslice2_exec, ref, 20);
    stimeslice_cron(stimeslice + 2, stimeslice3_exec, ref, 30);
    stimeslice_cron(stimeslice + 3, stimeslice4_exec, ref, 40);
    stimeslice_cron(stimeslice + 4, stimeslice5_exec, ref, 50);
    stimeslice_join(stimeslice + 0);
    stimeslice_join(stimeslice + 1);
    stimeslice_join(stimeslice + 2);
    stimeslice_join(stimeslice + 3);
    stimeslice_join(stimeslice + 4);

    stimeslice_drop(stimeslice + 1);
    if (stimeslice_exist(stimeslice + 1))
    {
        printf("failure in %s %i\n", __FILE__, __LINE__);
    }

    stimeslice_drop(stimeslice + 4);
    if (stimeslice_count() != 3)
    {
        printf("failure in %s %i\n", __FILE__, __LINE__);
    }
    if (stimeslice_exist(stimeslice + 2) == 0)
    {
        printf("failure in %s %i\n", __FILE__, __LINE__);
    }
    stimeslice_join(stimeslice + 1);
    if (stimeslice_exist(stimeslice + 1) == 0)
    {
        printf("failure in %s %i\n", __FILE__, __LINE__);
    }
    stimeslice_join(stimeslice + 4);

    while (true)
    {
        stimeslice_exec();
    }
}

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        step = static_cast<size_t>(atoi(argv[1]));
    }

    std::thread thread_1(stimeslice_exec_thread);
    std::thread thread_2(stimeslice_tick_thread);

    thread_1.join();
    thread_2.join();

    return 0;
}
