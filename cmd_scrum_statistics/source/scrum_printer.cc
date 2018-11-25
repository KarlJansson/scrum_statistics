#include "scrum_printer.h"

#include "graph_maker.h"

void ScrumPrinter::Print(DataFrame& frame) {
  auto frame_view = frame.GetDataFrameView();
  if (frame_view) {
    Histograms(*frame_view);
    Burndowns(*frame_view);
  }
}

void ScrumPrinter::Histograms(DataFrameView& view) {
  GraphMaker g_maker(GraphMaker::kHistogram);
  auto tmp_view = PrepareHistogramData(view);
  if (tmp_view.GetSizeX() > 1) g_maker.MakeGraph("Time histograms", tmp_view);
}

void ScrumPrinter::Burndowns(DataFrameView& view) {
  GraphMaker g_maker(GraphMaker::kLinePoint);
  std::vector<std::pair<std::string, std::vector<std::string>>> charts = {
      {"Point burndown ", {"Days", "Points"}},
      {"Story burndown ", {"Days", "Stories"}}};

  for (auto& p : charts) {
    std::string last_sprint_id;
    auto filtered_view = view.FilterFrameView(p.second);
    auto tmp_view = std::make_unique<DataFrameView>(view.GetName());
    tmp_view->AppendRow(0, filtered_view);
    auto sprint_id_view = view.FilterFrameView({"SprintId"});
    for (auto i = 1; i < filtered_view.GetSizeY(); ++i) {
      auto sprint_id = sprint_id_view.GetView(0, i);
      if (!sprint_id.empty()) {
        if (std::string(last_sprint_id).compare(std::string(sprint_id)) != 0) {
          if (!last_sprint_id.empty() && tmp_view->GetSizeX() > 1)
            g_maker.MakeGraph(p.first + last_sprint_id, *tmp_view);
          last_sprint_id = sprint_id;
          tmp_view = std::make_unique<DataFrameView>(view.GetName());
          tmp_view->AppendRow(0, filtered_view);
        }
      }
      tmp_view->AppendRow(i, filtered_view);
    }
    if (tmp_view->GetSizeX() > 1)
      g_maker.MakeGraph(p.first + last_sprint_id, *tmp_view);
  }
}

DataFrameView ScrumPrinter::PrepareHistogramData(DataFrameView& view) {
  std::string last_sprint_id;
  DataFrameView tmp_view(view.GetName());

  auto filtered_view = view.FilterFrameView(
      {"SprintId", "Points", "SprintFocus", "SprintTime", "ExtraTime"});

  int finished_points;
  for (auto i = 1; i < filtered_view.GetSizeY(); ++i) {
    auto sprint_id = filtered_view.GetView(0, i);
    auto points = filtered_view.GetView(1, i);
    if (!sprint_id.empty()) {
      if (std::string(last_sprint_id).compare(std::string(sprint_id)) != 0 ||
          filtered_view.GetSizeY() - 1 == i) {
        tmp_view.AppendRow(i, filtered_view);
        if (!last_sprint_id.empty()) tmp_view.AppendRow(i - 1, filtered_view);
        last_sprint_id = sprint_id;
      }
    } else if (!points.empty()) {
    }
  }

  DataFrameView result_view(view.GetName());
  result_view.AppendRow(0, filtered_view);

  int point_value;
  for (auto i = 0; i < tmp_view.GetSizeY(); ++i) {
    result_view.AppendRow(i, tmp_view);
  }

  return result_view;
}

DataFrameView ScrumPrinter::PrepareHBurndownData(DataFrameView& view) {
  DataFrameView tmp_view(view.GetName());
  return tmp_view;
}
