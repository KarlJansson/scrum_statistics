#pragma once
#include <list>
#include "data_frame.h"

class ScrumPrinter {
 public:
  void Print(DataFrame& frame);

 private:
  void Histograms(DataFrameView& view);
  void Burndowns(DataFrameView& view);

  DataFrameView PrepareHistogramData(DataFrameView& view);
  std::vector<std::string_view> HistogramRow(DataFrameView& view,
                                             std::string_view sprint_id);

  DataFrameView PrepareHBurndownData(DataFrameView& view);

  std::string_view StoreCustom(const std::string& value);

  std::list<std::string> string_buffer_;
};
