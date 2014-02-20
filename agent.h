
#include <stdio.h>
#include <stdlib.h>
#include "rand.h"

#include "utl.h"

struct Agent
{
  double bid;
  double ask;
  double spread;
  void (*refresh)();
  void (*set)();
};
typedef struct Agent Agent;

void
refresh_price(Agent * agent)
{
  double dp = (rnd() * 2 - 1) / 2000 * agent->ask;
  agent->ask += dp;
  agent->bid += dp;
}

void
set_price(Agent * agent, double price)
{
  agent->ask = price + agent->spread / 2;
  agent->bid = price - agent->spread / 2;
}

void
init_agent(Agent * agent)
{
  agent->ask = 100;
  agent->spread = agent->ask / 100;
  agent->bid = agent->ask - agent->spread;
  agent->refresh = refresh_price;
  agent->set = set_price;
}

void
init_agents(Agent * agents, int num_agents)
{
  Agent * agent = agents;
  for(; num_agents--; ++agent)
  {
    init_agent(agent);
    agent->refresh(agent);
  }
}

void
minmax(double * min_ask, double * max_bid, Agent * agents, int num_agents)
{
  double * bids;
  double * asks;
  bids = malloc(sizeof(double) * num_agents);
  asks = malloc(sizeof(double) * num_agents);

  double * bid;
  double * ask;
  Agent * agent = agents;
  int i;
  for(bid = bids, ask = asks, i = num_agents; i--; ++agent, ++bid, ++ask)
  {
    *bid = agent->bid;
    *ask = agent->ask;
  }

  *min_ask = min(asks, num_agents);
  *max_bid = max(bids, num_agents);
}

