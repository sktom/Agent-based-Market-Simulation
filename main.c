
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#include "utl.h"
#include "agent.h"
const int N_AGENTS = 1000;
const int N_TRIAL = 100;

Agent * init(int *, char **, int *, int *);

int
main(int argc, char ** argv)
{
  int myid, numprocs;
  Agent * agents = init(&argc, argv, &myid, &numprocs);

  double new_price = 0;
  int n;
  for(n = 0; n < N_TRIAL; ++n)
  {
    int i;
    Agent * agent;
    for(agent = agents, i = N_AGENTS; i--; ++agent)
      agent->refresh(agent);

    double l_min_ask, l_max_bid;
    minmax(&l_min_ask, &l_max_bid, agents, N_AGENTS);

    double g_min_ask, g_max_bid;
    MPI_Reduce(&l_max_bid, &g_max_bid, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&l_min_ask, &g_min_ask, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);

    if(myid == 0)
      if(g_max_bid > g_min_ask)
      {
        new_price = (g_min_ask + g_max_bid) / 2;
        printf("\n%dth trial : new market price = %lf\n", n, new_price);
      }

    MPI_Bcast(&new_price, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if(new_price != 0)
    {
      for(agent = agents, i = N_AGENTS; i--; ++agent)
        agent->set(agent, new_price);

      new_price = 0;
    }
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

