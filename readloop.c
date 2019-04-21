/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readloop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpeng <qpeng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 14:52:01 by qpeng             #+#    #+#             */
/*   Updated: 2019/04/20 22:47:01 by qpeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ping.h"

void    sig_alrm(int signo)
{
    if (_g.times--)
	    alarm(_g.duration);
    else
        sig_int(SIGINT);
    _g.ft_send();
	return;	
}

void                sig_int(int signo)
{
    int         diff;
    double      loss;
    double      rrt;

    gettimeofday(&_g._tv_end,NULL);
    tv_sub(&_g._tv_end, &_g._tv_start);
    rrt = _g._tv_end.tv_sec * 1000.0 +  _g._tv_end.tv_usec / 1000.0;
    diff = _g.msg_cnt - _g.pkg_received;
    loss = (double)diff / _g.msg_cnt * 100;
    printf("\n--- %s ping statistics ---\n", _g.host);
    printf("%d packets transmitted, %d received, %.0f%% packet loss time %.0f ms\n",\
    _g.msg_cnt, _g.pkg_received, loss, rrt); 
    printf("rtt min/avg/max= /%.3f/%.3f/%.3f ms\n", _g.min, _g.total /  _g.pkg_received,  _g.max);
    exit(EXIT_SUCCESS);
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
    gettimeofday(&_g._tv_start, NULL);
    while (1)
    {
        _g.msg.msg_namelen = _g.ssendlen;
        _g.msg.msg_controllen = sizeof(ctrlbuff);
        _tmp = recvmsg(_g.sockfd, &(_g.msg), 0);
        if (_tmp < 0)
            FETAL("Recvmsg Error.");
        else
            _g.ft_recv(_tmp, recvbuff);
    }
}
