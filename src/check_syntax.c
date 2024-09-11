/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 19:27:15 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/11 16:51:03 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_redirect(char *input)
{
	if (ft_strncmp(input, ">>", 2) == 0 || ft_strncmp(input, "<>", 2) == 0)
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
	size_t	len;
	size_t	i;
	int or;
	int	redirect;
	int ampersand;
	int	pipe;
	int	text;


	i = 0;
	or = 0;
	pipe = 0;
	redirect = 0;
	ampersand = 0;
	text = 0;
	len = ft_strlen(input);
	while (input[i])
	{
		if (is_redirection_operator(input[i]) && redirect == 0)
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
			redirect = 1;
		}
		else if (is_pipe(input[i]) && is_pipe(input[i + 1]))
		{
			if (or == 0 && redirect == 0 && ampersand == 0 && pipe == 0 && text == 1)
				or = 1;
			else
			{
				error_sys("minishell: syntax error near unexpected token `||'\n");
				return (1);
			}
			i++;
		}
		else if (is_pipe(input[i]))
		{
			if (pipe == 0 && redirect == 0 && or == 0 && ampersand == 0 && text == 1)
				pipe = 1;
			else
			{
				error_sys("minishell: syntax error near unexpected token `|'\n");
				return (1);
			}
		}
		else if (is_ampersand(input[i]) && is_ampersand(input[i + 1]))
		{
			if (ampersand == 0 && redirect == 0 && pipe == 0 && or == 0 && text == 1)
				ampersand = 1;
			else
			{
				error_sys("minishell: syntax error near unexpected token `&&'\n");
				return (1);
			}
			i++;
		}
		else if (is_ampersand(input[i]))
		{
			if (ampersand == 0 && redirect == 0 && pipe == 0 && or == 0 && text == 1)
				ampersand = 1;
			else
			{
				error_sys("minishell: syntax error near unexpected token `&'\n");
				return (1);
			}
		}
		else if (is_redirection_operator(input[i]) && redirect == 1)
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
			text = 1;
			or = 0;
			pipe = 0;
			redirect = 0;
			ampersand = 0;
		}
		i++;
	}
	return (0);
}
