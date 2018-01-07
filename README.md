# sig-test
Utility to test raising operating-system-defined signals and recover from exceptions.

### Signals Defined by the C Standard

The following signal numbers are universal to all implementations of the C programming language.  It's a given that there will be other signals on the operating system you use (Microsoft Windows, etc.), but only the ones below are universal to all C implementations.

| Constant |          Description          |  Example Cause in C Code 
| -------- | ----------------------------- | -------------------------
| SIGABRT  | Abnormal process termination. | `abort();`
| SIGFPE   | Arithmetic or FPU exception.  | `return (++x / 0);`
| SIGILL   | Illegal instruction.          |
| SIGINT   | Program interrupt.            | `getchar(); /* Ctrl+C */`
| SIGSEGV  | Memory access violation.      | `memcpy(&byte, NULL, 1);`
| SIGTERM  | Termination requested.        | `raise(SIGTERM);`

### Exact Numbers for the Signals

Each signal constant is defined to be some integer or other.  This will always vary between each operating system.

To list the exact number definitions on your system, execute this utility via just the minimal command:  `sig-test`.

Example output of the above on UNIX:
```
Values for standard signals on this operating system:
SIGABRT =  6
SIGFPE  =  8
SIGILL  =  4
SIGINT  =  2
SIGSEGV = 11
SIGTERM = 15
```

On DOS and Microsoft Windows:
```
Values for standard signals on this operating system:
SIGABRT = 22
SIGFPE  =  8
SIGILL  =  4
SIGINT  =  2
SIGSEGV = 11
SIGTERM = 15
```

### SIGILL

Causing this signal programmatically without issuing `raise(SIGILL);` is not very easy to do, because the assembler and a C compiler are both responsible for generating valid CPU operation commands from valid zeroes and ones for that CPU.  It is possible to direct the compiler or assembler to optimize or generate code for a newer version of the instruction set architecture than that being used by the person executing the program (an older version, where that instruction operation code is invalid and reserved for future implementation).  It is also possible this can be triggered if the CPU architecture in question allows self-modifying code at any point in time, and the function being executed has its binary modified with trivial bytes during the program's course.
