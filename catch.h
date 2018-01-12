#ifndef _CATCH_H_
#define _CATCH_H_

typedef void(*p_signal_handler)(int);
typedef void(*p_func)(void);

extern const char* sigtostr(p_signal_handler signal_handler);
extern void test_all_signals(void);
extern void trace_signal(int parameter);
extern void catch_signal(int parameter);

#endif
