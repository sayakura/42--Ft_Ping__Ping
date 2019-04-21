/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpeng <qpeng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 14:52:09 by qpeng             #+#    #+#             */
/*   Updated: 2019/04/21 06:13:38 by qpeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ping.h"

void	make_addrinfo(char *src, int version)
{
	static struct addrinfo		pckt;
	static char					buf[16];
	struct sockaddr_in			*addr_4;
	struct sockaddr_in6			*addr_6;

	pckt.ai_family = version == 4 ? AF_INET : AF_INET6;
	if (version == 4)
	{
		addr_4 = (struct sockaddr_in *)pckt.ai_addr;
		addr_4->sin_family = AF_INET;
		addr_4->sin_port = 0;
		addr_4->sin_addr.s_addr = inet_pton(AF_INET, src, buf);
		pckt.ai_addrlen = sizeof(struct sockaddr_in);
		return (pckt);
	}
	else
	{
		addr_6 = (struct sockaddr_in6 *)pckt.ai_addr;
	}
}

int		addr_hdlr(char *src)
{
	char buf[16];

	if (inet_pton(AF_INET, src, buf))
		return (4);
	else if (inet_pton(AF_INET6, src, buf))
		return (6);
	return (0);
}
