#include "minishell.h"

void sig(int signum) {
    (void)signum;
    write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void signal_handler(void)
{
    struct sigaction sa;
    sa.sa_handler = sig;
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    signal(SIGQUIT, SIG_IGN);
}
