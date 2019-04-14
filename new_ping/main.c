#include "ping.h"


void     sig_int(int signo)
{
    int         diff;
    double      loss;

    diff = _g.msg_cnt - _g.pkg_received;
    loss = (double)diff / _g.msg_cnt * 100;
    printf("\n--- %s ping statistics ---\n", _g.host);
    printf("%d packets transmitted, %d packets received, %.0f%% packet loss\n",_g.msg_cnt, _g.pkg_received, loss); 
    printf("rtt min/avg/max= /%.3fms/%.3fms/%.3f ms\n", _g.min, _g.total /  _g.pkg_received,  _g.max);
    exit(EXIT_SUCCESS);
}

struct addrinfo *   host_to_addrinfo(const char *host, const char *serv, int family, int socktype)
{
	struct addrinfo	hints;
    struct addrinfo *res;

	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_flags = AI_CANONNAME;	/* always return canonical name */
	hints.ai_family = family;		/* 0, AF_INET, AF_INET6, etc. */
	hints.ai_socktype = socktype;	/* 0, SOCK_STREAM, SOCK_DGRAM, etc. */

	if (getaddrinfo(host, serv, &hints, &res) != 0)
    {
        fprintf(stderr, "Can't find address info\n");
        exit(EXIT_FAILURE);
    }
	return(res);	                /* return pointer to first on linked list */
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
    else if (ret->ai_family == AF_INET6)
        ptr = &((struct sockaddr_in6 *) ret->ai_addr)->sin6_addr;
    inet_ntop(ret->ai_family, ptr, _g.ip, 100);
    printf("PING %s (%s): %d data bytes of data\n", host, _g.ip, DATALEN);
    readloop();
}

int     main(int ac, char **av)
{
    if (ac != 2)
    {
        printf("usage: ping [-v] host [-h]\n");
        exit(EXIT_SUCCESS);
    }
    _g.msg_cnt = 0;
    _g.host = av[1];
    signal(SIGALRM, sig_alrm);
    signal(SIGINT, sig_int);
    //signal(SIGALRM, sig_alarm);
    ping_init(av[1]);
    return (0);
}