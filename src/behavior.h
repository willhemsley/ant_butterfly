// Taken from demo for Epidemiology behaviour.h - RandomMovement
// Taken from chemotaxis.h

#ifndef BEHAVIOR_H_
#define BEHAVIOR_H_

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
class Bond_Species : public Behavior {
  BDM_BEHAVIOR_HEADER(Bond_Species, Behavior, 1);

  public:
     Bond_Species() {}
     explicit Bond_Species(double squared_dist) : squared_dist_(squared_dist) {}
     virtual ~Bond_Species() {}

   /*
   *
   * Finding agents of opposing type and bonding if within set distance
   *
   **/
   void Run(Cell* cell, double squared_dist_, double squared_radius) {
     auto* larva = bdm_static_cast<Butterfly*>(cell);
     auto* sim = Simulation::GetActive();
     auto& position = larva->GetPosition(); // get position of agent

     auto* ctxt = sim->GetExecutionContext(); // get context information
     auto check_surrounding =
       L2F([&](Agent* neighbor, double squared_dist) {

         // 1. Check if neighbor is an Ant type
         if (auto* nearby_ant = dynamic_cast<Ant*>(neighbor)) {

           // 2. Update vectors of bonded agents
           // add bonded butterfly to ant bond vector
           vector<Cell*> bonded_bfly_new = nearby_ant->GetBondedButterfly();
           bonded_bfly_new.push_back(larva);
           nearby_ant->SetBondedButterfly(bonded_bfly_new);

           // Get vector of ants bonded to butterfly
           vector<Cell*> bonded_ants_new = larva->GetBondedAnts();
           bonded_ants_new.push_back(nearby_ant);
           larva->SetBondedAnts(bonded_ants_new);

           // 3. Remove movement behaviors when to create 'bond' behavior
           // TO-DO

         }
       });

     ctxt->ForEachNeighbor(check_surrounding, *larva, squared_radius);

   }

   private:
     double squared_dist_ {1};
};

}  // namespace bdm

#endif  // BEHAVIOR_H_
