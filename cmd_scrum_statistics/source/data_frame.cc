#include "data_frame.h"

#include <iostream>

DataFrameView::DataFrameView(const std::string& name) : name_(name) {}

const std::string& DataFrameView::GetName() { return name_; }

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
  if (frame_.empty()) return 0;

  auto y = 0;
  for (auto& vec : frame_)
    if (y < vec.size()) y = vec.size();
  return y;
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
  std::size_t y = GetSizeY();
  for (auto x = 0; x < row.size(); ++x)
    if (!row[x].empty()) AddView(x, y, row[x]);
}

void DataFrameView::AppendRow(std::size_t row, DataFrameView& view) {
  std::size_t y = GetSizeY();
  for (auto x = 0; x < view.GetSizeX(); ++x) {
    auto str_view = view.GetView(x, row);
    if (!str_view.empty()) AddView(x, y, str_view);
  }
}

float DataFrameView::MaxValue(const std::string& name) {
  float max_value = 0.f;
  for (auto i = 0; i < frame_.size(); ++i)
    if (!frame_[i].empty() && std::string(frame_[i][0]).compare(name) == 0)
      for (auto ii = 1; ii < frame_[i].size(); ++ii)
        if (!frame_[i][ii].empty())
          if (float value = std::stof(std::string(frame_[i][ii]));
              value > max_value)
            max_value = value;
  return max_value;
}

DataFrameView DataFrameView::FilterFrameView(
    std::vector<std::string> column_names) {
  DataFrameView view(name_);
  for (auto i = 0; i < column_names.size(); ++i)
    for (auto ii = 0; ii < frame_.size(); ++ii)
      if (column_names[i].compare(std::string(frame_[ii][0])) == 0)
        view.AppendColumn(frame_[ii]);
  return view;
}

DataFrame::DataFrame(std::vector<char>&& data,
                     std::unique_ptr<DataFrameView>&& master_view) {
  master_view_ = std::move(master_view);
  raw_data_ = std::move(data);
}

std::optional<DataFrameView> DataFrame::GetDataFrameView() {
  return master_view_ ? std::optional<DataFrameView>(*master_view_)
                      : std::optional<DataFrameView>();
}
