#include "csv_reader.h"

#include <fstream>

DataFrame CsvReader::Parse(const std::string& path) {
  std::ifstream open(path, std::ios::binary);
  open.seekg(0, std::ios::end);
  std::vector<char> data(open.tellg());
  open.seekg(0, std::ios::beg);
  open.read(data.data(), data.size());
  open.close();

  DataFrame frame;
  frame.CreateDataFrame(std::move(data), SetupViews(data));
  return frame;
}

std::unique_ptr<DataFrameView> CsvReader::SetupViews(std::vector<char>& data) {
  std::unique_ptr<DataFrameView> master_view =
      std::make_unique<DataFrameView>();

  std::size_t start = 0, end = 0, x = 0, y = 0;
  for (auto c : data) {
    switch (c) {
      case '\r':
        break;
      case '\n':
        if (start != end) {
          while (start < end && data[start] == ' ') ++start;
          while (end - 1 > start && data[end - 1] == ' ') --end;
          master_view->AddView(x, y,
                               std::string_view(&data[start], end - start));
        }
        ++y, x = 0, start = 0, end = 0;
        break;
      case '\t':
      case ',':
        if (start != end) {
          std::size_t end_tmp = end, start_tmp = start;
          while (start_tmp < end_tmp && data[start_tmp] == ' ') ++start_tmp;
          while (end_tmp - 1 > start_tmp && data[end_tmp - 1] == ' ') --end_tmp;
          master_view->AddView(
              x, y, std::string_view(&data[start], end_tmp - start_tmp));
        }
        ++x, start = end;
        break;
      default:
        ++end;
        break;
    }
  }

  return master_view;
}