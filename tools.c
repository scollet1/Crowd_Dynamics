/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scollet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/01 13:00:40 by scollet           #+#    #+#             */
/*   Updated: 2017/08/12 01:43:25 by scollet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cd.h"

/*
int   optimal(t_robot *robot)
{
  int   i;
  int   action;

  i = -1;
  action = 0;
  while (++i < ACTIONS)
  {
    if (euclidean(robot, robot.location) < )

  }
}*/

int   prediction(double *distribution)
{
  int     i;
  int  action;

  i = ACTIONS;
  action = 0;
  while (--i >= 0)
    if (distribution[i] > distribution[action])
      action = i;
  return (action);
}

int   check(t_domain *domain, int action)
{
  int   x;
  int   y;

  y = domain->robot.location.y + domain->robot.action[0][action];
  x = domain->robot.location.x + domain->robot.action[1][action];
  if ((domain->world[0].grid[y][x]) != 1 && (x >= 0 && x < WIDTH) && (y >= 0 && y < HEIGHT))
      return (1);
  return (0);
}

double   euclidean(t_robot *robot, t_vector *location)
{
  double   distance;

  distance = (double)sqrt((double)pow((double)location->x - (double)robot->goal.x, 2) +
                         ((double)pow((double)location->y - (double)robot->goal.y, 2)));
  return (distance);
}

int  get_state(t_domain *domain)
{
  int   x;
  int   y;
  int   i;
  int   square;

  y = -1;
  i = MICRO;
  square = (int)sqrt(MICRO / 4);
  while (++y < HEIGHT)
  {
    x = -1;
    while (++x < WIDTH)
    {
      if (y <= domain->robot.location.y && x <= domain->robot.location.x &&
          y >= domain->robot.location.y - square && x >= domain->robot.location.x - square)
      {
        domain->robot.matrix.macro_state[0] += (domain->world[0].grid[y][x] == 1)? 1 : 0;
        domain->robot.matrix.micro_state[--i] = (domain->world[0].grid[y][x] == 1)? 1 : 0;
      }
      if (y <= domain->robot.location.y && x >= domain->robot.location.x &&
          y >= domain->robot.location.y - square && x <= domain->robot.location.x + square)
      {
        domain->robot.matrix.macro_state[1] += (domain->world[0].grid[y][x] == 1)? 1 : 0;
        domain->robot.matrix.micro_state[--i] = (domain->world[0].grid[y][x] == 1)? 1 : 0;
      }
      if (y >= domain->robot.location.y && x <= domain->robot.location.x &&
          y <= domain->robot.location.y + square && x >= domain->robot.location.x - square)
      {
        domain->robot.matrix.macro_state[2] += (domain->world[0].grid[y][x] == 1)? 1 : 0;
        domain->robot.matrix.micro_state[--i] = (domain->world[0].grid[y][x] == 1)? 1 : 0;
      }
      if (y >= domain->robot.location.y && x >= domain->robot.location.x &&
          y <= domain->robot.location.y + square && x <= domain->robot.location.x + square)
      {
        domain->robot.matrix.macro_state[3] += (domain->world[0].grid[y][x] == 1)? 1 : 0;
        domain->robot.matrix.micro_state[--i] = (domain->world[0].grid[y][x] == 1)? 1 : 0;
      }
      /*if (y == domain->robot.location.y && x > domain->robot.location.x - 8)
	    {
        //domain->robot.matrix.macro_state[3] += (domain->world[0].grid[y][x] == 1)? 1 : 0;
        domain->robot.matrix.micro_state[i++] = (domain->world[0].grid[y][x] == 1)? 1 : 0;
      }
      if (x == domain->robot.location.x && y > domain->robot.location.y - 8)
      {
        domain->robot.matrix.micro_state[i++] = (domain->world[0].grid[y][x] == 1)? 1 : 0;
      }*/
	  }
  }
  //printf("i : %d\n", i);
  while (i >= 0)
    domain->robot.matrix.micro_state[i--] = 0.5;
  //i = MICRO;
  //printf("This at here %f\n", domain->robot.matrix.macro_state[3]);
  //while (--i >= 0)
  //  printf("Micro : %f at i %d\n", domain->robot.matrix.micro_state[i], i);
  //printf("%d\n", MACRO);
  //i = MACRO;
  //exit(0);
  domain->robot.matrix.macro_state[0] /= MACRO;
  domain->robot.matrix.macro_state[1] /= MACRO;
  domain->robot.matrix.macro_state[2] /= MACRO;
  domain->robot.matrix.macro_state[3] /= MACRO;
  //while (--i >= 0)
  //  printf("Macro : %f at i %d\n", domain->robot.matrix.macro_state[i], i);
  return (0);
}

