/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpeng <qpeng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 14:51:58 by qpeng             #+#    #+#             */
/*   Updated: 2019/04/19 11:00:57 by qpeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ping.h"


void     sig_int(int signo)
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

struct addrinfo *   host_to_addrinfo(const char *host, const char *serv, int family, int socktype)
{
	struct addrinfo	hints;
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

void   ping_init(char *host)
{
    struct addrinfo     *ret;
    void                *ptr;
    static char         addrstr[100];

    ret = host_to_addrinfo(host, NULL, AF_INET, SOCK_STREAM);
    if (!ret)
    {
        fprintf(stderr, "ping: cannot resolve %s: Unknown host", host);
        exit(EXIT_FAILURE);
    }
    _g.pid = getpid() & 0xffff;
    setuid(getuid());
    _g.ssend = ret->ai_addr;
    _g.ssendlen = ret->ai_addrlen;
    if (ret->ai_family == AF_INET)
        ptr = &((struct sockaddr_in *) ret->ai_addr)->sin_addr;
    else
        ptr = &((struct sockaddr_in6 *) ret->ai_addr)->sin6_addr;
    _g.ft_send = (ret->ai_family == AF_INET) ? send_v4 : send_v6;
    _g.ft_recv = (ret->ai_family == AF_INET) ? readmsg_v4 : readmsg_v6;
    inet_ntop(ret->ai_family, ptr, _g.ip, 100);
    _g.r_host = _g.r_ns_lookup ? reverse_dns_lookup(_g.ip) : host;
    printf("PING %s (%s): %d data (%d) bytes of data\n", host, _g.ip, DATALEN, PCKSIZE(DATALEN));
    readloop();
}

int     main(int ac, char **av)
{
    _g.msg_cnt = 0;
    _g.host = av[1];
    _g.min = 0.0000;
    _g.max = 0.0000;
    _g.times = INT32_MAX;
    _g.duration = 1;
    _g.r_ns_lookup = true;
    readopt(ac, av);
    signal(SIGALRM, sig_alrm);
    signal(SIGINT, sig_int);
    ping_init(av[1]);
    return (0);
}