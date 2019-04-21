/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpeng <qpeng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 14:51:58 by qpeng             #+#    #+#             */
/*   Updated: 2019/04/21 02:20:04 by qpeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ping.h"



struct addrinfo     *host_to_addrinfo(const char *host, const char *serv, int family, int socktype)
{
	struct addrinfo hints;
    struct addrinfo *res;

	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_flags = AI_CANONNAME;
	hints.ai_family = family;
	hints.ai_socktype = socktype;
	if (getaddrinfo(host, serv, &hints, &res) != 0)
    {
        fprintf(stderr, "ping: %s: Name or service not known\n", host);
        exit(EXIT_FAILURE);
    }
	return(res);
}

void                ping_init(void)
{
    struct addrinfo     *ret;
    void                *ptr;
    static char         addrstr[100];

    ret = host_to_addrinfo(_g.host, NULL, AF_INET6, SOCK_STREAM);
    if (!ret)
    {
        fprintf(stderr, "ping: cannot resolve %s: Unknown host", _g.host);
        exit(EXIT_FAILURE);
    }
    setuid(getuid());
    _g.ssend = ret->ai_addr;
    _g.ssendlen = ret->ai_addrlen;
    if (ret->ai_family == AF_INET)
        ptr = &((struct sockaddr_in *) ret->ai_addr)->sin_addr;
    else
        ptr = &((struct sockaddr_in6 *) ret->ai_addr)->sin6_addr;
    _g.ft_send = (ret->ai_family == AF_INET) ? send_v4 : send_v6;
    _g.ft_recv = (ret->ai_family == AF_INET) ? readmsg_v4 : readmsg_v6;
    _g.protocol = (ret->ai_family == AF_INET) ? IPPROTO_ICMP : IPPROTO_ICMPV6;
    inet_ntop(ret->ai_family, ptr, _g.ip, 100);
    _g.r_host = _g.r_ns_lookup && ret->ai_family == AF_INET ? reverse_dns_lookup(_g.ip) : _g.host;
    printf("PING %s (%s): %d data (%d) bytes of data\n", _g.host, _g.ip, DATALEN, PCKSIZE(DATALEN));
}

void                env_init(void)
{
    _g.msg_cnt = 0;
    _g.min = 0.0000;
    _g.max = 0.0000;
    _g.times = INT32_MAX;
    _g.duration = 1;
    _g.r_ns_lookup = true;
    _g.pid = getpid() & 0xffff;
    signal(SIGALRM, sig_alrm);
    signal(SIGINT, sig_int);
}

int                 main(int ac, char **av)
{
    env_init();
    readopt(ac, av);
    ping_init();
    readloop();
    return (0);
}