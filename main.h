
#include "util.h"
#include "agent.h"

#define INIT_AGENTS \
  ulong number_all_agents, number_local_agents, number_trials; \
  int myid, numprocs; \
  Agent * agents = init(&argc, argv, \
      &number_all_agents, &number_local_agents, &number_trials, \
      &myid, &numprocs)

