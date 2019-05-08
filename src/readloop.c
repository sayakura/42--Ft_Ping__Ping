/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readloop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpeng <qpeng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 14:52:01 by qpeng             #+#    #+#             */
/*   Updated: 2019/05/08 05:48:32 by qpeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ping.h"

void	sig_alrm(int signo)
{
	(void)(signo);
	if (gl.times--)
		alarm(gl.duration);
	else
		sig_int(SIGINT);
	gl.ft_send();
	return ;
}

void	sig_int(int signo)
{
	int			diff;
	double		loss;
	double		rrt;

	(void)(signo);
	ERR_CHECK(gettimeofday(&gl.tv_end, NULL), "gettimeofday");
	tv_sub(&gl.tv_end, &gl.tv_start);
	rrt = gl.tv_end.tv_sec * 1000.0 + gl.tv_end.tv_usec / 1000.0;
	diff = gl.msg_cnt - gl.pkg_received;
	loss = (double)diff / gl.msg_cnt * 100;
	printf("\n--- %s ping statistics ---\n", gl.host);
	printf("%d packets transmitted, %d received, %.0f%% packet loss"
												" time %.0f ms\n",
	gl.msg_cnt, gl.pkg_received, loss, rrt);
	printf("rtt min/avg/max= /%.3f/%.3f/%.3f ms\n",\
						gl.min, gl.total / gl.pkg_received, gl.max);
	exit(EXIT_SUCCESS);
}

void	tv_sub(struct timeval *out, struct timeval *in)
{
	if ((out->tv_usec -= in->tv_usec) < 0)
	{
		--out->tv_sec;
		out->tv_usec += 1000000;
	}
	out->tv_sec -= in->tv_sec;
}

void	creat_sock(void)
{
	int		on;
	int		size;

	on = 1;
	size = 60 * 1024;
	gl.sockfd = socket(gl.ssend->sa_family, SOCK_RAW, gl.protocol);
	ERR_CHECK(gl.sockfd == -1, "socket");
	if (gl.protocol == IPPROTO_ICMPV6)
	{
		if (setsockopt(gl.sockfd, IPPROTO_IPV6,\
							IPV6_RECVHOPLIMIT, &on, sizeof(on)))
			FETAL("Can't set hop limit for the socket.");
	}
	if (setsockopt(gl.sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size)))
		fprintf(stderr, "Can't set receive buffer size for socket.");
	if (setsockopt(gl.sockfd, gl.protocol == IPPROTO_ICMPV6 ?
		IPPROTO_IPV6 : IPPROTO_IP, IP_TTL, &gl.ttl, sizeof(gl.ttl)))
		FETAL("Can't set ttl value for the socket.");
}

void	readloop(void)
{
	int			b_recv;
	char		recvbuff[BUFF_SIZE];
	char		ctrlbuff[BUFF_SIZE];

	creat_sock();
	printf("PING %s (%s): %d data (%d) bytes of data\n",\
									gl.host, gl.ip, DATALEN, PCKSIZE(DATALEN));
	gl.iov.iov_base = recvbuff;
	gl.iov.iov_len = sizeof(recvbuff);
	gl.msg.msg_name = gl.srecv;
	gl.msg.msg_iov = &(gl.iov);
	gl.msg.msg_iovlen = 1;
	gl.msg.msg_control = ctrlbuff;
	sig_alrm(SIGALRM);
	ERR_CHECK(gettimeofday(&gl.tv_start, NULL), "gettimeofday");
	while (1)
	{
		gl.msg.msg_namelen = gl.ssendlen;
		gl.msg.msg_controllen = sizeof(ctrlbuff);
		b_recv = recvmsg(gl.sockfd, &(gl.msg), 0);
		if (b_recv < 0)
			FETAL("Recvmsg Error.");
		else
			gl.ft_recv(b_recv, recvbuff);
	}
}
