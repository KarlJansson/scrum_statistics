#pragma once
#include "data_frame.h"

class GraphMaker {
 public:
  void MakeGraph(const std::string& out, DataFrameView& view);

 private:
  std::string GeneratePlot(DataFrameView& view);
  std::string SetStyle(DataFrameView& view);
};
