/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readloop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpeng <qpeng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 14:52:01 by qpeng             #+#    #+#             */
/*   Updated: 2019/04/17 14:52:01 by qpeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ping.h"

void    sig_alrm(int signo)
{
    _g.ft_send();
	alarm(1);
	return;	
}

void        error(char *str)
{
    fprintf(stderr, str);
    exit(EXIT_FAILURE);
}

void   tv_sub(struct timeval *out, struct timeval *in)
{
    if ((out->tv_usec -= in->tv_usec) < 0) 
    {    
        --out->tv_sec;
        out->tv_usec += 1000000;
    }
    out->tv_sec -= in->tv_sec;
}



void    readloop(void)
{
    int             _tmp;
    char            recvbuff[BUFF_SIZE];
    char            ctrlbuff[BUFF_SIZE];
   
    gettimeofday(&_g._tv_start, NULL);
    _g.sockfd = socket(_g.ssend->sa_family, SOCK_RAW, IPPROTO_ICMP);
    _tmp = 60 * 1024;
	setsockopt(_g.sockfd , SOL_SOCKET, SO_RCVBUF, &_tmp, sizeof(_tmp));
    _g.iov.iov_base = recvbuff;
    _g.iov.iov_len = sizeof(recvbuff);
    _g.msg.msg_name = _g.srecv;
    _g.msg.msg_iov = &(_g.iov);
    _g.msg.msg_iovlen = 1;
    _g.msg.msg_control = ctrlbuff;
    sig_alrm(SIGALRM);
    while (1)
    {
        _g.msg.msg_namelen = _g.ssendlen;
        _g.msg.msg_controllen = sizeof(ctrlbuff);
        _tmp = recvmsg(_g.sockfd, &(_g.msg), 0);
        if (_tmp < 0)
            if (errno = EINTR)
                continue ; 
            else
                error("recvmsg error");
        _g.ft_recv(_tmp, recvbuff);
    }
}
