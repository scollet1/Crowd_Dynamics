/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brain.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scollet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/08 12:20:24 by scollet           #+#    #+#             */
/*   Updated: 2017/08/08 12:20:25 by scollet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cd.h"

int   remember(t_grid *world, t_robot *robot, int t)
{
  robot->memory[t].location.x = robot->location.x;
  robot->memory[t].location.y = robot->location.y;
  for (int d = 0; d < ACTIONS; d++){robot->memory[t].distribution[d] =
                              robot->current_state.distribution[d];}
  robot->memory[t].reward = robot->current_state.reward;
  robot->memory[t].action = robot->current_state.action;
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      robot->memory[t].world.grid[y][x] = world->grid[y][x];
    }
  }
  return (0);
}
