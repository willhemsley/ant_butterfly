// Adapted from demo tumor_concept and soma_clustering tutorials

#ifndef BUTTERFLY_H_
#define BUTTERFLY_H_

#include <biodynamo.h>
#include <vector>

namespace bdm {

class Butterfly : public Cell {
  BDM_AGENT_HEADER(Butterfly, Cell, 1);

 public:
  Butterfly() {}
  explicit Butterfly(const Double3& position) : Base(position) {}
  virtual ~Butterfly() {}

  // Get and set cell type
  void SetAgentType(int type) { butterfly_type_ = type; }
  int GetAgentType() const { return butterfly_type_; }

  // Get and set cell colour
  // void SetButterflyColour(int butterfly_colour) { butterfly_colour_ = butterfly_colour; }
  // int GetButterflyColour() const { return butterfly_colour_; }

  // get and set bonded ants
  void SetBondedAgents(std::vector<Cell*> bonded_ants) { bonded_ants_ = bonded_ants; }
  std::vector<Cell*> GetBondedAgents() { return bonded_ants_; }

 private:
  int butterfly_type_ = -1;
  // int butterfly_colour_;
  std::vector<Cell*> bonded_ants_{};
};

}  // namespace bdm

#endif  // BUTTERFLY_H_
