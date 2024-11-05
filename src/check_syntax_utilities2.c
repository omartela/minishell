/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_utilities2.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:42:50 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/31 19:29:29 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_redirect(char *input)
{
	if (ft_strncmp(input, ">>", 2) == 0 || ft_strncmp(input, "<>", 2) == 0
		|| ft_strncmp(input, "<<", 2) == 0)
		return (1);
	return (0);
}

static void	handle_less_than(char *input, size_t i)
{
	size_t	len;

	len = ft_strlen(input);
	if ((i + 2 <= len) && input[i + 2] && input[i + 2] == '<')
		show_syntax_error("<<<");
	else if (input[i + 1] && input[i + 1] == '<')
		show_syntax_error("<<");
	else if (input[i + 1] && input[i + 1] == '>')
		show_syntax_error("<>");
	else
		show_syntax_error("<");
}

static int	handle_pipe_ampersand(char *input, size_t i)
{
	if (input[i] == '|')
	{
		if (input[i + 1] == '|')
			show_syntax_error("||");
		else
			show_syntax_error("|");
	}
	else if (input[i] == '&')
	{
		if (input[i + 1] == '&')
			show_syntax_error("&&");
		else
			show_syntax_error("&");
	}
	else
		return (1);
	return (0);
}

int	handle_first_redirect(t_check *check, char *input, size_t *i)
{
	if (input[(*i) + 1] == '\0')
	{
		show_syntax_error("newline");
		return (0);
	}
	if (is_redirection_operator(input[(*i) + 1])
		&& is_valid_redirect(input + (*i)))
	{
		(*i)++;
	}
	check->redirect = 1;
	return (1);
}

int	handle_second_redirect(char *input, size_t i)
{
	if (input[i] == '>')
	{
		if (input[i + 1] == '>')
			show_syntax_error(">>");
		else
			show_syntax_error(">");
		return (0);
	}
	else if (input[i] == '<')
	{
		handle_less_than(input, i);
		return (0);
	}
	else
		return (handle_pipe_ampersand(input, i));
	return (1);
}
