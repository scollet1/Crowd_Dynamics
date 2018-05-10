/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scollet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/09 13:31:35 by scollet           #+#    #+#             */
/*   Updated: 2017/08/09 13:31:36 by scollet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cd.h"

void   remember(t_state *memory, t_robot *robot)
{
  int     j;
  int     i;
  int     d;

  //printf("%d\n", t);
  i = -1;
  j = -1;
  d = 0;
  memory->location.x = robot->location.x;
  memory->location.y = robot->location.y;
  for (d = 0; d < ACTIONS; d++){memory->distribution[d] =
                                robot->current_state.distribution[d];}
  memory->action = robot->current_state.action;
  memory->reward[memory->action] =
  robot->current_state.reward[memory->action];
  //printf("right meow\n");
  while (++i < MACRO)
  {
    //printf("i : %d\n", i);
    //printf("this and this before : %f\n"/*, memory->matrix.macro_state[i]*/, robot->matrix.macro_state[i]);
    memory->macro_state[i] = robot->matrix.macro_state[i];
    //printf("this and this after : %f, %f\n", memory->matrix.macro_state[i], robot->matrix.macro_state[i]);
  }
  //printf("First loop\n");
  //printf("\n\n");
  while (++j < MICRO)
  {
    //printf("j : %d\n", j);
    //printf("this and that before : %f\n"/*, memory->matrix.micro_state[j]*/, robot->matrix.micro_state[j]);
    memory->micro_state[j] = robot->matrix.micro_state[j];
    //printf("this and that after : %f, %f\n", memory->matrix.micro_state[j], robot->matrix.micro_state[j]);
  }
  /*i = -1;
  j = -1;
  while (++i < MACRO)
     = max[i];
  printf("After first loop\n");
  while (++j < MICRO)
     = mini[j];*/
  //printf("exit\n");
  //exit(1);
  return ;
}

void  backpropogate(t_matrix *matrix, double loss)
{
  double  error;
  double  errors[NEURONS];
  int     layer;
  int     neuron;

  layer = LAYERS;
  while (--layer >= 0)
  {
    neuron = NEURONS;
    if (layer != LAYERS - 1)
    {
      error = 0.0;
      while (--neuron >= 0)
      {
        error = (matrix->neural_network.hidden[layer].weight[neuron] *
                  matrix->neural_network.hidden[layer].neuron[neuron].delta);
        errors[neuron] += error + 0.0001;
      }
    }
    else
    {
      while (--neuron >= 0)
      {
        errors[neuron] = loss - matrix->neural_network.hidden[layer].output[neuron];
        //printf("err : %f\n", errors[neuron]);
      }
    }
    neuron = NEURONS;
    while (--neuron >= 0)
    {
      matrix->neural_network.hidden[layer].neuron[neuron].delta =
      errors[neuron] * derivative(matrix->neural_network.hidden[layer].output[neuron]);
      //printf("derivative : %f\n", error);
    }
  }
  //exit(1);
  return ;
}

/*void  downsample(t_matrix matrix, int layer, int stride)
{
  int   i;
  int   j;
  int   q;

  i = -1;
  while (++i < POOL)
  {
    j = -1;
    while (j < NEURONS)
    {
      q = -1;
      while (++q < 4)
      {
        matrix.pooling[layer][i] += (random_number(0, 1) <=
        activation(matrix.neural_network[layer][j + q]))?
        matrix.neural_network[layer][j + q] : 0;
      }
      matrix.pooling[layer][i] /= 4;
      j += q;
    }
  }
}*/

