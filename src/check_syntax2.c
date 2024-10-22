/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 11:16:01 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/22 12:26:48 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_or(t_check *check, size_t *i)
{
	if (check->or == 0 && check->redirect == 0 && check->ampersand == 0
			&& check->pipe == 0 && check->text == 1)
		check->or = 1;
	else
	{
		error_sys("minishell: syntax error near unexpected token `||'\n");
		return (0);
	}
	(*i)++;
	return (1);
}

int	handle_pipe(t_check *check)
{
	if (check->pipe == 0 && check->redirect == 0 && check->or == 0
			&& check->ampersand == 0 && check->text == 1)
		check->pipe = 1;
	else
	{
		error_sys("minishell: syntax error near unexpected token `|'\n");
		return (0);
	}
	return (1);
}

int	handle_and(t_check *check, size_t *i)
{
	if (check->ampersand == 0 && check->redirect == 0 && check->pipe == 0
			&& check->or == 0 && check->text == 1)
		check->ampersand = 1;
	else
	{
		error_sys("minishell: syntax error near unexpected token `&&'\n");
		return (0);
	}
	(*i)++;
	return (1);
}

int	handle_ampersand(t_check *check)
{
	if (check->ampersand == 0 && check->redirect == 0 && check->pipe == 0
			&& check->or == 0 && check->text == 1)
		check->ampersand = 1;
	else
	{
		error_sys("minishell: syntax error near unexpected token `&'\n");
		return (0);
	}
	return (1);
}

int	handle_text(t_check *check, char *input, size_t *i)
{
	char	quote;

	if (input[*i] == '\'' || input[*i] == '\"')
	{
		quote = input[*i];
		i++;
		while (input[*i] && input[*i] != quote)
		{
			(*i)++;
		}
		if (input[*i] == '\0')
			(*i)--;
	}
	check->text = 1;
	check->or = 0;
	check->pipe = 0;
	check->redirect = 0;
	check->ampersand = 0;
}