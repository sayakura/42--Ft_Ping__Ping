#include "ping.h"

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
    _g.ssend = ret->ai_addr;
    _g.ssendlen = ret->ai_addrlen;
    if (ret->ai_family == AF_INET)
        ptr = &((struct sockaddr_in *) ret->ai_addr)->sin_addr;
    else if (ret->ai_family == AF_INET6)
        ptr = &((struct sockaddr_in6 *) ret->ai_addr)->sin6_addr;
    inet_ntop(ret->ai_family, ptr, _g.ip, 100);
    printf("ip found: %s\n", _g.ip);
    //readloop();
}

int     main(int ac, char **av)
{
    if (ac != 2)
    {
        printf("usage: ping [-v] host [-h]\n");
        exit(EXIT_SUCCESS);
    }
    _g.msg_cnt = 0;
    signal(SIGALRM, sig_alrm);
    //signal(SIGALRM, sig_alarm);
    ping_init(av[1]);
    return (0);
}