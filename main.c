/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scollet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/17 15:19:57 by scollet           #+#    #+#             */
/*   Updated: 2017/07/19 00:56:07 by scollet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cd.h"

int   main()
{
  time_t    t;
  int       running;
  static t_domain  domain;

  printf("Begin\n");
  fflush(stdout);
  srand((unsigned)time(&t));
  running = 0;
  domain = *(t_domain*)calloc(1, sizeof(t_domain));
  //for (int i = 0; i < 100; i++)
  //  printf("testing random number generator : %i\n", (int)random_number(0, 5));
  printf("Running\n");
  //exit(1);
  domain = init(domain, running);
  //display_world(&domain.world[0]);
  //display_world(&domain.world[1]);
  run(domain);
  return (0);
}
