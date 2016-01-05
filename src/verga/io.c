/****************************************************************************
    Copyright (C) 1987-2005 by Jeffery P. Hansen

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    Last edit by hansen on Wed Oct 25 10:50:09 2006
****************************************************************************/
#include <signal.h>
#include <errno.h>
#include "thyme.h"

int do_input_check = 0;			/* Flag to indicate it is time to check input */
static char cmdin_buf[STRMAX];		/* Buffer for unread characters */
static char *cmdin_q = cmdin_buf;	/* End of unread characters */


/*****************************************************************************
 *
 * Notify tkgate that we received an error and wait for tkgate to send us an exit.
 *
 *****************************************************************************/
void waitForExit(void)
{
  char buf[STRMAX];

  vgio_printf("error_exit\n");

  for (;;) {
    if (!fgets(buf,STRMAX,stdout)) break;
    if (strncmp(buf,"exit",4) == 0)
      break;
  }
  exit(0);
}

/*****************************************************************************
 *
 * Send output to the standard out.  If in interactive mode do it carefully
 * so we do not block or loose data.
 *
 * Parameters
 *
 *****************************************************************************/
void vgio_out(const char *prefix,const char *fmt,va_list ap)
{
  char buf[2*STRMAX];
  char *p;
  int l;

  p = buf;
  if (vgsim.vg_interactive && prefix) {
    p += sprintf(p,"%s",prefix);
  }


  vsprintf(p,fmt,ap);
  l = strlen(buf);

  if (vgsim.vg_interactive) {
    p = buf;
    while (l > 0) {
      int c = write(1,p,l);

      if (c > 0) {
	p += c;
	l -= c;
      }
      if (l != 0) {
	fd_set ws;

	FD_ZERO(&ws);
	FD_SET(1,&ws);
	select(2,0,&ws,0,0);
      }
    }
  } else {
    printf("%s",buf);
  }
}

/*****************************************************************************
 *
 * Same as sendMsg without the automatic \n append.
 *
 *****************************************************************************/
void vgio_printf(const char *fmt,...)
{
  va_list ap;

  va_start(ap,fmt);
  vgio_out(0,fmt,ap);
  va_end(ap);
}

void vgio_echo(const char *fmt,...)
{
  va_list ap;

  va_start(ap,fmt);
  vgio_out("echo ",fmt,ap);
  va_end(ap);
}

void vgio_comment(const char *fmt,...)
{
  va_list ap;

  va_start(ap,fmt);
  vgio_out("comment ",fmt,ap);
  va_end(ap);
}


/*****************************************************************************
 *
 * Salve-mode filtering of a string.
 *
 * Parameters:
 *     s		String to be filtered
 *
 * Returns:		s if we are in slave mode, empty string otherwise.
 *
 *****************************************************************************/
const char *slaveStr(const char *s)
{
  if (vgsim.vg_interactive)
    return s;
  else
    return "";
}

/*****************************************************************************
 *
 * Reads data from standard input and stores it in the buffer.
 *
 * Returns:		zero on eof, non-zero otherwise
 *
 *****************************************************************************/
int get_data(void)
{
  int c,en;

  if ((cmdin_q-cmdin_buf) >= STRMAX)
    return 0;

  do {
    errno = 0;
    c = read(0,cmdin_q,STRMAX-(cmdin_q-cmdin_buf));
    en = errno;
  } while (c == 0 && en == EINTR);
  if (c > 0) cmdin_q += c;

  return c != 0;
}

int input_ready(int doWait)
{
  fd_set rd,wr,ex;
  struct timeval tv;
  int r;

  if (cmdin_q != cmdin_buf) return 1;

  FD_ZERO(&rd); FD_ZERO(&wr); FD_ZERO(&ex);
  FD_SET(0,&rd);
  tv.tv_sec = 0;
  tv.tv_usec = 0;

  for (;;) {
    if (doWait)
      r = select(1,&rd,&wr,&ex,0);
    else
      r = select(1,&rd,&wr,&ex,&tv);
    if (r >= 0 || errno != EINTR) break;
  }

  return (r == 1);
}

/*****************************************************************************
 *
 * Read a line of data from the standard input.
 *
 * Parameters:
 *
 *     s		Buffer for reading characters.
 *     n		
 *
 *****************************************************************************/
int get_line(char *s,int n)
{
  char *p;
  int got_nl = 0;

  for (;;) {
    for (p = cmdin_buf;n > 0 && p != cmdin_q;p++) {
      *s++ = *p;
      n--;
      if (n <= 0 || *p == '\n') {
	got_nl = 1;
	p++;
	break;
      }
    }
    if (p != cmdin_q) memmove(cmdin_buf,p,cmdin_q-p);
    cmdin_q = cmdin_buf+(cmdin_q-p);

    if (got_nl) {
      *s = 0;
      if (s[-1] == '\n') s[-1] = 0;
      return 1;
    }

    if (!get_data()) return 0;
  }

  return 0;
}

void timer_event(int p)
{
  do_input_check = 1;
  signal(SIGALRM,timer_event);					/* Reset timer event handler */
}

void input_setup(void)
{
  static int is_init = 0;
  struct itimerval itv;
  int ms = POLL_RATE*1000;
  int s  = POLL_RATE/1000;

  if (is_init) return;
  is_init = 1;

#if 0
  fcntl(0,F_SETFL,O_NONBLOCK|fcntl(0,F_GETFL));			/* Set non-blocking I/O on stdin (does not seem necessary) */
#endif
  itv.it_interval.tv_sec  = itv.it_value.tv_sec  = s;		/* Set up timer interval */
  itv.it_interval.tv_usec = itv.it_value.tv_usec = ms;
  signal(SIGALRM,timer_event);					/* Set timer event handler */
  setitimer(ITIMER_REAL,&itv,0);				/* Enable timer */
}
