#include "data_frame.h"

#include <iostream>

void DataFrameView::AddView(size_t x, size_t y, std::string_view view) {
  while (frame_.size() <= x) frame_.push_back({});
  while (frame_[x].size() <= y) frame_[x].push_back({});
  frame_[x][y] = view;
}

std::string_view DataFrameView::GetView(size_t x, size_t y) {
  if (frame_.size() <= x) return {};
  if (frame_[x].size() <= y) return {};
  return frame_[x][y];
}

void DataFrameView::PrintView() {
  for (auto& x : frame_) {
    for (auto y : x) std::cout << std::string(y) << ",";
    std::cout << std::endl;
  }
}

size_t DataFrameView::GetSizeX() { return frame_.size(); }

size_t DataFrameView::GetSizeY() {
  return frame_.empty() ? 0 : frame_[0].size();
}
void DataFrameView::RemoveColumn(size_t x) {
  frame_.erase(std::begin(frame_) + x);
}

void DataFrameView::RemoveRow(size_t y) {
  for (auto& vec : frame_) vec.erase(std::begin(vec) + y);
}

void DataFrameView::AppendColumn(std::vector<std::string_view> column) {
  frame_.emplace_back(column);
}

void DataFrameView::AppendRow(std::vector<std::string_view> row) {
  auto x = 0;
  for (auto& vec : frame_) vec.emplace_back(row[x++]);
}

void DataFrameView::AppendRow(std::size_t row, DataFrameView& view) {
  std::size_t y = GetSizeY();
  for (auto x = 0; x < view.GetSizeX(); ++x) {
    auto str_view = view.GetView(x, row);
    if (!str_view.empty()) AddView(x, y, str_view);
  }
}

DataFrameView DataFrameView::FilterFrameView(
    std::set<std::string> column_names) {
  DataFrameView view;
  for (auto i = 0; i < frame_.size(); ++i) {
    if (column_names.find(std::string(frame_[i][0])) != std::end(column_names))
      view.AppendColumn(frame_[i]);
  }
  return view;
}

std::optional<DataFrameView> DataFrame::GetDataFrameView() {
  return master_view_ ? std::optional<DataFrameView>(*master_view_)
                      : std::optional<DataFrameView>();
}

void DataFrame::CreateDataFrame(std::vector<char>&& data,
                                std::unique_ptr<DataFrameView>&& master_view) {
  master_view_ = std::move(master_view);
  raw_data_ = std::move(data);
}
