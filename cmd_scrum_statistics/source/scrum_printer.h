#pragma once
#include "data_frame.h"

class ScrumPrinter {
 public:
  void Print(DataFrame& frame);

 private:
  void PointBurndown(DataFrameView view);
};
