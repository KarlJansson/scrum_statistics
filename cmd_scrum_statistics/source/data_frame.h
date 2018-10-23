#pragma once

#include <any>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

class DataFrameView {
 public:
  void AddView(size_t x, size_t y, std::string_view view);
  std::string_view GetView(size_t x, size_t y);
  size_t GetSizeX();
  size_t GetSizeY();

 private:
  std::vector<std::vector<std::string_view>> frame_;
};

class DataFrame {
 public:
  std::optional<DataFrameView> GetDataFrameView();
  void CreateDataFrame(std::vector<char>&& data,
                       std::unique_ptr<DataFrameView>&& master_view);

 private:
  std::vector<char> raw_data_;
  std::unique_ptr<DataFrameView> master_view_;
};