void  train_forward(t_matrix *matrix, int layer)
{
  int     neuron;
  int     action;
  double  active;

  neuron = NEURONS;
  action = 0;
  active = 0.0;
  //printf("layer : %d\n", layer);
  while (--neuron >= 0)
  {
    //printf("neuron : %d\n", neuron);
    //matrix->neural_network.hidden[layer].output[neuron] =
    //activation(matrix->neural_network.hidden[layer],
    //printf("weight and neuron : %f, %d\n" ,matrix->neural_network.hidden[layer].weight[neuron], neuron);
    if (layer < LAYERS - 1)
      matrix->neural_network.hidden[layer].output[neuron] =
      (random_number(0.0, 1.00) <= (active = activation(matrix->neural_network.hidden[layer],
      matrix->neural_network.hidden[layer - 1].weight, NEURONS, neuron)))?
      matrix->neural_network.hidden[layer - 1].output[neuron] : 0.0;
    /*else if (layer == LAYERS - 1)
      matrix->neural_network.hidden[layer].output[neuron] =
      sigmoid(matrix->neural_network.hidden[layer - 1].weight[neuron]);*/
    else if (layer == LAYERS - 1)
    {
      matrix->neural_network.hidden[layer].output[neuron] =
      (random_number(0.0, 1.00) <= (active = activation(matrix->neural_network.hidden[layer],
      matrix->neural_network.hidden[layer - 1].output, NEURONS, neuron)))?
      matrix->neural_network.hidden[layer - 1].output[neuron] : 0.0;
      //printf("out put : %f\n", matrix->neural_network.hidden[layer].output[neuron]);
      for (action = ACTIONS - 1; action >= 0; action--)
      {
        matrix->output[action] = (random_number(0.0, 1.00) <= (active = activation(matrix->neural_network.hidden[LAYERS - 1],
        matrix->neural_network.hidden[LAYERS - 1].output, NEURONS, neuron)))?
        matrix->neural_network.hidden[layer - 1].output[neuron] : 0.0;
        //printf("moutput action %f, %d\n", matrix->output[action], action);
      }
    }
  }
  //printf("Leaving compute_hidden\n");
  return ;
}

void  train_inputs(t_robot *robot, int layer, int snapshot)
{
  int     neuron;
  double  macro_active;
  double  micro_active;

  neuron = NEURONS;
  macro_active = 0.0;
  micro_active = 0.0;
  //printf("layer : %d\n", layer);
  while (--neuron >= 0)
  {
    macro_active = activation(robot->matrix.neural_network.hidden[layer],
    robot->memory[snapshot].macro_state, MACRO, neuron);
    micro_active = activation(robot->matrix.neural_network.hidden[layer],
    robot->memory[snapshot].micro_state, MICRO, neuron);

    macro_active = (random_number(0.0, 1.00) <= macro_active)? macro_active : 0.0;
    micro_active = (random_number(0.0, 1.00) <= micro_active)? micro_active : 0.0;
    robot->matrix.neural_network.hidden[layer].output[neuron] = (macro_active + micro_active) * 2;
    //printf("neuron, output : %d, %f\n", neuron, robot->matrix.neural_network.hidden[layer].output[neuron]);
  }
  //printf("Leaving compute_hidden\n");
  return ;
}

void  feed_forward(t_matrix *matrix, int layer)
{
  int     neuron;
  int     action;
  double  active;
  double  out;
  int     max;

  neuron = NEURONS;
  active = 0.0;
  out = 0.0;
  max = 0.0;
  //printf("layer : %d\n", layer);
  while (--neuron >= 0)
  {
    //printf("neuron : %d\n", neuron);
    if (layer < LAYERS - 1)
      matrix->neural_network.hidden[layer].output[neuron] =
      (random_number(0.0, 1.00) <= (active = activation(matrix->neural_network.hidden[layer],
      matrix->neural_network.hidden[layer - 1].output, NEURONS, neuron)))?
      matrix->neural_network.hidden[layer - 1].output[neuron] : 0.0;
      /*matrix->neural_network.hidden[layer + 1].neuron[neuron].output =
      (random_number(0.0, 1.00) <= activation(matrix->neural_network.hidden[layer],
      matrix->neural_network.hidden[layer].weight, NEURONS)?
    );*/
    else if (layer == LAYERS - 1)
    {
      matrix->neural_network.hidden[layer].output[neuron] =
      (random_number(0.0, 1.00) <= (active = activation(matrix->neural_network.hidden[layer],
      matrix->neural_network.hidden[layer - 1].output, NEURONS, neuron)))?
      matrix->neural_network.hidden[layer - 1].output[neuron] : 0.0;
      for (action = ACTIONS - 1; action >= 0; action--)
      {
        //max = (matrix->output[action] > max)? matrix->output[action] + 1.00 : max;
        matrix->output[action] += (random_number(0.0, 1.00) <=
        matrix->neural_network.hidden[layer].output[neuron])?
        matrix->neural_network.hidden[layer].output[neuron] : 0.0;
        //printf("moutput action %f, %d\n", matrix->output[action], action);
      }
      //printf("output : %f\n", matrix->neural_network.hidden[layer].output[neuron]);
      //for (action = 0; action < ACTIONS; action++)
        //matrix->output[action] = normalize(matrix->output[action], matrix->output[prediction(matrix->output)] + 1.00);
    }
    //printf("this : %f\n", matrix->neural_network.hidden[layer].output[neuron]);
    //printf("you know what this is take II : %f\n", matrix->neural_network.hidden[layer].output[neuron]);
  }
  //printf("Leaving compute_hidden\n");
  //exit(1);
  return ;
}

