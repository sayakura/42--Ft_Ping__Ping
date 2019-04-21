/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpeng <qpeng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 14:52:09 by qpeng             #+#    #+#             */
/*   Updated: 2019/04/21 06:59:08 by qpeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ping.h"

const char		*g_err_msg_fmt[] = {
	"ping: invalid count of packets to transmit:",
	"ping: invalid timing interval:",
	"ping: invalid TTL:"
};

const char		g_flag_lookup[] = {
	'c',
	'i',
	't'
};

uint16_t		in_cksum(uint16_t *addr, int len)
{
	uint32_t	sum;
	uint16_t	answer;

	answer = 0;
	sum = 0;
	while (len > 1)
	{
		sum += *addr++;
		len -= 2;
	}
	if (len == 1)
	{
		*(unsigned char *)(&answer) = *(unsigned char *)addr;
		sum += answer;
	}
	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	answer = ~sum;
	return (answer);
}

void			stat_cnt(double rrt)
{
	gl.pkg_received++;
	if (rrt != NOT_ECHO)
	{
		gl.total += rrt;
		if (rrt < gl.min || gl.min == 0.0000)
			gl.min = rrt;
		if (rrt > gl.max)
			gl.max = rrt;
	}
}

void			get_hlim_val(int *hlim)
{
	struct cmsghdr	*cmsg;

	cmsg = CMSG_FIRSTHDR(&gl.msg);
	while (cmsg != NULL)
	{
		if (cmsg->cmsg_level == IPPROTO_IPV6
			&& cmsg->cmsg_type == IPV6_HOPLIMIT)
		{
			*hlim = *(u_int32_t *)CMSG_DATA(cmsg);
			break ;
		}
		cmsg = CMSG_NXTHDR(&gl.msg, cmsg);
	}
}

void			arg_check(int rcx, int ac, char **av, int flag)
{
	if (!LAST_ARG(rcx, ac))
	{
		if (atoi(av[rcx + 1]) > 0)
		{
			if (flag == PING_FLAG_C)
				gl.times = atoi(av[rcx + 1]);
			else if (flag == PING_FLAG_I)
				gl.duration = atoi(av[rcx + 1]);
			else if (flag == PING_FLAG_T)
				gl.ttl = atoi(av[rcx + 1]);
		}
		else
		{
			fprintf(stderr, "%s `%s`\n", g_err_msg_fmt[flag], av[rcx + 1]);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		fprintf(stderr, "ping: option requires an argument -- %c\n"
		, g_flag_lookup[flag]);
		PRINT_USAGE;
	}
}

void			readopt(int ac, char **av)
{
	int		rcx;

	rcx = 0;
	while (++rcx < ac)
	{
		if (!strcmp(av[rcx], "-v"))
			gl.verbose = true;
		else if (!strcmp(av[rcx], "-h"))
			PRINT_USAGE;
		else if (!strcmp(av[rcx], "-c"))
			arg_check(rcx, ac, av, PING_FLAG_C);
		else if (!strcmp(av[rcx], "-i"))
			arg_check(rcx, ac, av, PING_FLAG_I);
		else if (!strcmp(av[rcx], "-m"))
			arg_check(rcx, ac, av, PING_FLAG_T);
		else if (!strcmp(av[rcx], "-n"))
			gl.r_ns_lookup = false;
		else if (!strcmp(av[rcx], "-a"))
			gl.bell = true;
		else if (!strcmp(av[rcx], "-q"))
			gl.quiet = true;
		else if (strcmp(av[rcx - 1], "-c") && strcmp(av[rcx - 1], "-i")
			&& strcmp(av[rcx - 1], "-m"))
			gl.host = strcmp(av[rcx], "0") == 0 ? "localhost" : av[rcx];
	}
}
