#include "csv_reader.h"
#include "scrum_printer.h"

#include <filesystem>
#include <sstream>

int main(int count, char** vargs) {
  CsvReader reader;
  ScrumPrinter printer;

  bool commandline_used = false;
  for (auto i = 1; i < count; ++i) {
    auto frame = reader.Parse(vargs[i]);
    printer.Print(frame);
    commandline_used = true;
  }

  if (!commandline_used) {
    for (auto path : std::filesystem::directory_iterator("./")) {
      std::stringstream ext_stream;
      ext_stream << path;
      std::string path_str = ext_stream.str();
      if (path_str.find(".csv") != std::string::npos) {
        while (path_str.back() == '"') path_str.pop_back();
        while (!path_str.empty() && path_str[0] == '"')
          path_str = path_str.substr(1, path_str.size());
        auto frame = reader.Parse(path_str);
        printer.Print(frame);
      }
    }
  }

  return 1;
}
