
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#include "utl.h"
#include "agent.h"
const int N_AGENTS = 250000;
const int N_TRIAL = 10000;

Agent * init(int *, char **, int *, int *);
void get_extreme_value(double *, double *, Agent *);

int
main(int argc, char ** argv)
{
  int myid, numprocs;
  Agent * agents = init(&argc, argv, &myid, &numprocs);

  double new_price = 0;
  int t;
  for(t = 0; t < N_TRIAL; ++t)
  {
    int i;
    Agent * agent;
    for(agent = agents, i = N_AGENTS; i--; ++agent)
      agent->refresh(agent);

    double g_min_ask, g_max_bid;
    get_extreme_value(&g_min_ask, &g_max_bid, agents);

    if(myid == 0)
      if(g_max_bid > g_min_ask)
      {
        new_price = (g_min_ask + g_max_bid) / 2;
        //printf("%d %lf\n", t, new_price);
      }

    MPI_Bcast(&new_price, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for(;new_price; new_price = 0)
      for(agent = agents, i = N_AGENTS; i--; ++agent)
        agent->set(agent, new_price);
  }

  MPI_Finalize();
  return 0;
}

Agent *
init(int * argc, char ** argv, int * myid, int * numprocs)
{
  MPI_Init(argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, myid);

  Agent * agents = malloc(sizeof(Agent) * N_AGENTS);
  init_agents(agents, N_AGENTS);
  return agents;
}

void
get_extreme_value(double * g_min_ask, double * g_max_bid, Agent * agents)
{
  double l_min_ask, l_max_bid;
  minmax(&l_min_ask, &l_max_bid, agents, N_AGENTS);
  MPI_Reduce(&l_max_bid, g_max_bid, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
  MPI_Reduce(&l_min_ask, g_min_ask, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
}

