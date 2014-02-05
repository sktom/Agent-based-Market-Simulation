
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
  double dp = (rnd() * 2 - 1) / 100 + 1;
  agent->bid *= dp;
  agent->ask *= dp;
}

void
set_price(Agent * agent, double price)
{
  agent->ask = price + agent->spread / 2;
  agent->bid = price - agent->spread / 2;
  agent->refresh(agent);
}

void
init_agent(Agent * agent)
{
  agent->ask = 100;
  agent->spread = agent->ask / 10;
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