double  sum_inputs(double *state, int length)
{
  int     i;
  double  sum;

  i = -1;
  sum = 0.0;
  while (++i < length)
    sum += state[i];
  return (sum);
}

double   sum_over(t_hidden hidden, int len)
{
  int     i;
  double  sum;

  sum = 0.0;
  for (i = 0; i < len; i++)
    sum += hidden.weight[i];
  //printf("leaving sum_over\n");
  return (sum);
}

void  update_weights(t_matrix *matrix, int length)
{
  int     layer;
  int     s;
  int     a;
  int     neuron;
  double  *input;
  static double  max;

  layer = LAYERS;
  input = (double*)calloc(length, sizeof(double));
  a = ACTIONS;
  //max = 0.0;
  while (--layer >= 0)
  {
    //printf("layer : %d\n", layer);
    s = length;
    neuron = NEURONS;
    while (--s >= 0)
    {
      input[s] = matrix->neural_network.hidden[layer].output[s] + 0.0001;
      //printf("inputs : %f\n", input[s]);
      //if (input[s] != input[s]) input[s] = 0.0;
      //a = ACTIONS;
      //while (--a >= 0)
      //  input[a] += matrix->output[a];
    }
    //exit(1);
    while (--neuron >= 0)
    {
      s = length;
      //printf("neuron clearly before %f\n", matrix->neural_network.hidden[layer].weight[neuron]);
      while (--s >= 0)
        matrix->neural_network.hidden[layer].weight[neuron] +=
        LEARNING_RATE * matrix->neural_network.hidden[layer].neuron[neuron].delta * input[s];
      //printf("neuron clearly after %f\n", network->hidden[layer].weight[neuron]);
      if (matrix->neural_network.hidden[layer].weight[neuron] > max) max = matrix->neural_network.hidden[layer].weight[neuron] + 0.00001;
      matrix->neural_network.hidden[layer].weight[neuron] = normalize(matrix->neural_network.hidden[layer].weight[neuron], max);
    }
    //network->hidden[layer].weight[0] +=
    //LEARNING_RATE * network->hidden[layer].neuron[0].delta;
    //network->hidden[layer].weight[neuron] +=
    //LEARNING_RATE * network->hidden[layer].neuron[neuron].delta;
  }
  //exit(1);
  //printf("Leaving update_weights\n");
  free(input);
  return ;
}

double  derivative(double output)
{
  //printf("snbnsoi : %f, %f\n", output * (1.0 + output), output);
  return (output * (1.0 - output));
}

double  largest(double *d) /* ( ͡° ͜ʖ ͡°) */
{
  double  largest;
  int     action;

  largest = 0;
  action = -1;
  while (++action < ACTIONS)
    if (largest < d[action])
      largest = d[action];
  return (largest);
}

void  soft_max(t_robot *robot)
{
  int     action;
  double  *array;
  double  sum;

  array = (double*)calloc(ACTIONS, sizeof(double));
  sum = 0.001;
  for (action = ACTIONS - 1; action >= 0; action--)
  {
    //printf("%f\n", robot->matrix.output[action]);
    //printf("power %f\n", pow(E, robot->matrix.output[action]);// - robot->matrix.output[prediction(robot->matrix.output)]));
    array[action] = pow(E, robot->matrix.output[action] -
    robot->matrix.output[prediction(robot->matrix.output)]);
    //printf("array of actions versus outputs :: %f, %f\n", array[action], robot->matrix.output[action]);
  }
  sum += sum_inputs(array, ACTIONS);
  //printf("sum num %f\n", sum_inputs(array, ACTIONS));
  for (action = 0; action < ACTIONS; action++)
  {
    robot->current_state.distribution[action] = array[action] / sum;
    //printf("distribution of actions :: %f\n", robot->current_state.distribution[action]);
  }
  //thing = pow(E, robot->matrix.output[i]);
  //for (j = ACTIONS - 1; j >= 0; j--)
  //  thing += pow(E, sum);
  //printf("sumthing : %f, %f\n", sum, thing);
  //}
  //sum = pow(E, robot->current_state.reward[i] -
  //          largest(robot->current_state.reward))
  //robot->current_state.distribution[i] = thing / sum;
  //(sum_inputs());

  //printf("distr : %f\n", robot->current_state.distribution[0]);
  free(array);
  //exit(1);
  return ;
}

