/*
** main.c for PSU_2016_minishell2 in /home/nikola/delivery/PSU_2016_minishell2
**
** Made by nikola.tomic@epitech.eu
** Login   <nikola@epitech.net>
**
** Started on  Mon Apr 03 13:49:13 2017 nikola.tomic@epitech.eu
** Last update Sun May 21 15:45:27 2017 
*/

#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include "define.h"
#include "my.h"

extern char	**environ;

void			check_status(int *ret)
{
  int			status;
  int			sig;

  status = 0;
  wait(&status);
  if (WIFEXITED(status))
    *ret = WEXITSTATUS(status);
  else if (WIFSIGNALED(status))
    {
      sig = WTERMSIG(status);
      if (sig == SIGABRT)
	my_putstr(ABORT);
      else if (sig == SIGFPE)
	my_putstr(FLOAT);
      else if (sig == SIGSEGV)
	my_putstr(SEV);
      else
	my_putstr(SIGNAL);
      if (WCOREDUMP(status))
	my_putstr(CORE);
      my_putchar('\n');
      *ret = sig + 128;
    }
}

int			main(void)
{
  struct termios	old;
  t_env			my_env;

  signal(SIGINT, sigint_handler);
  ioctl(0, TCGETS, &old);
  my_env.ret = 0;
  my_env.env = tab_to_list(environ);
  if (via_pipe(NULL) == 0)
    routine_ioctl();
  mainloop(&my_env);
  free_list(my_env.env);
  if (!via_pipe(NULL))
    my_putchar('\n');
  ioctl(0, TCSETS, &old);
  return (my_env.ret);
}
