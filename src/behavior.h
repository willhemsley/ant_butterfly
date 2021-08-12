// Taken from demo for Epidemiology behaviour.h - RandomMovement
// Taken from chemotaxis.h

#ifndef BEHAVIOR_H_
#define BEHAVIOR_H_

#include "ant.h"
#include "butterfly.h"
//#include "physical_bond.h"

namespace bdm {

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
// Movement in Y-axis Only (move from underground to overground)
class Move_Underground : public Behavior {
  BDM_BEHAVIOR_HEADER(Move_Underground, Behavior, 1);

  public:
    Move_Underground() {}
    explicit Move_Underground(double ymove_speed) : ymove_speed_(ymove_speed) {}
    virtual ~Move_Underground() {}

  /*
  *
  * Movement in Y direction (downwards) if above 100 in y-axis
  *
  **/
  void Run(Agent* agent) override {
    auto& position = agent->GetPosition(); // get position of cell (Double3)
    if (position[1] > 100) { // if overground (i.e. y>100 )
      MathArray<double, 3> ymovement = {{0,-1,0}}; // double with only negative y direction
      MathArray<double, 3> new_pos = position + (ymovement * ymove_speed_); // update position
      agent->SetPosition(new_pos);
    }
  }
  private:
    double ymove_speed_ = 1;
};

//------------------------------------------------------------------------------
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
   * When found the behaviors are stripped
   *
   **/
   void Run(Agent* agent) override {
     // set squared distance for search
     double squared_dist = squared_dist_;
     // cast agent to Butterfly class
     auto* larva = bdm_static_cast<Butterfly*>(agent);
     auto* sim = Simulation::GetActive();

     // 1. Find bonded vector of bonded ants to larva
     std::vector<Cell*> bonded_ants{};
     bonded_ants = larva->GetBondedAgents();

     // get context information
     auto* ctxt = sim->GetExecutionContext();

     // Search the surroudning area within squared distance
     auto check_surrounding =
       L2F([&](Agent* neighbor, double squared_dist) {

         // 1. Check if neighbor is an Ant type
         if (auto* nearby_ant = dynamic_cast<Ant*>(neighbor)) {

           // 2. Remove all behaviors if nearby to create 'bond' behavior
           // adapted from forum
           // Remove larvae behavior
           // const auto& larva_behaviors = larva->GetAllBehaviors();
           // for (int i = 0; i < larva_behaviors.size(); i++) {
           //        larva->RemoveBehavior(larva_behaviors[i]);
           // }
           // Remove nearby ant agent behaviors
           const auto& ant_behaviors = nearby_ant->GetAllBehaviors();
           for (int i = 0; i < ant_behaviors.size(); i++) {
             nearby_ant->RemoveBehavior(ant_behaviors[i]);
           }

           // 4. Update vectors of bonded agents -- were getting overwritten
           // add bonded butterfly to ant bond vector
           std::vector<Cell*> bonded_bfly{};
           bonded_bfly = nearby_ant->GetBondedAgents();
           bonded_bfly.push_back(larva);
           nearby_ant->SetBondedAgents(bonded_bfly);

           bonded_bfly = nearby_ant->GetBondedAgents(); // rewrite

           // 3. Add collective movement underground
           // make into a behavior?
           // needs work with positional movement
           auto& pos_ant = nearby_ant->GetPosition(); // get position of cell
           auto& pos_larva = larva->GetPosition(); // get position of cell

           if ((pos_ant[1] > 150) && (pos_larva[1] > 150)) { // if overground (i.e. y>150 )
             Double3 ymovement = {{0,100,0}};
             auto new_pos_ant = pos_ant - ymovement; // update position
             auto new_pos_larva = pos_larva - ymovement; // update position
             larva->SetPosition(new_pos_larva);
             nearby_ant->SetPosition(new_pos_ant);
           }

           // Add new bonded ants to bond list for butterfly
           bonded_ants.push_back(nearby_ant);
           larva->SetBondedAgents(bonded_ants); //update the list
       }
     });

       ctxt->ForEachNeighbor(check_surrounding, *larva, squared_dist);
   }

   private:
     double squared_dist_ {1};
};

}  // namespace bdm

#endif  // BEHAVIOR_H_
