
/*
 * This program require 2 arguments to run.
 *   First agrgument stands for total number of agents, and
 *   second agrument stands for a number of trials.
 */
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include "main.h"

Agent * init(int *, char **, int *, int *, int *, int *, int *);
void get_extreme_value(double *, double *, Agent *, int, int);

int
main(int argc, char ** argv)
{
  INIT_AGENTS;
  if(myid == 0)
    printf("%d %d %ld\n", number_all_agents, number_local_agents, number_trials);
  double new_price = 0;
  unsigned long t;
  for(t = 0; t < number_trials; ++t)  {
    int i; 
    Agent * agent;
    
    for(agent = agents, i = number_local_agents; i--; ++agent)
      agent->refresh(agent);

    double min_whole_ask, max_whole_bid;
    get_extreme_value(&min_whole_ask, &max_whole_bid, agents, number_local_agents, t);

    if(myid == 0)
      if(max_whole_bid > min_whole_ask)
      {
        new_price = (min_whole_ask + max_whole_bid) / 2;
        printf("%ld %lf\n", t, new_price);
      }

    MPI_Bcast(&new_price, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for(; new_price; new_price = 0)
      for(agent = agents, i = number_local_agents; i--; ++agent)
        agent->set(agent, new_price);
  }

  MPI_Finalize();
  return 0;
}

Agent *
init(int * argc, char ** argv,
    int * number_total_agents, int * number_local_agents, int * number_trials,
    int * myid, int * numprocs)
{
  init_rand();
  MPI_Init(argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, myid);
  
  *number_total_agents = atoi(argv[1]);
  *number_local_agents = *number_total_agents / *numprocs;
  *number_trials = atoi(argv[2]);

  Agent * agents = malloc(sizeof(Agent) * *number_local_agents);
  init_agents(agents, *number_local_agents);
  return agents;
}

void
get_extreme_value(double * min_whole_ask, double * max_whole_bid,
    Agent * agents, int number_local_agents, int t)
{
  double min_local_ask, max_local_bid;
  minmax(&min_local_ask, &max_local_bid, agents, number_local_agents);
  MPI_Reduce(&max_local_bid, max_whole_bid, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
  MPI_Reduce(&min_local_ask, min_whole_ask, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
}

