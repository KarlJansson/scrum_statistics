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
  DataFrameView name_view, tmp_view;
  auto filtered_view = view.FilterFrameView({"Days", "Points", "Stories"});
  name_view.AppendRow(0, filtered_view);
  auto sprint_id_view = view.FilterFrameView({"SprintId"});
  for (auto i = 1; i < filtered_view.GetSizeY(); ++i) {
    auto sprint_id = sprint_id_view.GetView(0, i);
    if (!sprint_id.empty()) {
      if (std::string(last_sprint_id).compare(std::string(sprint_id)) != 0) {
        if (!last_sprint_id.empty() && tmp_view.GetSizeX() > 1)
          g_maker.MakeGraph("Burndown " + last_sprint_id, tmp_view);
        last_sprint_id = sprint_id;
        tmp_view = name_view;
      }
    }
    tmp_view.AppendRow(i, filtered_view);
  }
  if (tmp_view.GetSizeX() > 1)
    g_maker.MakeGraph("Burndown " + last_sprint_id, tmp_view);
}
