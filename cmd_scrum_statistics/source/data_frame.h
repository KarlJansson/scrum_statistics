#pragma once

#include <any>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

class DataFrameView {
 public:
  DataFrameView(const std::string& name);

  const std::string& GetName();

  void AddView(size_t x, size_t y, std::string_view view);
  std::string_view GetView(size_t x, size_t y);

  void PrintView();

  size_t GetSizeX();
  size_t GetSizeY();

  void RemoveColumn(size_t x);
  void RemoveRow(size_t y);

  void AppendColumn(std::vector<std::string_view> column);
  void AppendRow(std::vector<std::string_view> row);
  void AppendRow(std::size_t row, DataFrameView& view);

  DataFrameView FilterFrameView(std::set<std::string> column_names);

 private:
  const std::string& name_;
  std::vector<std::vector<std::string_view>> frame_;
};

class DataFrame {
 public:
  DataFrame(std::vector<char>&& data,
            std::unique_ptr<DataFrameView>&& master_view);

  std::optional<DataFrameView> GetDataFrameView();

 private:
  std::vector<char> raw_data_;
  std::unique_ptr<DataFrameView> master_view_;
};
