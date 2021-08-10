// Taken from demo for Epidemiology behaviour.h - RandomMovement
// Taken from chemotaxis.h

#ifndef BEHAVIOR_H_
#define BEHAVIOR_H_

#include "core/behavior/behavior.h"
#include "ant.h"
#include "butterfly.h"
//#include "physical_bond.h"

namespace bdm {

// Random Movement
// https://github.com/BioDynaMo/biodynamo/blob/master/demo/epidemiology/src/behavior.h
class RandomMovement : public Behavior {
  BDM_BEHAVIOR_HEADER(RandomMovement, Behavior, 1);

  public:
   RandomMovement() {}
   explicit RandomMovement(double rm_speed) : rm_speed_(rm_speed) {}
   virtual ~RandomMovement() {}

  void Run(Agent* agent) override {
    auto* sim = Simulation::GetActive();
    auto* random = sim->GetRandom();

    auto& position = agent->GetPosition();
    auto rand_movement = random->UniformArray<3>(-1, 1).Normalize();
    auto new_pos = position + (rand_movement * rm_speed_); // update position
    agent->SetPosition(new_pos);
  }
  private:
    double rm_speed_;
};

// Movement in Y-axis Only (move from underground to overground)
class YMovement_Und : public Behavior {
  BDM_BEHAVIOR_HEADER(YMovement_Und, Behavior, 1);

  public:
    YMovement_Und() {}
    explicit YMovement_Und(double ymove_speed) : ymove_speed_(ymove_speed) {}
    virtual ~YMovement_Und() {}

  void Run(Agent* agent) override {
    auto* sim = Simulation::GetActive();
    auto* random = sim->GetRandom();

    auto& position = agent->GetPosition(); // get position of cell
    if (position[1] > 150) { // if overground (i.e. y>150 )
      auto rand_ymovement = random->UniformArray<3>(0, 1).Normalize();
      rand_ymovement.at(0) = 0;
      rand_ymovement.at(2) = 0;
      auto new_pos = position - (rand_ymovement * ymove_speed_); // update position
      agent->SetPosition(new_pos);
    }
  }
  private:
    double ymove_speed_;
};

// Bonding with Nearby Agents
class BondwithNearbyAgents : public Behavior {
   BDM_BEHAVIOR_HEADER(BondwithNearbyAgents, Behavior, 1);

   public:
     BondwithNearbyAgents () {}
     explicit BondwithNearbyAgents(double squared_dist) : squared_dist_(squared_dist) {}
     virtual ~BondwithNearbyAgents() {}

   void FindNearby(Agent* agent, double squared_dist_) override {
     auto* sim = Simulation::GetActive();
     auto& position = agent->GetPosition(); // get position of agent

     auto* ctxt = sim->GetExecutionContext(); // get context information
     auto check_surrounding =
       L2F([&](Agent* neighbor, squared_dist) {
         if (neighbor->GetAgentType() != agent->GetAgentType()) { // if others are opposing type
           PhysicalBond(agent_a, nearby) // Apply the Physical Bond between agents
         }
       });

     ctxt->ForEachNeighbor(check_surrounding, *agent, squared_dist);
   }

   private:
     double sq_dist_;
 };

}  // namespace bdm

#endif  // BEHAVIOR_H_
