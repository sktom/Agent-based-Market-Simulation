
#define MPI_MODE
#ifdef MPI_MODE
#include "mpi.h"
#endif

#include <stdio.h>
#include <stdlib.h>

#include "utl.h"
#include "agent.h"

const int N_AGENTS = 25;
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
  for(n = N_TRIAL; n--;)
  {
    int i;
    Agent * agent;
    for(agent = agents, i = N_AGENTS; i--; ++agent)
    {
      agent->refresh(agent);
    }

    double * bids;
    double * asks;
    bids = (double *)malloc(sizeof(double) * N_AGENTS);
    asks = (double *)malloc(sizeof(double) * N_AGENTS);

    double * bid;
    double * ask;
    for(agent = agents, bid = bids, ask = asks, i = N_AGENTS; i--; ++agent, ++bid, ++ask)
    {
      *bid = agent->bid;
      *ask = agent->ask;
    }

    double l_min, l_max;
    l_max = max(bids, N_AGENTS);
    l_min = min(asks, N_AGENTS);

    double g_min, g_max;
#ifdef MPI_MODE
    MPI_Reduce(&l_max, &g_max, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&l_min, &g_min, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);

#endif

#ifndef MPI_MODE
    int myid = 0;
#endif
    double new_market_price = 0;
    if(!myid)
    {
      if(g_max > g_min)
      {
        new_market_price = (g_max + g_min) / 2;
#ifdef MPI_MODE
        MPI_Bcast(&new_market_price, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
#endif
      }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if((int)new_market_price)
    {
puti(N_TRIAL - n);
putd(new_market_price);
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

