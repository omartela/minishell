/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 19:27:15 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/22 12:17:36 by irychkov         ###   ########.fr       */
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

static int	is_redirection_operator(char c)
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
	char	quote;
	int		is_continue;
	size_t	len;
	size_t	i;
	t_check	check;

	i = 0;
	len = ft_strlen(input);
	is_continue = 1;
	ft_memset(&check, 0, sizeof(t_check));
	while (input[i])
	{
		if (is_redirection_operator(input[i]) && check.redirect == 0)
		{
			if (input[i + 1] == '\0') // or newline?
			{
				error_sys("minishell: syntax error near unexpected token `newline'\n");
				return (1);
			}
			if (is_redirection_operator(input[i + 1]) && is_valid_redirect(input + i))
			{
				i++;
			}
			check.redirect = 1;
		}
		else if (is_pipe(input[i]) && is_pipe(input[i + 1]))
		{
			is_continue = handle_or(&check, &i);
		}
		else if (is_pipe(input[i]))
		{
			is_continue = handle_pipe(&check);
		}
		else if (is_ampersand(input[i]) && is_ampersand(input[i + 1]))
		{
			is_continue = handle_and(&check, &i);
		}
		else if (is_ampersand(input[i]))
		{
			is_continue = handle_ampersand(&check);
		}
		else if (is_redirection_operator(input[i]) && check.redirect == 1)
		{
			if (input[i] == '>')
			{
				if (input[i + 1] == '>')
					error_sys("minishell: syntax error near unexpected token `>>'\n");
				else
					error_sys("minishell: syntax error near unexpected token `>'\n");
				return (1);
			}
			else if (input[i] == '<')
			{
				if ((i + 2 <= len) && input[i + 2] && input[i + 2] == '<')
					error_sys("minishell: syntax error near unexpected token `<<<'\n");
				else if (input[i + 1] && input[i + 1] == '<')
					error_sys("minishell: syntax error near unexpected token `<<'\n");
				else if (input[i + 1] && input[i + 1] == '>')
					error_sys("minishell: syntax error near unexpected token `<>'\n");
				else
					error_sys("minishell: syntax error near unexpected token `<'\n");
				return (1);
			}
			else if (input[i] == '|')
			{
				if (input[i + 1] == '|')
					error_sys("minishell: syntax error near unexpected token `||'\n");
				else
					error_sys("minishell: syntax error near unexpected token `|'\n");
				return (1);
			}
			else if (input[i] == '&')
			{
				if (input[i + 1] == '&')
					error_sys("minishell: syntax error near unexpected token `&&'\n");
				else
					error_sys("minishell: syntax error near unexpected token `&'\n");
				return (1);
			}
		}
		else if (input[i] != ' ')
		{
			if (input[i] == '\'' || input[i] == '\"')
			{
				quote = input[i];
				i++;
				while (input[i] && input[i] != quote)
				{
					i++;
				}
				if (input[i] == '\0')
					i--;
			}
			check.text = 1;
			check.or = 0;
			check.pipe = 0;
			check.redirect = 0;
			check.ampersand = 0;
		}
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
