// Adapted from demo tumor_concept and soma_clustering tutorials

#ifndef ANT_H_
#define ANT_H_

#include <biodynamo.h>

namespace bdm {

class Ant : public Cell {
  BDM_AGENT_HEADER(Ant, Cell, 1);

 public:
  Ant() {}
  explicit Ant(const Double3& position) : Base(position) {}
  virtual ~Ant() {}

  // Get and set cell type
  void SetAntType(int type) { ant_type_ = type; }
  int GetAntType() const { return ant_type_; }

  // Get and set cell colour
  void SetAntColour(int ant_colour) { ant_colour_ = ant_colour; }
  int GetAntColour() const { return ant_colour_; }

 private:
  int ant_type_;
  int ant_colour_;
};

}  // namespace bdm

#endif  // ANT_H_
