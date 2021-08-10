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
// class YMovement_Und : public Behavior {
//   BDM_BEHAVIOR_HEADER(YMovement_Und, Behavior, 1);
//
//   public:
//     YMovement_Und() {}
//     explicit YMovement_Und(double ymove_speed) : ymove_speed_(ymove_speed) {}
//     virtual ~YMovement_Und() {}
//
//   void Run(Agent* agent) override {
//     auto* sim = Simulation::GetActive();
//
//     auto& position = agent->GetPosition(); // get position of cell
//     if (position[1] > 150) { // if overground (i.e. y>150 )
//       Double3 ymovement = {{0,1,0}};
//       auto new_pos = position - (ymovement * ymove_speed_); // update position
//       agent->SetPosition(new_pos);
//     }
//   }
//   private:
//     double ymove_speed_;
// };

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
     auto check_surrounding =
       L2F([&](Agent* neighbor, double squared_dist) {

         // 1. Check if neighbor is an Ant type
         if (auto* nearby_ant = dynamic_cast<Ant*>(neighbor)) {

           // 2. Remove all behaviors when to create 'bond' behavior
           // adapted from forum
           const auto& larva_behaviors = larva->GetAllBehaviors();
           for (int i = 0; i < larva_behaviors.size(); i++) {
                  larva->RemoveBehavior(larva_behaviors[i]);
           }
           const auto& ant_behaviors = nearby_ant->GetAllBehaviors();
           for (int i = 0; i < ant_behaviors.size(); i++) {
             nearby_ant->RemoveBehavior(ant_behaviors[i]);
           }

           // 3. Add collective movement underground
           // needs work with positional movement
           auto& pos_ant = nearby_ant->GetPosition(); // get position of cell
           auto& pos_larva = larva->GetPosition(); // get position of cell

           while ((pos_ant[0] > 150) && (pos_larva[0] > 150)) { // if overground (i.e. y>150 )
             Double3 ymovement = {{0,1,0}};
             auto new_pos_ant = pos_ant - ymovement; // update position
             auto new_pos_larva = pos_larva - ymovement; // update position
             larva->SetPosition(new_pos_larva);
             nearby_ant->SetPosition(new_pos_ant);
             larva->SetPosition(new_pos_larva);
           }

           // 4. Update vectors of bonded agents
           // add bonded butterfly to ant bond vector
           std::vector<Cell*> bonded_bfly_new{};
           bonded_bfly_new = nearby_ant->GetBondedButterfly();
           bonded_bfly_new.push_back(larva);
           nearby_ant->SetBondedButterfly(bonded_bfly_new);
           // Get vector of ants bonded to butterfly
           std::vector<Cell*> bonded_ants_new{};
           bonded_ants_new = larva->GetBondedAnts();
           bonded_ants_new.push_back(nearby_ant);
           larva->SetBondedAnts(bonded_ants_new);

         }

       });

       ctxt->ForEachNeighbor(check_surrounding, *larva, squared_dist);
   }

   private:
     double squared_dist_ {1};
};

}  // namespace bdm

#endif  // BEHAVIOR_H_
