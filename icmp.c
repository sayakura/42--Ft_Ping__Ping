/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpeng <qpeng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 14:51:54 by qpeng             #+#    #+#             */
/*   Updated: 2019/04/21 04:43:33 by qpeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ping.h"

void		send_v4(void)
{
	int			len;
	struct icmp	*icmp;

	icmp = (struct icmp *)_g.sendbuf;
	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_id = _g.pid;
	icmp->icmp_seq = _g.msg_cnt++;
	memset(icmp->icmp_data, 0xa5, DATALEN);
	gettimeofday((struct timeval *)icmp->icmp_data, NULL);
	len = 8 + DATALEN;
	icmp->icmp_cksum = 0;
	icmp->icmp_cksum = in_cksum((u_short *)icmp, len);
	sendto(_g.sockfd, _g.sendbuf, len, 0, _g.ssend, _g.ssendlen);
}

void		send_v6(void)
{
	int					len;
	struct icmp6_hdr	*icmp;

	icmp = (struct icmp6_hdr *)_g.sendbuf;
	icmp->icmp6_type = ICMP6_ECHO_REQUEST;
	icmp->icmp6_code = 0;
	icmp->icmp6_id = _g.pid;
	icmp->icmp6_seq = _g.msg_cnt++;
	memset((icmp + 1), 0x77, DATALEN);
	gettimeofday((struct timeval *)(icmp + 1), NULL);
	len = 8 + DATALEN;
	sendto(_g.sockfd, _g.sendbuf, len, 0, _g.ssend, _g.ssendlen);
}

static void	print_msg(t_info info, bool is_echo)
{
	char	bell_char;

	bell_char = 0x07;
	stat_cnt(is_echo ? info.rrt : NOT_ECHO);
	if (!_g.quiet)
	{
		if (is_echo)
			if (_g.r_ns_lookup)
				printf("%d bytes from %s (%s): icmp_seq=%u, %s=%d, time=%.3f\
					ms\n", info.b_recv, _g.r_host ? _g.r_host : _g.host,\
					_g.ip, info.seq, info.ver == AF_INET ? "ttl" : "hlim",\
					info.cnt, info.rrt);
			else
				printf("%d bytes from %s: icmp_seq=%u, %s=%d, time=%.3f ms\n",
					info.b_recv, _g.ip, info.seq, info.ver == AF_INET ? "ttl"\
					: "hlim", info.cnt, info.rrt);
		else
			printf(" %d bytes from %s: type = %d, code = %d\n",
				info.b_recv, _g.ip, info.type, info.code);
		if (_g.bell)
			write(1, &bell_char, 1);
	}
	if (!_g.times)
		sig_int(SIGINT);
}

void		readmsg_v6(int b_read, char *recvbuff)
{
	struct icmp6_hdr	*icmp6;
	struct timeval		tvrecv;
	struct cmsghdr		*cmsg;
	int					hlim;
	double				rrt;

	gettimeofday(&tvrecv, NULL);
	icmp6 = (struct icmp6_hdr *)recvbuff;
	if (b_read < 8)
		return ;
	if (icmp6->icmp6_type == ICMP6_ECHO_REPLY)
	{
		if (icmp6->icmp6_id != _g.pid || b_read < 16)
			return ;
		tv_sub(&tvrecv, (struct timeval *)(icmp6 + 1));
		rrt = tvrecv.tv_sec * 1000.0 + tvrecv.tv_usec / 1000.0;
		hlim = -1;
		get_hlim_val(&hlim);
		print_msg((t_info){.ver = AF_INET6, .seq = icmp6->icmp6_seq,\
			.b_recv = b_read, .cnt = hlim, .rrt = rrt}, true);
	}
	else if (_g.verbose)
		print_msg((t_info){.ver = AF_INET, .b_recv = b_read,\
			.type = icmp6->icmp6_type, .code = icmp6->icmp6_code}, false);
}

void		readmsg_v4(int b_read, char *recvbuff)
{
	struct ip		*iphdr;
	struct icmp		*icmp;
	struct timeval	tvrecv;
	int				hdrlen;
	double			rrt;

	gettimeofday(&tvrecv, NULL);
	iphdr = (struct ip *)recvbuff;
	hdrlen = iphdr->ip_hl << 2;
	if (iphdr->ip_p != IPPROTO_ICMP || (b_read - hdrlen) < 8)
		return ;
	icmp = (struct icmp *)(recvbuff + hdrlen);
	if (icmp->icmp_type == ICMP_ECHOREPLY)
	{
		if (icmp->icmp_id != _g.pid || (b_read - hdrlen) < 16)
			return ;
		tv_sub(&tvrecv, (struct timeval *)icmp->icmp_data);
		rrt = tvrecv.tv_sec * 1000.0 + tvrecv.tv_usec / 1000.0;
		print_msg((t_info){.ver = AF_INET, .seq = icmp->icmp_seq,\
			.b_recv = b_read - hdrlen, .cnt = iphdr->ip_ttl, .rrt = rrt}, true);
	}
	else if (_g.verbose)
		print_msg((t_info){.ver = AF_INET, .b_recv = b_read - hdrlen,\
		.type = icmp->icmp_type, .code = icmp->icmp_type}, false);
}
