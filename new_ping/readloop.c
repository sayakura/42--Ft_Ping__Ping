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