#include <stdio.h>
#include <stdlib.h>

#include <limits.h>
#include <signal.h>

typedef void(*p_signal_handler)(int);
static const char* sigtostr(p_signal_handler signal_handler);
extern void trace_signal(int parameter);

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
            "SIGABRT:  %i\n"\
            "SIGFPE :  %i\n"\
            "SIGILL :  %i\n"\
            "SIGINT :  %i\n"\
            "SIGSEGV:  %i\n"\
            "SIGTERM:  %i\n",
            SIGABRT, SIGFPE, SIGILL, SIGINT, SIGSEGV, SIGTERM
        );
        puts(
            "If a third parameter is specified after [signal], an exception-"\
            "recovering handler will be used to trace the raised signal."
        );
        return 0;
    }

    signal_number = strtol(argv[1], NULL, 0);
    if (signal_number < INT_MIN || signal_number > INT_MAX) {
        fprintf(stderr,
            "Signal ID %li exceeds precision for C library input.\n",
            signal_number
        );
        return -1; /* We don't really want to expose raising invalid signals. */
    }
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

static const char* sigtostr(p_signal_handler signal_handler)
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

void trace_signal(int parameter)
{
    printf("Programmer-defined response to signal %i.\n", parameter);
    return;
}
