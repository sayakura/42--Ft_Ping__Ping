/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kura <kura@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 20:51:51 by qpeng             #+#    #+#             */
/*   Updated: 2019/04/09 08:34:24 by kura             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ping.h"


// char    *dns_lookup(char *addr_host, struct sockaddr_in *addr_con) 
// { 
//     struct hostent *host_entity; 

//     char *ip = (char*)malloc(NI_MAXHOST*sizeof(char)); 
//     int i; 

//     printf("\nResolving DNS..\n"); 
//     if ((host_entity = gethostbyname(addr_host)) == NULL) 
//         return NULL; 
//     strcpy(ip, inet_ntoa(*(struct in_addr *) 
//                         host_entity->h_addr)); 
//     (*addr_con).sin_family = host_entity->h_addrtype; 
//     (*addr_con).sin_port = htons(PORT_NO); 
//     (*addr_con).sin_addr.s_addr  = *(long*)host_entity->h_addr; 
//     return ip; 
// } 

#include <netinet/in.h>

void intHandler(int __) 
{ 
    // sth
} 

void    send_v4(void)
{
    struct icmp     *icmp;
    int             len;

    icmp = (struct icmp *)_g.sendbuf;
    icmp->icmp_type = ICMP_ECHO;
    icmp->icmp_code = 0;
    icmp->icmp_seq = _g.seq++;
    // gettimeofday((struct timeval *)_g.icmp->icmp_data, NULL);
    memset(icmp->icmp_data, 0xff, 56);
    len = 8 + 56;
    icmp->icmp_cksum = 0;
    icmp->icmp_cksum = get_checksum((uint16_t *)(&icmp), len);
    if (sendto(_g.sockfd, &icmp, len, 0, _g.ssend, sizeof(struct sockaddr)) < 0)
    {
        perror("sendto");
        exit(EXIT_FAILURE);
    }
    printf("sent: %d\n", len);
}

// void    pkg_builder(t_ping_pkt *pckt)
// {
//     u_int8_t    rdx;
//     u_int8_t    siz;

//     XOR(rdx);
//     siz = sizeof(pckt->msg) - 1;
//     bzero(pckt, sizeof(t_ping_pkt));
//     (pckt->hdr).type = ICMP_ECHO; 
//     (pckt->hdr).un.echo.id = getpid(); 
//     while (rdx < siz)
//         pckt->msg[rdx++] = (char)rdx + 1;
//     XOR(pckt->msg[rdx]);
//     (pckt->hdr).un.echo.sequence = _g.msg_cnt++;
//     (pckt->hdr).checksum = get_checksum(&pckt, sizeof(pckt));
// }

void    ping(void)
{
    t_ping_pkt          pckt;
    struct timeval      start;
    struct timeval      end;
    int                 addrlen;
   
    char                control_buf[100];
    char                buffer[80];
    struct msghdr       msg;
    struct iovec        iov;

    memset(&msg, 0, sizeof(msg));
    memset(&msg, 0, sizeof(iov));
    iov.iov_base = buffer;
    iov.iov_len = sizeof(buffer);
    msg.msg_name = &_g.ssend;
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = control_buf;
    msg.msg_controllen = sizeof(control_buf);

    //pkg_builder(&pckt);
    gettimeofday(&start, NULL);
    printf("send to %d\n", _g.sockfd);
    if (sendto(_g.sockfd, &pckt, sizeof(t_ping_pkt), 0,\
        (struct sockaddr *)(&_g.ssend), sizeof(_g.ssendlen)) <= 0)
    {
        perror("sendto");
        exit(EXIT_FAILURE);
    }
    printf("sent\n");
    if (recvmsg(_g.sockfd, &msg,  0) < 0)
    {
        perror("recvmsg");
        exit(EXIT_FAILURE);
    };
    printf("received\n");
    gettimeofday(&end, NULL);
    printf("%d bytes from %s (%s) icmp_seq=%d ttl=%d time = %.3f ms.\n",\
        _g.pkg_siz, _g.r_host, _g.ip, _g.msg_cnt, _g.ttl_val, caltime(end, start));
}

int     sock_init(void)
{
    int                  on;

    on = 1;
    // _g.ser_addr.sin_family = AF_INET;
    // _g.ser_addr.sin_port = 0;
    // _g.ser_addr.sin_addr.s_addr = inet_addr(_g.ip);
    if ((setsockopt(_g.sockfd, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0) || 
        (setsockopt(_g.sockfd, SOL_IP, IP_TTL, &_g.ttl_val, sizeof(_g.ttl_val)) < 0))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    // setsockopt(sock_id, SOL_SOCKET, SO_RCVTIMEO, 
    //                (const char*)&tv_out, sizeof tv_out); 
}

int    ft_recvmsg(void)
{
    int             ret;
    // struct sockaddr sin;

    // memset(&_g.msg, 0, sizeof(_g.msg));
    // memset(&_g.iov, 0, sizeof(_g.iov));
    // _g.iov.iov_base = _g.recvbuf;
    // _g.iov.iov_len = sizeof(_g.recvbuf);
    // _g.msg.msg_name = &sin;
    // _g.msg.msg_namelen = sizeof(sin);
    // _g.msg.msg_iov = &(_g.iov);
    // _g.msg.msg_iovlen = 1;
    // _g.msg.msg_control = _g.ctrl_buf;
    // _g.msg.msg_controllen = sizeof(_g.ctrl_buf);
    //printf("trying to recv some shits\n");
    struct msghdr   mhdr;
    struct iovec    iov[1];
    struct cmsghdr  *cmhdr;
    struct sockaddr_in  sin;
    char            databuf[1500];
    char            control[1000];
    unsigned char   tos;
    
    mhdr.msg_name = _g.ssend;
    mhdr.msg_namelen = sizeof(struct sockaddr);
    mhdr.msg_iov = iov;
    mhdr.msg_iovlen = 1;
    mhdr.msg_control = &control;
    mhdr.msg_controllen = sizeof(control);
    iov[0].iov_base = databuf;
    iov[0].iov_len = sizeof(databuf);
    memset(databuf, 0, sizeof(databuf));
    printf("fd: %d\n", _g.sockfd);
    ret = recvmsg(_g.sockfd, &mhdr, 0);
    if (ret < 0)
        perror("recvmsg");
    printf("received: %d\n", ret);
    return ret;
}

void    sig_alarm(int signal)
{
    send_v4();
    alarm(1);
    return ;
}

void    readloop(void)
{
    int             size;
    size_t          n;
    socklen_t       len;
    struct timeval  tval;

    _g.sockfd = socket(_g.ssend->sa_family, SOCK_RAW, IPPROTO_ICMP);
    size = 60 * 1024;		/* OK if setsockopt fails */
	setsockopt(_g.sockfd , SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));

    _g.pid = getuid();

    sig_alarm(SIGALRM);
    // set socket options
    while (1)
    {
        n = ft_recvmsg();
        // gettimeofday(&tval, NULL);
        printf("received: %ld\n", n);
    }
}

