#include "data_frame.h"

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

size_t DataFrameView::GetSizeX() { return frame_.size(); }

size_t DataFrameView::GetSizeY() {
  return frame_.empty() ? 0 : frame_[0].size();
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
