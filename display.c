/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scollet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/10 20:22:30 by scollet           #+#    #+#             */
/*   Updated: 2017/08/10 20:22:31 by scollet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cd.h"

void  display_world(t_grid *world)
{
  int   x;
  int   y;

  system("clear");
  printf("#~~~~~~~~~~~~~~~~~~~~~~~~~~~ Display ~~~~~~~~~~~~~~~~~~~~~~~~~~~~#\n");
  y = HEIGHT;
  while (--y >= 0)
  {
    x = WIDTH;
    printf("|");
    while (--x >= 0)
    {
      //printf("x, y : %d, %d\n", x, y);
      if (world->grid[y][x] == 0)
        printf(" ");
      else if (world->grid[y][x] == 1)
        printf("\x1B[31mx\x1b[0m");
      else if (world->grid[y][x] == 2)
        printf("\x1B[32mo\x1b[0m");
      else if (world->grid[y][x] == 3)
        printf("\x1B[35mo\x1b[0m");
    }
    printf("|\n");
  }
  printf("#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#\n");
  return ;
}

void  display_stats(t_robot *robot, int t)
{
  t=(t==0)?t:t-1;
  printf("\nScore : %d\n", robot->score);
  printf("Current reward : %f\n", robot->current_state.reward[robot->current_state.action]);
  printf("Epsilon : %f\n", robot->epsilon);
  printf("Closest distance to goal : %f\n", robot->distance);
  printf("Convergence time : %d\n", robot->convergence_rate);
  printf("Probability of staying still : %f\n", robot->current_state.distribution[0] * (1.0 - robot->epsilon));
  printf("Probability of moving down : %f\n", robot->current_state.distribution[1] * (1.0 - robot->epsilon));
  printf("Probability of moving right : %f\n", robot->current_state.distribution[2] * (1.0 - robot->epsilon));
  printf("Probability of moving up : %f\n", robot->current_state.distribution[3] * (1.0 - robot->epsilon));
  printf("Probability of moving left : %f\n", robot->current_state.distribution[4] * (1.0 - robot->epsilon));
  return ;
}

void  display_weights(t_network *network)
{
  int   layer;
  int   neuron;

  layer = -1;
  //system("clear");
  while (++layer < LAYERS)
  {
    neuron = -1;
    while (++neuron < NEURONS)
    {
      //printf("neuron, layer : %d, %d\n", neuron, layer);
      if (!(neuron % (NEURONS / 4)))
        printf("\n");
      else if (network->hidden[layer].weight[neuron] <= 0.10 &&
               network->hidden[layer].weight[neuron] >= 0.00)
        printf(" ");
      else if (network->hidden[layer].weight[neuron] <= 0.25 &&
               network->hidden[layer].weight[neuron] >= 0.11)
        printf("`");
      else if (network->hidden[layer].weight[neuron] <= 0.50 &&
               network->hidden[layer].weight[neuron] >= 0.26)
        printf(".");
      else if (network->hidden[layer].weight[neuron] <= 0.75 &&
               network->hidden[layer].weight[neuron] >= 0.51)
        printf("~");
      else if (network->hidden[layer].weight[neuron] <= 0.90 &&
               network->hidden[layer].weight[neuron] >= 0.76)
        printf("+");
      else if (network->hidden[layer].weight[neuron] <= 0.95 &&
               network->hidden[layer].weight[neuron] >= 0.91)
        printf("x");
      else
        printf("#");
      //printf("%f\n", network->hidden[0].weight[0]);
    }
    printf("\n");
  }
  //printf("Leaving display weights\n");
  printf("\n\n");
  return ;
}

void  display_output(t_network *network)
{
  int   layer;
  int   neuron;

  layer = -1;
  //system("clear");
  while (++layer < LAYERS)
  {
    neuron = -1;
    while (++neuron < NEURONS)
    {
      //printf("neuron, layer : %d, %d\n", neuron, layer);
      if (!(neuron % (NEURONS / 4)))
        printf("\n");
      else if (network->hidden[layer].output[neuron] <= 0.10 &&
               network->hidden[layer].output[neuron] >= 0.0)
        printf(".");
      else if (network->hidden[layer].output[neuron] <= 0.49 &&
               network->hidden[layer].output[neuron] >= 0.11)
        printf("~");
      else if (network->hidden[layer].output[neuron] <= 0.89 &&
               network->hidden[layer].output[neuron] >= 0.50)
        printf("+");
      else if (network->hidden[layer].output[neuron] <= 0.99 &&
               network->hidden[layer].output[neuron] >= 0.90)
        printf("#");
      else if (network->hidden[layer].output[neuron] <= 5.00 &&
               network->hidden[layer].output[neuron] >= 1.00)
        printf("X");
      else if (network->hidden[layer].output[neuron] <= 10.0 &&
               network->hidden[layer].output[neuron] >= 5.01)
        printf("x");
      else
        printf(" ");
      //printf("%f\n", network->hidden[0].weight[0]);
    }
    printf("\n");
  }
  printf("\n\n");
  //printf("Leaving display weights\n");
  return ;
}

/*
void display_memory()
{

}
*/
