/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpeng <qpeng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 14:52:09 by qpeng             #+#    #+#             */
/*   Updated: 2019/04/21 06:41:05 by qpeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ping.h"

int		get_ip_v(char *src)
{
	char buf[100];

	if (inet_pton(AF_INET, src, buf))
		return (4);
	else if (inet_pton(AF_INET6, src, buf))
		return (6);
	return (0);
}
