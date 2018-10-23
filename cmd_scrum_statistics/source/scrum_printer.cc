#include "scrum_printer.h"

#include "graph_maker.h"

void ScrumPrinter::Print(DataFrame& frame) {
  auto frame_view = frame.GetDataFrameView();
  if (frame_view) {
    PointBurndown(*frame_view);
  }
}

void ScrumPrinter::PointBurndown(DataFrameView& view) {
  GraphMaker g_maker;
  std::string last_sprint_id;
  auto filtered_view = view.FilterFrameView({"Days", "NrPoints", "NrStories"});
  auto sprint_id_view = view.FilterFrameView({"SprintId"});
  for (auto i = 1; i < sprint_id_view.GetSizeY(); ++i) {
    if (!sprint_id_view.GetView(0, i).empty()) {
      if (last_sprint_id.empty()) last_sprint_id = sprint_id_view.GetView(0, i);
    }
  }
  g_maker.MakeGraph("pointBurndown_" + last_sprint_id, filtered_view);
}
