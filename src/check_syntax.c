/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 19:27:15 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/22 13:32:46 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirection_operator(char c)
{
	if (c == '<' || c == '>')
		return (1);
	return (0);
}

static int	is_pipe(char c)
{
	if (c == '|')
		return (1);
	return (0);
}

static int	is_ampersand(char c)
{
	if (c == '&')
		return (1);
	return (0);
}

int	check_syntax(char *input)
{
	int		is_continue;
	size_t	i;
	t_check	check;

	i = 0;
	is_continue = 1;
	ft_memset(&check, 0, sizeof(t_check));
	while (input[i])
	{
		if (is_redirection_operator(input[i]) && check.redirect == 0)
			is_continue = handle_first_redirect(&check, input, &i);
		else if (is_pipe(input[i]) && is_pipe(input[i + 1]))
			is_continue = handle_or(&check, &i);
		else if (is_pipe(input[i]))
			is_continue = handle_pipe(&check);
		else if (is_ampersand(input[i]) && is_ampersand(input[i + 1]))
			is_continue = handle_and(&check, &i);
		else if (is_ampersand(input[i]))
			is_continue = handle_ampersand(&check);
		else if (is_redirection_operator(input[i]) && check.redirect == 1)
			is_continue = handle_second_redirect(input, i);
		else if (input[i] != ' ')
			handle_text(&check, input, &i);
		if (!is_continue)
			return (1);
		i++;
	}
	if (check.redirect == 1)
	{
		error_sys("minishell: syntax error near unexpected token `newline'\n");
		return (1);
	}
	return (0);
}
