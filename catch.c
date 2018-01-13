#include <stdio.h>
#include <stdlib.h>

#include <setjmp.h>
#include <signal.h>

#include "catch.h"

const char* sigtostr(p_signal_handler signal_handler)
{
    if (signal_handler == SIG_ERR)
        return "SIG_ERR";
    if (signal_handler == SIG_IGN)
        return "SIG_IGN";
    if (signal_handler == SIG_DFL)
        return "SIG_DFL";
    if (signal_handler == NULL)
        return "NULL";
    return "(unknown)";
}

static jmp_buf CPU_state;
void trace_signal(int parameter)
{
    printf("Programmer-defined response to signal %i.\n", parameter);
    return;
}
void catch_signal(int parameter)
{
    putchar('\n');
    printf("Caught exception from raised signal %i.\n", parameter);
    longjmp(CPU_state, parameter);
}
void dummy_func(void)
{
    return;
}
void illegal_inst(void)
{
    raise(SIGILL);
}

static p_signal_handler* NULL_ptr = NULL;
static unsigned char a_scratch = 1, b_scratch = 0;
static p_func func_ptr = illegal_inst;
void test_all_signals(void)
{
    p_signal_handler set_handlers[6];
    int recovered_from_exception;
    register unsigned int i;

    set_handlers[0] = signal(SIGABRT, catch_signal);
    set_handlers[1] = signal(SIGFPE,  catch_signal);
    set_handlers[2] = signal(SIGILL,  catch_signal);
    set_handlers[3] = signal(SIGINT,  catch_signal);
    set_handlers[4] = signal(SIGSEGV, catch_signal);
    set_handlers[5] = signal(SIGTERM, catch_signal);
    for (i = 0; i < sizeof(set_handlers) / sizeof(set_handlers[0]); i++)
        if (set_handlers[i] == SIG_ERR)
            fprintf(
                stderr, "Failed to set exception handler for signal %u.\n", i
            );

    puts("Testing program termination signal...");
    recovered_from_exception = setjmp(CPU_state);
    if (recovered_from_exception)
        set_handlers[5] = signal(SIGTERM, SIG_IGN);
    raise(SIGTERM);

    puts("Testing program abortion signal...");
    recovered_from_exception = setjmp(CPU_state);
    if (recovered_from_exception)
        { /* branch */ } else
    abort();

    puts("Testing user-generated interrupt...");
    recovered_from_exception = setjmp(CPU_state);
    if (recovered_from_exception)
        goto esc_loop;
    puts("(Entering an infinite loop.  User should press Ctrl-C to recover.)");
    for (;;)
        continue;
esc_loop:

    puts("Testing illegal instruction exception...");
    recovered_from_exception = setjmp(CPU_state);
    if (recovered_from_exception)
        func_ptr = dummy_func;
    func_ptr();

    puts("Testing segmentation violation signal...");
    recovered_from_exception = setjmp(CPU_state);
    if (recovered_from_exception)
        NULL_ptr = &(set_handlers[1]);
    set_handlers[0] = *(NULL_ptr);

    puts("Testing FPU exception signal...");
    recovered_from_exception = setjmp(CPU_state);
    if (recovered_from_exception)
        b_scratch = 2;
    a_scratch /= b_scratch;

    puts("Testing finished.  All signal traps were recovered from.");
    return;
}
