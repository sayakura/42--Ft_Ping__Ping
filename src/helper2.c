/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpeng <qpeng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 14:52:09 by qpeng             #+#    #+#             */
/*   Updated: 2019/04/21 07:27:46 by qpeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ping.h"

char	*c_to_s(int code)
{
	static char *code_lookup[] = {
		"Net is unreachable",
		"Host is unreachable",
		"Protocol is unreachable",
		"Port is unreachable",
		"Fragmentation is needed and Don't Fragment was set",
		"Source route failed",
		"Destination network is unknown",
		"Destination host is unknown",
		"Source host is isolated",
		"Communication with destination network is administratively prohibited",
		"Communication with destination host is administratively prohibited",
		"Destination network is unreachable for type of service",
		"Destination host is unreachable for type of service",
		"Communication is administratively prohibited",
		"Host precedence violation",
		"Precedence cutoff is in effect"
	};

	if (code < 0 || code > 15)
		return ("unknown code");
	return (code_lookup[code]);
}

char	*t_to_s(int type)
{
	static char *type_lookup[] = {
		"Echo reply", NULL, NULL,
		"Destination unreachable",
		"Source quench", "Redirect",
		NULL, NULL, "Echo",
		"Router advertisement",
		"Router selection",
		"Time exceeded",
		"Parameter problem",
		"Timestamp", "Timestamp reply",
		"Information request",
		"Information reply",
		"Address mask request",
		"Address mask reply",
		NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		"Traceroute"
	};

	if (type < 0 || type > 30)
		return ("unknown type");
	return (type_lookup[type]);
}

int		get_ip_v(char *src)
{
	char buf[100];

	if (inet_pton(AF_INET, src, buf))
		return (4);
	else if (inet_pton(AF_INET6, src, buf))
		return (6);
	return (0);
}