/*
double  output(t_hidden hidden, double *output, int length, int action)
{
  double  active;
  int     i;

  i = length;
  active = 0.0;
  active += hidden.neuron[]
  while (--i >= 0)
    active += hidden.weight[neuron] * state[i];
  // NOTE : BREAK IN BASE OF EMERGENCY
  active = (active > 0)? 1.0 / (1.0 + pow(E, -1.0 * active)):0;

  //active = (double)(1.000 / (1.0 + pow(E, active * -1.000)));
  //printf("Activation : %f\n", active);
  return (active);
}*/

double normalize(double active, double max)
{
  //printf("inside normalization : %f\n", active / max);
  return (active / max);
}

double  activation(t_hidden hidden, double *state, int length, int neuron)
{
  double  active;
  static double  max;
  int     i;

  //printf("Enter activate first layer\n");
  active = 0.0;
  //max = 0.0;
  i = length;
  active += hidden.neuron[neuron].bias;
  //printf("plus bias %f\n", active);
  while (--i >= 0)
    active += hidden.weight[neuron] * state[i];//, printf("activity : %f\n", active);
  /* NOTE : BREAK IN BASE OF EMERGENCY
  */
  //printf("active? %f\n", active);
  //exit(1);
  //active = (active > 0)? 1.0 / (1.0 + pow(E, -1.0 * active)):0.0;
  active = (active > 0.0)? active : 0.0;
  if (active > max) max = active + 0.00001;
  //max = (active > max)? active : max;
  //printf("Activation before normalization : %f, %f\n", active, max);
  active = normalize(active, max);
  //printf("Activation : %f\n", active);
  //active = (double)(1.000 / (1.0 + pow(E, active * -1.000)));
  //exit(1);
  return (active);
}

/*
double  activate_first_layer_training(t_hidden hidden, double *state)
{
  double  active;
  int     i;
  int     x;
  int     y;

  //printf("Enter activate first layer\n");
  i = NEURONS;
  while (--i >= 0)
  {
    active = hidden.neuron[i].bias;
    y = HEIGHT;
    while (--y >= 0)
    {
      x = WIDTH;
      while (--x >= 0)
      {
        active += hidden.weight[i] * ;
      }
    }
  }
  // NOTE : MIGHT NEED THESE
  //
  return (active = (x > 0)? x:0);
  //printf("Activation training : %f\n", active);
  //active = (double)(1.000 / (1 + pow(E, active * -1.000)));
  //printf("Activation training : %f\n", active);
  return (active);
}
*/
/*
double  activation(t_hidden hidden, double *state, int length)
{
  double  active;
  int     i;
  int     j;

  //printf("Enter activation\n");
  active = 0.0;
  i = NEURONS;
  while (--i >= 0)
  {
    active += hidden.neuron[i].bias;
    j = length;
    while (--j >= 0)
    {
      //printf("%d\n", j);
      active += hidden.weight[i] * state[j];
      //printf("hello\n");
    }
  }
  * NOTE : MIGHT NEED THESE
  /
  active = (double)(1.000 / (1.0 + pow(E, active * -1.000)));
  //printf("right meow\n");
  //active = (active > 0.0)? active:0.0;
  printf("Activation : %f\n", active);
  return (active);
  //return (active);
}
*/

/*double  *stream_cat(double *stream, double *cat, int len1, int len2)
{
  double  *ret;

  ret = (double*)calloc(len1 + len2, sizeof(double));
  while (len1 + len2 >= len1)
  {
    ret[len1 + len2] = cat[len2];
    --len2;
  }
  while (len1 >= 0)
  {
    ret[len1] = stream[len1];
    --len1;
  }
  return (ret);
}*/

double   random_number(double min, double max)
{
  double  diff;
  double  r;

  diff = max - min;
  r = 0.0;
  r = (double)(((double)(diff) / RAND_MAX) * (double)rand() + min);
  //printf("r : %f\n", r);
  return (r);
}

double  sigmoid(double n)
{
  n *= (n > 0.0)? 1.0 : -1.0;
  return (n / (1.0 + (n)));
}
