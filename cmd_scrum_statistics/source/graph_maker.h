#pragma once
#include "data_frame.h"

class GraphMaker {
 public:
  enum GraphType { kLinePoint, kLine, kHistogram, kBar };

  GraphMaker(const GraphType type);
  void MakeGraph(const std::string& out, DataFrameView& view);

 private:
  std::string GeneratePlot(DataFrameView& view);
  std::string SetStyle(DataFrameView& view);

  const GraphType type_;
};
