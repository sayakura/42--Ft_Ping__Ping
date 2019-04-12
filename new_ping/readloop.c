#include "ping.h"
uint16_t

in_cksum (uint16_t * addr, int len)
 {
    int     nleft = len;
    uint32_t sum = 0;
    uint16_t *w = addr;
    uint16_t answer = 0;

    while (nleft > 1) {
        sum += *w++;
        nleft -= 2;
     }
    if (nleft == 1) {
        * (unsigned char *) (&answer) = * (unsigned char *) w;
        sum += answer;
     }
    sum = (sum >> 16) + (sum & 0xffff); /* add hi 16 to low 16 */
    sum += (sum >> 16);     /* add carry */
    answer = ~sum;     /* truncate to 16 bits */
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
    memset(icmp->icmp_data, 0xff, DATALEN);
    gettimeofday((struct timeval *)icmp->icmp_data, NULL);
    len = 8 + DATALEN;
    icmp->icmp_cksum = 0;
    icmp->icmp_cksum = in_cksum((u_short *) icmp, len);
    len = sendto(_g.sockfd, _g.sendbuf, len, 0, _g.ssend, _g.ssendlen);
    printf("sent: %d\n", len);
}

void    sig_alrm(int signo)
{
	// send 
    send_v4();
	alarm(1);
	return;		/* probably interrupts recvfrom() */
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
        printf("%d\n", _tmp);
    }
    // sig_alarm(SIGALRM);
    
    // while (1)
    // {
    //     n = ft_recvmsg();
    //     // gettimeofday(&tval, NULL);
    //     printf("received: %ld\n", n);
    // }
}
