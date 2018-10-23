#include "csv_reader.h"
#include "scrum_printer.h"

TEST(CsvReader, parse_data) {
  CsvReader reader;
  ScrumPrinter printer;

  auto frame = reader.Parse("./data.csv");
  printer.Print(frame);
}
