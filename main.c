#include <stdio.h>
#include <stdlib.h>

#include <limits.h>
#include <signal.h>

int main(int argc, char* argv[])
{
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

    status = raise(sig);
    if (status != 0) {
        fprintf(stderr,
            "raise(%i) failed with returned status %i.\n", sig, status
        );
        return -1;
    }
    return 0;
}
