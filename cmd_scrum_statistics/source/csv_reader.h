#pragma once

#include "data_frame.h"

class CsvReader {
 public:
  DataFrame Parse(const std::string& path);

 private:
  std::unique_ptr<DataFrameView> SetupViews(std::vector<char>& data);
};
