/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amansour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/22 14:23:47 by amansour          #+#    #+#             */
/*   Updated: 2019/08/04 12:07:05 by amansour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "ft_ssl.h"

static void		hash_stdin(t_flags fl, void (*c)(char*, t_flags, char*, t_u64))
{
	char	*str;
	t_u64	len;

	len = read_stdin(&str);
	fl.p > 0 ? ft_putstr_size(str, len) : 0;
	c(str, fl, NULL, len);
	free(str);
}

static	void	hash_file(t_flags fl, void (*cmd)(char*, t_flags, char*, t_u64)
				, char *s, char *c)
{
	char	*msg;
	t_u64	len;

	len = file_check(s, c, &msg);
	if (msg)
	{
		cmd(msg, fl, s, len);
		free(msg);
	}
}

static	void	p_flag(t_flags *flags, void (*cmd)(char*, t_flags,
				char*, t_u64))
{
	hash_stdin(*flags, cmd);
	while (flags->p > 1)
	{
		cmd("", *flags, NULL, 0);
		flags->p -= 1;
	}
}

static int		all_flag(t_flags *f, int argc, char **argv, void (*c)(char*,
				t_flags, char*, t_u64))
{
	int i;

	i = flags_check(argv, argc, f, 1);
	if (f->p || (!f->s && (i == argc)))
		p_flag(f, c);
	while (f->str)
	{
		f->p = 0;
		c(f->str, *f, f->str, ft_strlen(f->str));
		f->s = 0;
		f->str = NULL;
		i = flags_check(argv, argc, f, i - 1);
		if (f->p)
			p_flag(f, c);
	}
	if (f->error)
		flag_error(f->error, argv[1]);
	return (i);
}

int				main(int argc, char **argv)
{
	t_flags		flags;
	void		(*cmd)(char*, t_flags, char*, t_u64);
	int			i;

	(argc < 2) ? usage() : 0;
	cmd_check(argv[1], &cmd);
	i = all_flag(&flags, argc, argv, cmd);
	if (flags.s && !flags.str)
		s_error(argv[1]);
	if (i == argc && flags.p && !flags.s && !flags.error &&
			(flags.r || flags.q))
		cmd("", flags, NULL, 0);
	while (i < argc)
	{
		hash_file(flags, cmd, argv[i], argv[1]);
		i++;
	}
	return (0);
}
