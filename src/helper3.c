/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpeng <qpeng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 00:29:46 by qpeng             #+#    #+#             */
/*   Updated: 2019/05/08 05:43:32 by qpeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include "ping.h"

extern const char * const   sys_errlist[];
extern int            sys_nerr;
extern int                  errno;

void        perror_(const char *s)
{    
    if (errno < 0 || errno >= sys_nerr
        || sys_errlist[errno] == NULL)
        fprintf(stderr, "Unknown error : %d\n", errno);
    else
        fprintf(stderr, "%s: %s\n", s, 
                                    sys_errlist[errno]);
}

int         strcmp_(const char *rdi, const char *rsi)
{
    while (*rdi == *rsi++)
		if (*rdi++ == 0)
			return (0);
	return (*(unsigned char *)rdi - *(unsigned char *)--rsi);
}

void        bzero_(void *rsi, size_t rcx)
{
    char    *rdi;

    rdi = (char *)rsi;
    while (rcx--)
    {
        *rdi = 0;
        rdi++;
    }
}

char        *strcpy_(char *rdi, const char *rsi)
{
    char *rax = rdi;
    while(*rdi++ = *rsi++)
        ;
    return (rax);
}