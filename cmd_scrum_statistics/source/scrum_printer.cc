#include "scrum_printer.h"

#include "graph_maker.h"

void ScrumPrinter::Print(DataFrame& frame) {
  auto frame_view = frame.GetDataFrameView();
  if (frame_view) {
    Histograms(*frame_view);
    Burndowns(*frame_view);
    ExtraHours(*frame_view);
  }
}

void ScrumPrinter::Histograms(DataFrameView& view) {
  GraphMaker g_maker(GraphMaker::kHistogram);
  std::string last_sprint_id;
  auto filtered_view =
      view.FilterFrameView({"SprintId", "Points", "SprintFocus"});
  std::unique_ptr<DataFrameView> tmp_view =
      std::make_unique<DataFrameView>(view.GetName());
  tmp_view->AppendRow(0, filtered_view);
  for (auto i = 1; i < filtered_view.GetSizeY(); ++i) {
    auto sprint_id = filtered_view.GetView(0, i);
    if (!sprint_id.empty()) {
      if (std::string(last_sprint_id).compare(std::string(sprint_id)) != 0 ||
          filtered_view.GetSizeY() - 1 == i) {
        tmp_view->AppendRow(i, filtered_view);
        if (!last_sprint_id.empty()) tmp_view->AppendRow(i - 1, filtered_view);
        last_sprint_id = sprint_id;
      }
    }
  }
  if (tmp_view->GetSizeX() > 1)
    g_maker.MakeGraph("Histogram " + last_sprint_id, *tmp_view);
}

void ScrumPrinter::Burndowns(DataFrameView& view) {
  GraphMaker g_maker(GraphMaker::kLinePoint);
  std::string last_sprint_id;
  auto filtered_view = view.FilterFrameView({"Days", "Points", "Stories"});
  std::unique_ptr<DataFrameView> tmp_view =
      std::make_unique<DataFrameView>(view.GetName());
  tmp_view->AppendRow(0, filtered_view);
  auto sprint_id_view = view.FilterFrameView({"SprintId"});
  for (auto i = 1; i < filtered_view.GetSizeY(); ++i) {
    auto sprint_id = sprint_id_view.GetView(0, i);
    if (!sprint_id.empty()) {
      if (std::string(last_sprint_id).compare(std::string(sprint_id)) != 0) {
        if (!last_sprint_id.empty() && tmp_view->GetSizeX() > 1)
          g_maker.MakeGraph("Burndown " + last_sprint_id, *tmp_view);
        last_sprint_id = sprint_id;
        tmp_view = std::make_unique<DataFrameView>(view.GetName());
        tmp_view->AppendRow(0, filtered_view);
      }
    }
    tmp_view->AppendRow(i, filtered_view);
  }
  if (tmp_view->GetSizeX() > 1)
    g_maker.MakeGraph("Burndown " + last_sprint_id, *tmp_view);
}

void ScrumPrinter::ExtraHours(DataFrameView& view) {}
