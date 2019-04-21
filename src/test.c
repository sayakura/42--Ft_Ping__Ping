#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct addrinfo		*host_to_addrinfo(const char *host,\
						const char *serv, int family, int socktype)
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
	return (res);
}

int main()
{
    struct addrinfo		*ret;

	ret = host_to_addrinfo("216.58.195.238", NULL, AF_INET, SOCK_STREAM);
    return 0;
}
