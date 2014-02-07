
#define MPI_MODE
#ifdef MPI_MODE
#include "mpi.h"
#endif

#include <stdio.h>
#include <stdlib.h>

#include "utl.h"
#include "agent.h"

const int N_AGENTS = 75;
const int N_TRIAL = 100;

int
main(int argc, char ** argv)
{
#ifdef MPI_MODE
  MPI_Init(&argc, &argv);
  int myid;
  int numprocs;
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
#endif

  Agent * agents = malloc(sizeof(Agent) * N_AGENTS);
  init_agents(agents, N_AGENTS);

  int n;
  for(n = 0; n < N_TRIAL; ++n)
  {
printf("\nid = %d : %dth trial\n", myid, n);
    int i;
    Agent * agent;
    for(agent = agents, i = N_AGENTS; i--; ++agent)
    {
      agent->refresh(agent);
    }

    double l_min_ask, l_max_bid;
    minmax(&l_min_ask, &l_max_bid, agents, N_AGENTS);

    MPI_Barrier(MPI_COMM_WORLD);
    double g_min_ask, g_max_bid;
#ifdef MPI_MODE
    MPI_Reduce(&l_max_bid, &g_max_bid, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&l_min_ask, &g_min_ask, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
#endif
    MPI_Barrier(MPI_COMM_WORLD);

#ifndef MPI_MODE
    int myid = 0;
#endif
    double new_market_price = 0;
    if(myid == 0)
    {
      putd(g_min_ask);
      putd(g_max_bid);
      if(g_max_bid > g_min_ask)
      {
        new_market_price = (g_min_ask + g_max_bid) / 2;
#ifdef MPI_MODE
        MPI_Bcast(&new_market_price, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
#endif
      }
    }

    barrier(myid);
    MPI_Barrier(MPI_COMM_WORLD);

    if(new_market_price != 0)
    {
printf("\n%dth trial : new market price = %lf\n", n, new_market_price);
      for(agent = agents, i = N_AGENTS; i--; ++agent)
      {
        agent->set(agent, new_market_price);
      }
      new_market_price = 0;
    }
  }

#ifdef MPI_MODE
  MPI_Finalize();
#endif
  return 0;
}

