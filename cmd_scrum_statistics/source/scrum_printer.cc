#include "scrum_printer.h"

#include "graph_maker.h"

void ScrumPrinter::Print(DataFrame& frame) {
  auto frame_view = frame.GetDataFrameView();
  if (frame_view) {
    PointBurndown(*frame_view);
  }
}

void ScrumPrinter::PointBurndown(DataFrameView view) {
  GraphMaker g_maker;
  g_maker.MakeGraph("PointBurndown_", view);
}
