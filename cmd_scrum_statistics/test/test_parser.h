#include "csv_reader.h"

TEST(CsvReader, parse_data) {
  CsvReader reader;
  reader.Parse("./data.csv");
}
