/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kura <kura@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 00:31:42 by kura              #+#    #+#             */
/*   Updated: 2019/04/08 06:54:45 by kura             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ping.h"

u_int8_t    get_checksum(uint16_t *addr, int len)
{

    int         nleft;
    uint16_t    *w;
    uint16_t    answer;
    uint32_t    sum;

    nleft = len;
    sum = 0;
    w = addr;
    answer = 0;

    /*
    **  Our algorithm is simple, using a 32 bit accumulator (sum), we add
    **  sequential 16 bit words to it, and at the end, fold back all the
    **  carry bits from the top 16 bits into the lower 16 bits.
    */

    while (nleft > 1)
    {

        /*
        **
        */

        sum += *w++;

        /*
        **
        */

        nleft -= 2;

    }

    /*
    ** mop up an odd byte, if necessary
    */

    if (nleft == 1)
    {

        /*
        **
        */

        * (unsigned char *) (&answer) = * (unsigned char *) w;

        /*
        **
        */

        sum += answer;

    }

    /*
    ** Add back carry outs from top 16 bits to low 16 bits
    ** add hi 16 to low 16
    */

    sum = (sum >> 16) + (sum & 0xffff);

    /*
    ** Add carry
    */

    sum += (sum >> 16);

    /*
    ** truncate to 16 bits
    */

    answer = ~sum;

    /*
    **
    */

    return (answer);

}


char        *reverse_dns_lookup(char *ip_addr) 
{ 
    struct sockaddr_in  _temp;     
    socklen_t           len; 
    char                buf[NI_MAXHOST];
    char                *ret_buf; 
  
    _temp.sin_family = AF_INET; 
    _temp.sin_addr.s_addr = inet_addr(ip_addr); 
    len = sizeof(struct sockaddr_in); 
    if (getnameinfo((struct sockaddr *) &_temp, len, buf,  
                    sizeof(buf), NULL, 0, NI_NAMEREQD))  
    { 
        perror("getnameinfo");
        return NULL; 
    }
    ret_buf = (char*)malloc((strlen(buf) +1)*sizeof(char) ); 
    strcpy(ret_buf, buf); 
    return ret_buf; 
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

char    *lookup_host (const char *host)
{
    struct addrinfo   hints;
    struct addrinfo   *res;
    static char       addrstr[100];
    void              *ptr;

    memset (&hints, 0, sizeof (hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags |= AI_CANONNAME;
    if (getaddrinfo(host, NULL, &hints, &res) < 0)
    {
        perror ("getaddrinfo");
        return (NULL);
    }
    if (res)
    {
        inet_ntop (res->ai_family, res->ai_addr->sa_data, addrstr, 100);
        if (res->ai_family == AF_INET)
            ptr = &((struct sockaddr_in *) res->ai_addr)->sin_addr;
        else if (res->ai_family == AF_INET6)
            ptr = &((struct sockaddr_in6 *) res->ai_addr)->sin6_addr;
        inet_ntop (res->ai_family, ptr, addrstr, 100);
        return (addrstr);
    }
    else
        return (NULL);
}

double    caltime(struct timeval end, struct timeval start)
{
    if ((end.tv_usec -= start.tv_usec) < 0)
    {
        --end.tv_sec;
        end.tv_usec += 1000000;
    }
    end.tv_sec -= start.tv_sec;
    return -(double)(end.tv_sec * 1000.0 + end.tv_usec / 1000.0);
}