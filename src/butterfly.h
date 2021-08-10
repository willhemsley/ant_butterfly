// Adapted from demo tumor_concept and soma_clustering tutorials

#ifndef BUTTERFLY_H_
#define BUTTERFLY_H_

#include <biodynamo.h>

namespace bdm {

class Butterfly : public Cell {
  BDM_AGENT_HEADER(Butterfly, Cell, 1);

 public:
  Butterfly() {}
  explicit Butterfly(const Double3& position) : Base(position) {}
  virtual ~Butterfly() {}

  // Get and set cell type
  void SetButterflyType(int type) { butterfly_type_ = type; }
  int GetButterflyType() const { return butterfly_type_; }

  // Get and set cell colour
  void SetButterflyColour(int butterfly_colour) { butterfly_colour_ = butterfly_colour; }
  int GetButterflyColour() const { return butterfly_colour_; }

 private:
  int butterfly_type_;
  int butterfly_colour_;
};

}  // namespace bdm

#endif  // BUTTERFLY_H_
