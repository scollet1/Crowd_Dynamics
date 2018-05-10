/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scollet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/18 02:06:10 by scollet           #+#    #+#             */
/*   Updated: 2017/07/19 00:57:48 by scollet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cd.h"

t_agent   agent(t_agent agent)
{
  agent.goal.x = (int)random_number(0, 99);
  agent.goal.y = (int)random_number(0, 99);
  agent.start.x = (int)random_number(0, 99);
  agent.start.y = (int)random_number(0, 99);
  agent.id = 1;
  return (agent);
}

void    matrices(t_matrix *matrix, int layers, int length)
{
  int   i;
  int   j;

  matrix->macro_state = (double*)calloc(MACRO, sizeof(double));
  matrix->micro_state = (double*)calloc(MICRO, sizeof(double));
  matrix->neural_network = *(t_network*)calloc(1, sizeof(t_network));
  matrix->neural_network.hidden = (t_hidden*)calloc(LAYERS, sizeof(t_hidden));
  i = -1;
  while (++i < layers)
  {
    matrix->neural_network.hidden[i].neuron = (t_neuron*)calloc(NEURONS, sizeof(t_neuron));
    matrix->neural_network.hidden[i].weight = (double*)calloc(NEURONS, sizeof(double));
    matrix->neural_network.hidden[i].output = (double*)calloc(NEURONS, sizeof(double));
    j = -1;
    while (++j < length)
    {
      matrix->neural_network.hidden[i].weight[j] = random_number(0.01, 0.95);
      matrix->neural_network.hidden[i].neuron[j].delta = random_number(0.01, 1.00);
      matrix->neural_network.hidden[i].output[j] = 0.02;
      matrix->neural_network.hidden[i].neuron[j].bias = random_number(0.01, 0.05);
    }
  }
  //printf("this %f\n", matrix->neural_network.hidden[0].output[63]);
  printf("Leave matrices\n");
  //exit(0);
  return ;
}

void   robot(t_robot *robot)
{
  robot->goal = *(t_vector*)calloc(1, sizeof(t_vector));
  robot->start = *(t_vector*)calloc(1, sizeof(t_vector));
  robot->location = *(t_vector*)calloc(1, sizeof(t_vector));
  robot->matrix = *(t_matrix*)calloc(1, sizeof(t_matrix));
  robot->current_state = *(t_state*)calloc(1, sizeof(t_state));
  if ((robot->memory = (t_state*)malloc(DEQUE * sizeof(t_state))) == NULL)
    printf("malloc'd\n"), exit(1);
  /*for (i = 0; i < DEQUE; i++)
  {
    robot->memory[i].location.x = 0;
    robot->memory[i].location.y = 0;
  }*/

  /* Jus' debuggin'
  for (i = 0; i < DEQUE; i++)
  {
    int   j = MICRO;
    while (--j >= 0)
      printf("%f @ %d\n", robot->memory[i].micro_state[j], i);
  }
   Nothing to see here */

  //robot->action = (int**)calloc(LAYERS, sizeof(int));
  //for (i = LAYERS - 1; i >= 0; i--)
  //  robot->action[i] = (int*)calloc(NUM_ACTIONS, sizeof(int));
  robot->goal.x = (int)random_number(WIDTH / 2, WIDTH - 1);
  robot->goal.y = (int)random_number(HEIGHT / 2, HEIGHT - 1);
  robot->start.x = (int)random_number(5, 10);
  robot->start.y = (int)random_number(5, 10);
  robot->location.x = robot->start.x;
  robot->location.y = robot->start.y;
  matrices(&robot->matrix, LAYERS, NEURONS);
  robot->distance = euclidean(robot, &robot->start);
  robot->id = 2;
  robot->epsilon = EPSILON;
  robot->lambda = LAMBDA;
  robot->gamma = GAMMA;
  robot->convergence_rate = DEQUE;
  robot->action[0][0] = 0;
  robot->action[0][1] = 1;
  robot->action[0][2] = 0;
  robot->action[0][3] = -1;
  robot->action[0][4] = 0;
  robot->action[1][0] = 0;
  robot->action[1][1] = 0;
  robot->action[1][2] = 1;
  robot->action[1][3] = 0;
  robot->action[1][4] = -1;
  return ;
}

t_domain    spawn(t_domain domain)
{
  int   i;
  int   j;

  j = -1;
  //memset(&domain.world, 0, sizeof(domain.world));
  while (++j < UNIVERSE)
  {
    domain.world[j] = *(t_grid*)calloc(1, sizeof(t_grid));
    i = -1;
    while (++i < NUM_AGENTS)
    {
      domain.world[j].grid[(int)domain.agents[i].start.y]
                          [(int)domain.agents[i].start.x] = domain.agents[i].id;
    }
    //domain.world[j].grid[domain.robot->start.y]
    //                    [domain.robot->start.x] = domain.robot->ID;
    domain.world[j].grid[(int)domain.robot.goal.y]
                        [(int)domain.robot.goal.x] = 3;
  }
  return (domain);
}

t_domain  init(t_domain domain, int running)
{
  int   i;

  //printf("Initializing...\n");
  domain.agents = (t_agent*)malloc(NUM_AGENTS * sizeof(t_agent));
  domain.robot = *(t_robot*)malloc(sizeof(t_robot));
  domain.world = (t_grid*)malloc(UNIVERSE * sizeof(t_grid));
  if (!running)
    robot(&domain.robot);
  for (i = NUM_AGENTS; i >= 0; i--)
    domain.agents[i] = agent(domain.agents[i]);
  domain = spawn(domain);
  //display_world(&domain.world[0]);
  //printf("Done initizlizing HERE\n");
  return (domain);
}
