// Adapted from demo tumor_concept and soma_clustering tutorials

#ifndef MY_CELL_H_
#define MY_CELL_H_

#include <biodynamo.h>

namespace bdm {

class MyCell : public Cell {
  BDM_AGENT_HEADER(MyCell, Cell, 1);

 public:
  MyCell() {}
  explicit MyCell(const Double3& position) : Base(position) {}
  virtual ~MyCell() {}

  // Get and set cell type
  void SetCellType(int type) { cell_type_ = type; }
  int GetCellType() const { return cell_type_; }

  // Get and set cell colour
  void SetCellColour(int cell_colour) { cell_colour_ = cell_colour; }
  int GetCellColour() const { return cell_colour_; }

 private:
  int cell_type_;
  int cell_colour_;
};

}  // namespace bdm

#endif  // MY_CELL_H_
