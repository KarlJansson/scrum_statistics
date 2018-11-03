#pragma once
#include "data_frame.h"

class ScrumPrinter {
 public:
  void Print(DataFrame& frame);

 private:
  void Histograms(DataFrameView& view);
  void Burndowns(DataFrameView& view);
  void ExtraHours(DataFrameView& view);
};
