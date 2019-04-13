#include "ping.h"

void        error(char *str)
{
    fprintf(stderr, str);
    exit(EXIT_FAILURE);
}

void   tv_sub (struct timeval *out, struct timeval *in)
{
    if ((out->tv_usec -= in->tv_usec) < 0) 
    {    
        --out->tv_sec;
        out->tv_usec += 1000000;
    }
    out->tv_sec -= in->tv_sec;
}

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

void    sig_alrm(int signo)
{
    send_v4();
	alarm(1);
	return;	
}

void    readmsg(int b_read, char *recvbuff)
{
    struct ip       *iphdr;
    struct icmp     *icmp;
    struct timeval *tvsend;
    struct timeval  tvrecv;
    double          _tmp;
    int             hdrlen;

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
        tvsend = (struct timeval *)icmp->icmp_data;
        tv_sub(&tvrecv, tvsend);
        printf ("%d bytes from %s: icmp_seq=%u, ttl=%d, time=%.3f ms\n",
            (b_read - hdrlen), "google.com", icmp->icmp_seq, 
            iphdr->ip_ttl, (tvrecv.tv_sec * 1000.0 + tvrecv.tv_usec / 1000.0));
    }
}

void    readloop(void)
{
    int             _tmp;
    char            recvbuff[BUFF_SIZE];
    char            ctrlbuff[BUFF_SIZE];
   
    _g.sockfd = socket(_g.ssend->sa_family, SOCK_RAW, IPPROTO_ICMP);
    _tmp = 60 * 1024;
	setsockopt(_g.sockfd , SOL_SOCKET, SO_RCVBUF, &_tmp, sizeof(_tmp));
    _g.iov.iov_base = recvbuff;
    _g.iov.iov_len = sizeof(recvbuff);
    _g.msg.msg_name = _g.srecv;
    _g.msg.msg_iov = &(_g.iov);
    _g.msg.msg_iovlen = 1;
    _g.msg.msg_control = ctrlbuff;
    sig_alrm(SIGALRM);
    while (1)
    {
        _g.msg.msg_namelen = _g.ssendlen;
        _g.msg.msg_controllen = sizeof(ctrlbuff);
        _tmp = recvmsg(_g.sockfd, &(_g.msg), 0);
        if (_tmp < 0)
            if (errno = EINTR)
                continue ; 
            else
                error("recvmsg error");
        readmsg(_tmp, recvbuff);
    }
}
