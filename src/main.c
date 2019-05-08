/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpeng <qpeng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 14:51:58 by qpeng             #+#    #+#             */
/*   Updated: 2019/05/08 01:17:29 by qpeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ping.h"

char				*reverse_dns_lookup(char *ip_addr)
{
	struct sockaddr_in	temp_addr;
	socklen_t			len;
	char				buf[100];
	static char			ret_buf[200];

	temp_addr.sin_family = AF_INET;
	temp_addr.sin_addr.s_addr = inet_addr(ip_addr);
	len = sizeof(struct sockaddr_in);
	if (getnameinfo((struct sockaddr *)&temp_addr, len, buf,
					sizeof(buf), NULL, 0, NI_NAMEREQD))
		return (ip_addr);
	strcpy(ret_buf, buf);
	return (ret_buf);
}

struct addrinfo		*host_to_addrinfo(char *host,\
						const char *serv, int family, int socktype)
{
	struct addrinfo hints;
	struct addrinfo *res;

	bzero_(&hints, sizeof(struct addrinfo));
	hints.ai_flags = AI_CANONNAME;
	hints.ai_family = family;
	hints.ai_socktype = socktype;
	if (getaddrinfo(host, serv, &hints, &res))
	{
		fprintf(stderr, "ping: %s: Name or service not known\n", host);
		exit(EXIT_FAILURE);
	}
	return (res);
}

void				ping_init(void)
{
	struct addrinfo		*ret;
	void				*ptr;
	int					version;

	if (!gl.host)
		PRINT_USAGE;
	version = get_ip_v(gl.host);
	ret = host_to_addrinfo(gl.host, NULL, (version == 4 || version == 0) ?
		AF_INET : AF_INET6, SOCK_STREAM);
	gl.ssend = ret->ai_addr;
	gl.ssendlen = ret->ai_addrlen;
	if (ret->ai_family == AF_INET)
		ptr = &((struct sockaddr_in *)ret->ai_addr)->sin_addr;
	else
		ptr = &((struct sockaddr_in6 *)ret->ai_addr)->sin6_addr;
	gl.ft_send = (ret->ai_family == AF_INET) ? send_v4 : send_v6;
	gl.ft_recv = (ret->ai_family == AF_INET) ? readmsg_v4 : readmsg_v6;
	gl.protocol = (ret->ai_family == AF_INET) ? IPPROTO_ICMP : IPPROTO_ICMPV6;
	ERR_CHECK(!inet_ntop(ret->ai_family, ptr, gl.ip, 100), "inet_ntop");
	gl.r_host = gl.r_ns_lookup && ret->ai_family ==\
								AF_INET ? reverse_dns_lookup(gl.ip) : gl.host;
}

void				env_init(void)
{
	gl.msg_cnt = 0;
	gl.min = 0.0000;
	gl.max = 0.0000;
	gl.total = 0.0000;
	gl.times = INT32_MAX;
	gl.duration = 1;
	gl.r_ns_lookup = true;
	gl.pid = getpid() & 0xffff;
	gl.ttl = 64;
	gl.host = NULL;
	ERR_CHECK(signal(SIGALRM, sig_alrm) == SIG_ERR, "signal");
	ERR_CHECK(signal(SIGINT, sig_int) == SIG_ERR, "signal");
}

int					main(int ac, char **av)
{
	if (ac < 2)
	{
		PRINT_USAGE;
		exit(EXIT_FAILURE);
	}
	env_init();
	readopt(ac, av);
	ping_init();
	readloop();
	return (0);
}
