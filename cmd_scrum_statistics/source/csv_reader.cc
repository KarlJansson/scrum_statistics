#include "csv_reader.h"

#include <fstream>

DataFrame CsvReader::Parse(const std::string& path) {
  std::ifstream open(path, std::ios::binary);
  open.seekg(0, std::ios::end);
  std::vector<char> data(open.tellg());
  open.seekg(0, std::ios::beg);
  open.read(data.data(), data.size());
  open.close();

  DataFrame frame(std::move(data), SetupViews(path, data));
  return frame;
}

std::unique_ptr<DataFrameView> CsvReader::SetupViews(const std::string& path,
                                                     std::vector<char>& data) {
  std::unique_ptr<DataFrameView> master_view =
      std::make_unique<DataFrameView>(path);

  std::size_t start = 0, end = 0, x = 0, y = 0, end_tmp, start_tmp;
  for (auto c : data) {
    switch (c) {
      case '\n':
        ++end, ++y, x = 0, start = end;
        break;
      case ',':
        end_tmp = end, start_tmp = start;
        while (start_tmp < end_tmp && data[start_tmp] == ' ') ++start_tmp;
        while (end_tmp - 1 > start_tmp && data[end_tmp - 1] == ' ') --end_tmp;
        master_view->AddView(
            x, y, std::string_view(&data[start_tmp], end_tmp - start_tmp));
        ++end, ++x, start = end;
        break;
      default:
        ++end;
        break;
    }
  }

  return master_view;
}
