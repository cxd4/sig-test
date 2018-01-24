#include <stdio.h>
#include <stdlib.h>

#include <limits.h>
#include <signal.h>

#include "catch.h"

int main(int argc, char* argv[])
{
    p_signal_handler signal_handler;
    long signal_number;
    int sig; /* signal_number converted to (int) precision */
    int status;

    if (argc < 2) {
        printf("Command syntax:  %s [signal]\n", argv[0]);
        puts("Values for standard signals on this operating system:");
        printf(
            "SIGABRT = %2i\n"\
            "SIGFPE  = %2i\n"\
            "SIGILL  = %2i\n"\
            "SIGINT  = %2i\n"\
            "SIGSEGV = %2i\n"\
            "SIGTERM = %2i\n",
            SIGABRT, SIGFPE, SIGILL, SIGINT, SIGSEGV, SIGTERM
        );
        puts("Beginning tests for all standard signals.");
        test_all_signals();
        return 0;
    }

    signal_number = strtol(argv[1], NULL, 0);
#if (LONG_MAX > INT_MAX || LONG_MIN < INT_MIN)
    if (signal_number < INT_MIN || signal_number > INT_MAX) {
        fprintf(stderr,
            "Signal ID %li exceeds precision for C library input.\n",
            signal_number
        );
        return -1; /* We don't really want to expose raising invalid signals. */
    }
#endif
    sig = (int)(signal_number);

    signal_handler = signal(sig, SIG_DFL);
    printf("Handler for signal %i:  %s\n", sig, sigtostr(signal_handler));
    if (argc > 2) {
        signal_handler = signal(sig, trace_signal);
        if (signal_handler == SIG_ERR)
            fprintf(stderr, "Error defining handler for signal %i.\n", sig);
    }

    status = raise(sig);
    if (status != 0) {
        fprintf(stderr,
            "raise(%i) failed with returned status %i.\n", sig, status
        );
        return -1;
    }
    puts("No unhandled exceptions encountered.  Program execution complete.");
    return 0;
}
