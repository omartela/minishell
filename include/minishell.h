/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 14:10:51 by omartela          #+#    #+#             */
/*   Updated: 2024/09/04 18:17:24 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_cmd
{
	char	**args;
	char	**path;
}	t_cmd;

int		init_cmd(t_cmd **cmd, const char *command, char **envp);
int		execute_pipes(char **commands, char **envp);
void	test_split(char *input); //only for test
char	**ft_split_args(char const *s, char c);
void	free_array(char **array);
void	free_cmd(t_cmd *cmd);

#endif
