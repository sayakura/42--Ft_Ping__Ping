#include "ping.h"

uint16_t    in_cksum (uint16_t * addr, int len)
 {
    uint32_t    sum;
    uint16_t    answer;
    
    answer = 0;
    sum = 0;
    while (len > 1)
    {
        sum += *addr++;
        len -= 2;
     }
    if (len == 1)
    {
        * (unsigned char *) (&answer) = * (unsigned char *) addr;
        sum += answer;
     }
    sum = (sum >> 16) + (sum & 0xffff); 
    sum += (sum >> 16); 
    answer = ~sum; 
    return (answer);
}


void    send_v4(void)
{
    int     len;
    struct  icmp *icmp;

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

void    readmsg_v4(int b_read, char *recvbuff)
{
    struct ip       *iphdr;
    struct icmp     *icmp;
    struct timeval  tvrecv;
    int             hdrlen;
    double          rrt;

    gettimeofday(&tvrecv, NULL);
    iphdr = (struct ip *)recvbuff;
    hdrlen = iphdr->ip_hl << 2;
    if (iphdr->ip_p != IPPROTO_ICMP)
        return ;
    icmp = (struct icmp *)(recvbuff + hdrlen);
    if ((b_read - hdrlen) < 8)
        return ;
    if (icmp->icmp_type == ICMP_ECHOREPLY)
    {
        if (icmp->icmp_id != _g.pid || (b_read - hdrlen) < 16)
            return ;
        tv_sub(&tvrecv, (struct timeval *)icmp->icmp_data);
        rrt = tvrecv.tv_sec * 1000.0 + tvrecv.tv_usec / 1000.0;
        stat_cnt(rrt);
        printf("%d bytes from %s (%s): icmp_seq=%u, ttl=%d, time=%.3f ms\n",
            (b_read - hdrlen), _g.r_host ? _g.r_host : _g.host, _g.ip,  icmp->icmp_seq, iphdr->ip_ttl, rrt);
    }
    else if (_g.verbose)
    {
        stat_cnt(0);
        printf(" %d bytes from %s (%s): type = %d, code = %d\n",
            (b_read - hdrlen), _g.host, _g.ip, icmp->icmp_type, icmp->icmp_code);
    }
}


void    send_v6(void)
{
    int     len;
    struct  icmp *icmp;

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

void    readmsg_v6(int b_read, char *recvbuff)
{
    struct ip       *iphdr;
    struct icmp     *icmp;
    struct timeval  tvrecv;
    int             hdrlen;
    double          rrt;

    gettimeofday(&tvrecv, NULL);
    iphdr = (struct ip *)recvbuff;
    hdrlen = iphdr->ip_hl << 2;
    if (iphdr->ip_p != IPPROTO_ICMP)
        return ;
    icmp = (struct icmp *)(recvbuff + hdrlen);
    if ((b_read - hdrlen) < 8)
        return ;
    if (icmp->icmp_type == ICMP_ECHOREPLY)
    {
        if (icmp->icmp_id != _g.pid || (b_read - hdrlen) < 16)
            return ;
        tv_sub(&tvrecv, (struct timeval *)icmp->icmp_data);
        rrt = tvrecv.tv_sec * 1000.0 + tvrecv.tv_usec / 1000.0;
        stat_cnt(rrt);
        printf("%d bytes from %s (%s): icmp_seq=%u, ttl=%d, time=%.3f ms\n",
            (b_read - hdrlen), _g.r_host ? _g.r_host : _g.host, _g.ip,  icmp->icmp_seq, iphdr->ip_ttl, rrt);
    }
    else if (_g.verbose)
    {
        stat_cnt(0);
        printf(" %d bytes from %s (%s): type = %d, code = %d\n",
            (b_read - hdrlen), _g.host, _g.ip, icmp->icmp_type, icmp->icmp_code);
    }
}