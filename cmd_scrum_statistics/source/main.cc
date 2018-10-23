#include <filesystem>
#include "csv_reader.h"
#include "scrum_printer.h"

int main(int count, char** vargs) {
  CsvReader reader;
  ScrumPrinter printer;

  for (auto i = 1; i < count; ++i) {
    auto frame = reader.Parse(vargs[i]);
    printer.Print(frame);
  }

  return 1;
}
