#pragma once
#include "data_frame.h"

class ScrumPrinter {
 public:
  void Print(DataFrame& frame);

 private:
  void Histograms(DataFrameView& view);
  void Burndowns(DataFrameView& view);

  DataFrameView PrepareHistogramData(DataFrameView& view);
  DataFrameView PrepareHBurndownData(DataFrameView& view);

  std::vector<char> string_buffer_;
};