void  feed_inputs(t_robot *robot, int layer)
{
  int     neuron;
  double  input;
  double  macro_active;
  double  micro_active;
  int     macro;
  int     micro;

  neuron = NEURONS;
  input = 0.0;
  macro_active = 0.0;
  micro_active = 0.0;
  macro = MACRO;
  micro = MICRO;
  while (--neuron >= 0)
  {
    if (--macro >= 0)
      macro_active = robot->matrix.macro_state[macro];
      /*(random_number(0.0, 1.00) <= activation(robot->matrix.neural_network.hidden[layer],
      robot->matrix.macro_state, MACRO, macro))?
      robot->matrix.macro_state[macro] : 0.0;*/
    else
      macro = MACRO;
    if (--micro >= 0)
      micro_active = robot->matrix.micro_state[micro];
      /*(random_number(0.0, 1.00) <= activation(robot->matrix.neural_network.hidden[layer],
      robot->matrix.micro_state, MICRO, micro))?
      robot->matrix.micro_state[micro] : 0.0;*/
    else
      micro = MICRO;
    //macro_active = (random_number(0.0, 1.00) <= macro_active)? robot->matrix.macro_state[neuron] : 0.0;
    //micro_active = (random_number(0.0, 1.00) <= micro_active)? robot->matrix.macro_state[neuron] : 0.0;
    robot->matrix.neural_network.hidden[layer].output[neuron] = (macro_active + micro_active) / 2;// / 4;
    //printf("you know what this is : %f\n", robot->matrix.neural_network.hidden[layer].output[neuron]);
  }
}

/*
void  compute_distribution(t_robot *robot)
{
  //int   i;

  //i = ACTIONS;
  //while (--i >= 0)
  soft_max(robot);
    //printf("distr : %f\n", robot->current_state.distribution[i] / 5);
  //exit(1);
  //printf("Leaving compute_distribution\n");
  return ;
}
*/

void  enter_the_network(t_domain *domain, int start)
{
  int   layer;
  int   end;

  end = start + BATCH_SIZE;
  while (start < end)
  {
    layer = 0;
    //printf("Enter the Void with layer : %d\n", layer);
    train_inputs(&domain->robot, layer, start);
    while (++layer < LAYERS)
      train_forward(&domain->robot.matrix, layer);
    //printf("Before backprop\n");
    //exit(1);
    backpropogate(&domain->robot.matrix, domain->robot.memory[start].reward[domain->robot.memory[start].action]);
    //printf("After backprop\n");
    update_weights(&domain->robot.matrix, NEURONS);
    //printf("start to end : %d, %d\n", start, end);
    ++start;
    //exit(1);
  }
  //printf("Leave the void\n");
  //exit(1);
  return ;
}

void  learn(t_domain *domain, int t)
{
  int   start;

  printf("Starting to Learn\n");
  t = 0;
  start = (int)random_number(0, DEQUE - BATCH_SIZE);
  enter_the_network(domain, start);
  if (domain->robot.epsilon > 0.01)
    domain->robot.epsilon *= domain->robot.lambda;
  printf("Did we learn?\n");
  //exit(0);
  return ;
}
