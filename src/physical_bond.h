// Adapted from demo tumor_concept and soma_clustering tutorials

#ifndef PHYSICAL_BOND_H_
#define PHYSICAL_BOND_H_

#include <biodynamo.h>

namespace bdm {

/**
 * This class represents a physical bond between two agents.
 *
 * @author willhemsley
 *
*/

class PhysicalBond {

  public:
    PhysicalBond() {}

    explicit PhysicalBond(Agent* agent_a, Agent* agent_b)
                                : agent_a_(agent_a), agent_b_(agent_b) {}

    virtual ~PhysicalBond() {}


    // Get and set Agent First Agent (a)
    void SetFirstAgent(Agent* agent_a) {agent_a_ = agent_a; }
    Agent* GetFirstAgent() { return agent_a_; }

    // Get and set Agent Second Agent (b)
    void SetSecondAgent(Agent* agent_b) { agent_b_ = agent_b; }
    Agent* GetSecondAgent() { return agent_b_; }

    // Set if bond has an effect on first agent (a)
    void setHasEffectOnA(bool hasEffectOnA) { hasEffectOnA_ = hasEffectOnA; }
    bool isHasEffectOnA() { return hasEffectOnA_; }

    // Set if bond has an effect on second agent (b)
    void setHasEffectOnB(bool hasEffectOnB) { hasEffectOnB_ = hasEffectOnB; }
    bool isHasEffectOnB() { return hasEffectOnB_; }

    void CreateBond(Agent* agent_a, Agent* agent_b) {
      //??? Lock the two agents together with the bond ???
      //??? Could a cell be made between the two that excretes a chemical ???
      //??? Chemotaxis pulls the cells together ???
      auto* sim = Simulation::GetActive();
      auto* random = sim->GetRandom();

      // Initialise bond substance => does not diffuse or decay
      ModelInitializer::DefineSubstance(kBond, "Bond", 0.1, 0.1, 2);

      // find positions of agents
      const auto& position_a = agent_a->GetPosition(); // get position of agent_1
      const auto& position_b = agent_b->GetPosition(); // get position of agent_2

      // find midpoint of agents
      std::vector<Double3> midpoint_pos = (position1 + position2) / 2;
      auto avg_dist = midpoint_pos[0] + midpoint_pos[1] + midpoint_pos[2] / 3;

      // create new cell (bonding cell) at midpoint
      MyCell* bond = bond MyCell(midpoint_pos);
      bond->SetDiameter(avg_dist); // set size of bond to max distance away
      // bonding cell secretes substabce to keep together
      bond->AddBehavior(new Secretion("Bond"));

      // 5. agents are attracted to the bond cell chemical
      agent1->AddBehavior(new Chemotaxis("Bond", 1));
      agent2->AddBehavior(new Chemotaxis("Bond", 1));
    }

  private:
    Agent* agent_a_;
    Agent* agent_b_;
    bool hasEffectOnA_ = true;
    bool hasEffectOnB_ = true;

};

}  // namespace bdm

#endif  // PHYSICAL_BOND_H_
