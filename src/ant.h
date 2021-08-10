// Adapted from demo tumor_concept and soma_clustering tutorials

#ifndef ANT_H_
#define ANT_H_

#include <biodynamo.h>
#include <vector>

namespace bdm {

class Ant : public Cell {
  BDM_AGENT_HEADER(Ant, Cell, 1);

 public:
  Ant() {}
  explicit Ant(const Double3& position) : Base(position) {}
  virtual ~Ant() {}

  // Get and set cell type
  void SetAgentType(int type) { ant_type_ = type; }
  int GetAgentType() const { return ant_type_; }

  // Get and set cell colour
  // void SetAgentColour(int ant_colour) { ant_colour_ = ant_colour; }
  // int GetAgentColour() const { return ant_colour_; }

  // get and set bonded larva
  void SetBondedButterfly(vector<Cell*> bonded_bfly) { bonded_bfly_ = bonded_bfly; }
  vector<Cell*> GetBondedButterfly() { return bonded_bfly_; }

 private:
  int ant_type_ = 1;
  // int ant_colour_;
  vector<Cell*> bonded_bfly_{};
};

}  // namespace bdm

#endif  // ANT_H_
