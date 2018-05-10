/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scollet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/03 15:55:09 by scollet           #+#    #+#             */
/*   Updated: 2017/07/03 15:55:10 by scollet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CD_H
# define CD_H

# include <stdlib.h>
# include <time.h>
# include <stdio.h>
# include <math.h>
# include <fcntl.h>
# include <unistd.h>
# include <string.h>
# include <ctype.h>
# include <float.h>

# define ACTIONS        5
# define BATCH_SIZE     256
# define DEQUE          1024
# define DONE           1
# define E              0.577
# define EMPTY          0
# define EPISODES       10000
# define EPSILON        0.999
# define FAIL           0
# define GAMMA          0.95
# define HEIGHT         64
# define ID             1
# define LAMBDA         0.94
# define LAYERS         3
# define LEARNING_RATE  0.02
# define MACRO          4
# define MICRO          321
# define NEURONS        1024
# define NUM_ACTIONS    5
# define NUM_AGENTS     (HEIGHT * WIDTH) / 10
# define POOL           256
# define SUCCESS        1
# define THRESHOLD      110
# define UNIVERSE       2
# define WIDTH          64

typedef struct s_vector {
  int   x;
  int   y;
}              t_vector;

typedef struct s_grid {
  int   grid[HEIGHT][WIDTH];
}              t_grid;

typedef struct s_agent {
  t_vector goal;
  t_vector start;
  int      id;
}              t_agent;

typedef struct s_neuron {
  double  delta;
  double  bias;
}              t_neuron;

typedef struct s_hidden {
  t_neuron  *neuron;
  double    *weight;
  double    *output;
}              t_hidden;

/*
typedef struct s_pool {
  double  pool[POOL];
}              t_pool;
*/

typedef struct  s_network {
  t_hidden *hidden;
  //t_pool *pooling;
  //double output_layer[NEURONS];
}               t_network;

typedef struct s_matrix {
  double      *macro_state;
  double      *micro_state;
  t_network   neural_network;
  double      output[ACTIONS];
}              t_matrix;

typedef struct s_state {
  t_vector  location;
  double    macro_state[MACRO];
  double    micro_state[MICRO];
  double    distribution[ACTIONS];
  double    reward[ACTIONS];
  int       action;
  int       prediction;
  int       optimal;
}              t_state;

typedef struct s_robot {
  t_vector  goal;
  t_vector  start;
  t_vector  location;
  t_matrix  matrix;
  t_state   current_state;
  t_state   *memory;
  double    distance;
  int       action[2][5];
  double    state_evalulation;
  int       id;
  double    epsilon;
  double    lambda;
  double    gamma;
  int       done;
  int       score;
  int       convergence_rate;
}              t_robot;


typedef struct s_domain {
  t_agent   *agents;
  t_robot   robot;
  t_grid    *world;
}              t_domain;

void   action(t_domain *domain, int action);
double  activation(t_hidden hidden, double *state, int length, int neuron);
double  activate_first_layer(t_hidden hidden, double *state, int length, int neuron);
t_agent   agent(t_agent agent);
int  check(t_domain *domain, int action);
void  compute_distribution(t_robot *robot);
void  compute_hidden(t_matrix matrix, double *input, int size, int layer);
double  *copy_stream(double *stream, int size);
double  derivative(double output);
void  display_output(t_network *network);
void  display_stats(t_robot *robot, int t);
void  display_weights(t_network *network);
void  display_world(t_grid *world);
void  downsample(t_matrix matrix, int layer, int stride);
void  enter_the_network(t_domain *domain, int start);
double   euclidean(t_robot *robot, t_vector *location);
void  feed_forward(t_matrix *matrix, int layer);
void  feed_inputs(t_robot *robot, int layer);
int   get_state(t_domain *domain);
t_domain  init(t_domain domain, int running);
void   learn(t_domain *domain, int t);
void    matrices(t_matrix *matrix, int layers, int length);
int  move(t_domain *domain, int counter);
double normalize(double active, double max);
int   optimal(t_robot *robot);
int   prediction(double *distribution);
double   random_number(double min, double max);
void   remember(t_state *memory, t_robot *robot);
void  run(t_domain domain);
double  sigmoid(double n);
void  soft_max(t_robot *robot);
t_domain    spawn(t_domain domain);
double  *stream_cat(double *stream, double *cat, int len1, int len2);
double  sum_inputs(double *state, int length);
double   sum_over(t_hidden hidden, int len);
void  update_weights(t_matrix *matrix, int length);

#endif
