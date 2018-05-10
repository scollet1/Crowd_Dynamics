/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scollet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/01 13:04:36 by scollet           #+#    #+#             */
/*   Updated: 2017/08/01 13:04:37 by scollet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cd.h"

void  reset(t_domain *domain)
{
  int   x;
  int   y;
  int   i;

  y = HEIGHT;
  i = -1;
  while (--y >= 0)
  {
    x = WIDTH;
    while (--x >= 0)
    {
      domain->world[0].grid[y][x] = domain->world[1].grid[y][x];
    }
  }
  domain->robot.location.x = domain->robot.start.x;
  domain->robot.location.y = domain->robot.start.y;
  /*while (++i < DEQUE)
  {
    memset(&domain.robot.memory[i].matrix.macro_state, 0, MACRO);
    memset(&domain.robot.memory[i].matrix.micro_state, 0, MICRO);
  }*/
  //int   a = ACTIONS;
  //while (--a >= 0)
  //  domain->robot.matrix.output[a] = 0.0;
  domain->robot.done = 0;
  return ;
}

void   reward(t_robot *robot, int t)
{
  int     score;
  double  distance;

  score = 0;
  distance = 0.0;
  if (robot->current_state.location.x == robot->goal.x &&
      robot->current_state.location.y == robot->goal.y)
  {
      score = 499;
      robot->done = DONE;
      if (t < robot->convergence_rate)
        score += 10, robot->convergence_rate = t;
      else
        score -= 1;
  }
  else
    score = -1;
  distance = euclidean(robot, &robot->current_state.location);
  if (distance <= robot->distance)
    score += 100, robot->distance = distance;
  //else
  //  score -= 20;
  robot->current_state.reward[robot->current_state.action] =
  (double)score;// * robot->gamma;
  robot->score += score;
  //robot->distance = distance;
  //printf("Rewarding... score, distance : %d, %f\n", score, distance);
  return ;
}

void   action(t_domain *domain, int action)
{
  domain->world[0].grid[(int)domain->robot.location.y]
                       [(int)domain->robot.location.x] = 0;
  domain->robot.location.y += domain->robot.action[0][action];
  domain->robot.location.x += domain->robot.action[1][action];
  domain->world[0].grid[(int)domain->robot.location.y]
                       [(int)domain->robot.location.x] = domain->robot.id;
  domain->robot.current_state.location.y = domain->robot.location.y;
  domain->robot.current_state.location.x = domain->robot.location.x;
  domain->robot.current_state.action = action;
  //domain->robot.memory[t].location.y = domain->robot.locaiton.y;
  //domain->robot.memory
  //domain->robot.memory[t].action = action.
  //display(domain->world[0]);
  return ;
}

int  move(t_domain *domain, int counter)
{
  int   good;
  int   action;

  good = 0;
  action = random_number(0, ACTIONS);
  if (random_number(0.0, 1.00) <= domain->robot.epsilon)
    good = check(domain, action);
  else if ((good = (check(domain, action =
    prediction(domain->robot.current_state.distribution)))))
    return (action);
  else if (counter > 10)
    return (0);
  action = (good)? action : move(domain, counter + 1);
  //printf("Action = %d\n", action);
  return (action);
}

void  observe(t_domain *domain)
{
  int   layer;

  layer = 0;
  feed_inputs(&domain->robot, layer);
  while (++layer < LAYERS)
  {
    //printf("\n\nlayer : %d\n\n", layer);
    feed_forward(&domain->robot.matrix, layer);
  }
  soft_max(&domain->robot);
  //printf("Leaving observe\n");
  return ;
}

void  run(t_domain domain)
{
  int     episode;
  int     t;

  episode = -1;
  while (++episode < EPISODES)
  {
    //printf("Episode : %d\n", episode);
    //display_world(&domain.world[0]);
    reset(&domain);
    t = -1;
    while (++t < DEQUE)
    {
      get_state(&domain);
      //printf("State acquired\n");
      observe(&domain);
      //printf("State observed\n");
      action(&domain, move(&domain, 0));
      //printf("Action Taken\n");
      reward(&domain.robot, t);
      //printf("Rewards given at time : %d\n", t);
      remember(&domain.robot.memory[t], &domain.robot);
      //printf("And hopefully we remembered something\n");
        //t == DEQUE;
      //if (!(episode % 10))
      //{

      /**/
      display_world(&domain.world[0]);
      display_weights(&domain.robot.matrix.neural_network);
      display_output(&domain.robot.matrix.neural_network);
      printf("\nPlaying at time on episode : %d, %d\n", t, episode);
      display_stats(&domain.robot, t);
      /**/
      if (domain.robot.done == 1)
        break ;
      //}
    }
    //display_world(&domain.world[0]);
    learn(&domain, t);
  }
  //exit(1);
  return ;
}
