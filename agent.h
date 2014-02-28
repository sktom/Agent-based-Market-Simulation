
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

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
  double dp = normal_rand() / 2000 * agent->ask;
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
init_agents(Agent * agents, int number_agents)
{
  Agent * agent = agents;
  for(; number_agents--; ++agent)
  {
    init_agent(agent);
    agent->refresh(agent);
  }
}

double * asks = NULL;
double * bids = NULL;
void
minmax(double * min_ask, double * max_bid, Agent * agents, int number_agents)
{
  if(!asks)
  {
    bids = malloc(sizeof(double) * number_agents);
    asks = malloc(sizeof(double) * number_agents);
  }

  double * ask;
  double * bid;
  Agent * agent = agents;
  int i;
  for(ask = asks, bid = bids, i = number_agents; i--; ++agent, ++ask, ++bid)
  {
    *ask = agent->ask;
    *bid = agent->bid;
  }

  *min_ask = min(asks, number_agents);
  *max_bid = max(bids, number_agents);
}

