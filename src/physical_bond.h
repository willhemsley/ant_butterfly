// Adapted from demo tumor_concept and soma_clustering tutorials

#ifndef PHYSICAL_BOND_H_
#define PHYSICAL_BOND_H_

#include <biodynamo.h>

namespace bdm {

class CreatePhysicalBond : public PhysicalBond {
  BDM_BOND_HEADER(PhysicalBond, PhysicalBond, 1);

public:
 CreatePhysicalBond() {}
 explicit CreatePhysicalBond(Agent* agent_a, Agent* agent_b)
                      : agent_a_(agent_a), agent_b_(agent_b) {}

 virtual ~CreatePhysicalBond() {}


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

 void Run(Agent* agent_a, Agent* agent_b) {
   ??? Need to actually create the bond behavior here ???
 }

private:
  Agent* a_;
  Agent* b_;
  bool hasEffectOnA_ = true;
  bool hasEffectOnB_ = true;

};

}  // namespace bdm

#endif  // PHYSICAL_BOND_H_
