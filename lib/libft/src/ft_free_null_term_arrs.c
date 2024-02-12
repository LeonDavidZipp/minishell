/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_null_term_arrs.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 00:23:00 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/04 00:33:31 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	ft_free_2d_arr(void **arr)
{
	int	i;

	i = -1;
	while (arr[++i])
		ft_safe_free(arr[i]);
	ft_safe_free(arr);
}

void	ft_free_3d_arr(void ***arr)
{
	int	i;

	i = -1;
	while (arr[++i])
		ft_free_2d_arr(arr[i]);
	ft_safe_free(arr);
}

void	ft_free_4d_arr(void ****arr)
{
	int	i;

	i = -1;
	while (arr[++i])
		ft_free_3d_arr(arr[i]);
	ft_safe_free(arr);
}
