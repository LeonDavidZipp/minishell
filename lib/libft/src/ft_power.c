/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_power.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 00:44:32 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/04 00:47:44 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

long long	ft_power(long long base, long long exponent)
{
	long long	result;

	if (exponent < 0 || base == 0)
		return (0);
	result = 1;
	while (exponent > 0)
	{
		result *= base;
		exponent--;
	}
	return (result);
}
