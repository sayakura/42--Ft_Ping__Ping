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
    int         nleft = len;
    uint32_t    sum = 0;
    uint16_t    *w = addr;
    uint16_t    answer = 0;

    while (nleft > 1)
    {
        sum += *w++;
        nleft -= 2;
     }
    if (nleft == 1)
    {
        * (unsigned char *) (&answer) = * (unsigned char *) w;
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
    printf("%d\n",  _g.msg_cnt);
    icmp->icmp_seq = _g.msg_cnt++;
    memset(icmp->icmp_data, 0xff, DATALEN);
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

void    readmsg(int b_read)
{
    struct ip       *iphdr;
    struct icmp     *icmp;
    struct timeval *tvsend;
    struct timeval  tvrecv;
    double          _tmp;
    int             hdrlen;

    gettimeofday(&tvrecv, NULL);
    iphdr = (struct ip *)_g.recvbuf;
    hdrlen = iphdr->ip_hl << 2;
    printf("hdrlen: %d iphdr->ip_hl: %d\n", hdrlen, iphdr->ip_hl);
    if (iphdr->ip_p != IPPROTO_ICMP)
        return ;
    icmp = (struct icmp *)(iphdr + hdrlen);
    if ((b_read - hdrlen) < 8)
        return ;
    if (icmp->icmp_type == ICMP_ECHOREPLY)
    {
        if (icmp->icmp_id != _g.pid || (b_read - hdrlen) < 16)
            return ;
        tvsend = (struct timeval *)icmp->icmp_data;
        printf("recv: %lld; send: %lld", tvrecv.tv_sec, tvsend->tv_sec);
        tv_sub(&tvrecv, tvsend);
        _tmp = tvrecv.tv_sec * 1000.0 + tvrecv.tv_usec / 1000.0;
        printf ("%d bytes from %s: icmp_seq=%u, ttl=%d, time=%.3f ms\n",
            (b_read - hdrlen), "google.com", icmp->icmp_seq, 
            iphdr->ip_ttl, _tmp);
    }
}

void    readloop(void)
{
    int             _tmp;
   
    _g.pid = getuid();
    setuid(_g.pid);
    _g.sockfd = socket(_g.ssend->sa_family, SOCK_RAW, IPPROTO_ICMP);
    _tmp = 60 * 1024;
	setsockopt(_g.sockfd , SOL_SOCKET, SO_RCVBUF, &_tmp, sizeof(_tmp));
    _g.iov.iov_base = _g.recvbuf;
    _g.iov.iov_len = sizeof(_g.recvbuf);
    _g.msg.msg_name = _g.srecv;
    _g.msg.msg_iov = &_g.iov;
    _g.msg.msg_iovlen = 1;
    _g.msg.msg_control = _g.ctrl_buf;
    sig_alrm(SIGALRM);
    while (1)
    {
        _g.msg.msg_namelen = _g.ssendlen;
        _g.msg.msg_controllen = sizeof(_g.ctrl_buf);
        _tmp = recvmsg(_g.sockfd, &_g.msg, 0);
        if (_tmp < 0)
            if (errno = EINTR)
                continue ; 
            else
                error("recvmsg error");
        readmsg(_tmp);
    }
}
