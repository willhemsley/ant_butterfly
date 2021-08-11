// Taken from demo for Epidemiology behaviour.h - RandomMovement
// Taken from chemotaxis.h

#ifndef BEHAVIOR_H_
#define BEHAVIOR_H_

#include "ant.h"
#include "butterfly.h"
//#include "physical_bond.h"

namespace bdm {

// -----------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------
// Movement in Y-axis Only (move underground)
class Move_Underground : public Behavior {
  BDM_BEHAVIOR_HEADER(Move_Underground, Behavior, 1);

  public:
    Move_Underground() {}
    explicit Move_Underground(double ymove_speed) : ymove_speed_(ymove_speed) {}
    virtual ~Move_Underground() {}

  void Run(Agent* agent) override {
    // auto* sim = Simulation::GetActive();

    auto& position = agent->GetPosition(); // get position of agent
    // if overground (i.e. y>150 ) and if has a bond
    if (position[1] > 100) {
      Double3 ymovement = {{0,10,0}};
      auto new_pos = position - (ymovement * ymove_speed_); // update position
      agent->SetPosition(new_pos);
    }
    else {
      // already underground
    }
  }
  private:
    double ymove_speed_ = 1;
};

// -----------------------------------------------------------------------------
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
     double squared_dist = squared_dist_;
     auto* larva = bdm_static_cast<Butterfly*>(agent);
     auto* sim = Simulation::GetActive();

     auto* ctxt = sim->GetExecutionContext(); // get context information

     // find vector of bonds (ants bonded to butterfly)
     std::vector<Cell*> bonded_ants_new{};
     bonded_ants_new = larva->GetBondedAgents();

     auto check_surrounding =
       L2F([&](Agent* neighbor, double squared_dist) {

         // 1. Check if neighbor is an Ant type
         if (auto* nearby_ant = dynamic_cast<Ant*>(neighbor)) {

           // 2. Remove all behaviors when to create 'bond' behavior
           // adapted from forum (https://forum.biodynamo.org/t/agent-removal-cell-death/35/4)

           const auto& ant_behaviors = nearby_ant->GetAllBehaviors();
           for (int i = 0; i < ant_behaviors.size(); i++) {
             nearby_ant->RemoveBehavior(ant_behaviors[i]);
           }

           // 3. Update vectors of bonded agents
           // add bonded butterfly to ant bond vector
           // find vector of bonds (butterfly bonded to ant)
           std::vector<Cell*> bonded_bfly_new{};
           bonded_bfly_new = nearby_ant->GetBondedAgents();
           bonded_bfly_new.push_back(larva);
           nearby_ant->SetBondedAgents(bonded_bfly_new);
           // Get vector of ants bonded to butterfly

           bonded_ants_new.push_back(nearby_ant);
           larva->SetBondedAgents(bonded_ants_new);

           // 4. Add collective movement underground
           // // needs work with positional movement
           // auto& pos_ant = nearby_ant->GetPosition(); // get position of cell
           // auto& pos_larva = larva->GetPosition(); // get position of cell
           //
           // if ((pos_ant[1] > 20) && (pos_larva[1] > 10)) { // if overground (i.e. y>150 )
           //   // make this into a behavior?
           //   Double3 ymovement = {{0,20,0}};
           //   auto new_pos_ant = pos_ant - ymovement; // update position
           //   auto new_pos_larva = pos_larva - ymovement; // update position
           //   nearby_ant->SetPosition(pos_ant - ymovement);
           //   larva->SetPosition(pos_ant - ymovement);
           // }
           // 4 (ADAPTED). add movement underground behavior
           std::vector<Cell*> bonded{};
           bonded = larva->GetBondedAgents();
           if (bonded.size()>3) { // if at least 3 bonds
             const auto& larva_behaviors = larva->GetAllBehaviors();
             for (int i = 0; i < larva_behaviors.size(); i++) {
               larva->RemoveBehavior(larva_behaviors[i]);
             }
             larva->AddBehavior(new Move_Underground(10));
             nearby_ant->AddBehavior(new Move_Underground(10));

           }
         }

       });

       ctxt->ForEachNeighbor(check_surrounding, *larva, squared_dist);
   }

   private:
     double squared_dist_ = 1.0;
};

}  // namespace bdm

#endif  // BEHAVIOR_H_
