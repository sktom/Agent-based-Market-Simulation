
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

struct Agent
{
  double mid;
  double spread;
  double bid;
  double ask;
  void (*refresh)();
  void (*set)();
};
typedef struct Agent Agent;

void
set_price(Agent * agent, double price)
{
  double spread = price / 100;
  agent->spread = spread;
  agent->mid = price;
  agent->ask = price + spread / 2;
  agent->bid = price - spread / 2;
}

void
refresh_price(Agent * agent)
{
  double dp = normal_rand() / 2000 * agent->mid;
  set_price(agent, agent->mid + dp);
}

void
init_agent(Agent * agent)
{
  set_price(agent, 100);
  agent->refresh = refresh_price;
  agent->set = set_price;
}

void
init_agents(Agent * agents, ulong number_agents)
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
minmax(double * min_ask, double * max_bid, Agent * agents, ulong number_agents)
{
  if(!asks)
  {
    bids = malloc(sizeof(double) * number_agents);
    asks = malloc(sizeof(double) * number_agents);
  }

  double * ask;
  double * bid;
  Agent * agent = agents;
  ulong i;
  for(ask = asks, bid = bids, i = number_agents; i--; ++agent, ++ask, ++bid)
  {
    *ask = agent->ask;
    *bid = agent->bid;
  }

  *min_ask = min(asks, number_agents);
  *max_bid = max(bids, number_agents);
}