void   ping_init(char *host)
{
    // char                        *ip;
    // char                        *r_host

    // XOR(_g.seq);
    // // _g.pkg_siz = PING_PKG_SIZ;
    // _g.time_out = PING_TIMEOUT;
    // _g.ttl_val = PING_TTL;
    // _g.ip = lookup_host(host);
    // if (_g.ip == NULL)
    //    printf("ping: %s: Name or service not known\n", host);
    // r_host = reverse_dns_lookup(_g.ip);
    // if ((_g.sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
    // {
    //     perror("socket");
    //     exit(EXIT_FAILURE);
    // }
    // sock_init();
    // printf("PING %s (%s) %d bytes of data.\n", r_host, _g.ip, _g.pkg_siz);
    // // signal(SIGINT, intHandler);
    // signal(SIGALRM, sig_alarm);
    // sig_alarm(SIGALRM);


    struct addrinfo     *ret;
    void                *ptr;
    char                addrstr[100];

    ret = host_to_addrinfo(host, NULL, AF_INET, SOCK_STREAM);
    _g.ssend = malloc(sizeof(struct sockaddr));
    memcpy(_g.ssend, ret->ai_addr, ret->ai_addrlen);
    //_g.ssend = ret->ai_addr;
    _g.ssendlen = ret->ai_addrlen;

     if (ret->ai_family == AF_INET)
            ptr = &((struct sockaddr_in *) ret->ai_addr)->sin_addr;
    else if (ret->ai_family == AF_INET6)
        ptr = &((struct sockaddr_in6 *) ret->ai_addr)->sin6_addr;
    inet_ntop (ret->ai_family, ptr, addrstr, 100);
    printf("ip found: %s\n", addrstr);
    readloop();
}

int     main(int ac, char **av)
{
    if (ac != 2)
    {
        printf("usage: ping [-v] host [-h]\n");
        exit(EXIT_SUCCESS);
    }
    _g.datalen = 56;
    _g.icmp.icmp_seq = 0;
    signal(SIGALRM, sig_alarm);
    ping_init(av[1]);
    return (0);
}