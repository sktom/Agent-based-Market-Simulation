
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

struct Agent
{
  double mid;
  double spread_rate;
  double bid;
  double ask;
  void (*refresh)();
  void (*set)();
};
typedef struct Agent Agent;

void
set_price(Agent * agent, double price)
{
  agent->mid = price;
  double spread_rate = agent->spread_rate;
  double exp_dif = (spread_rate + pow(pow(spread_rate, 2) + 4, 0.5)) / 2;
  agent->ask = price * exp_dif;
  agent->bid = price / exp_dif;
}

void
refresh_price(Agent * agent)
{
  double dif_rate = 1 + normal_rand() / 100;
  set_price(agent, agent->mid * pow(dif_rate, (long)(genrand_int32() % 2) * 2 - 1));
}

void
init_agent(Agent * agent)
{
  agent->spread_rate = 0.1;
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

