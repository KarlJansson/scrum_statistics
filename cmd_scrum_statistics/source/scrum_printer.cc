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
  std::string_view sprint_id;
  std::string_view last_sprint_id;

  DataFrameView result_view(view.GetName());
  auto filtered_view = view.FilterFrameView(
      {"SprintId", "Points", "SprintFocus", "SprintTime", "ExtraTime"});
  result_view.AppendRow({StoreCustom("SprintId"), StoreCustom("PointTime"),
                         StoreCustom("Meetings"), StoreCustom("ExtraTime")});

  DataFrameView tmp_view(view.GetName());
  tmp_view.AppendRow(0, filtered_view);
  for (auto i = 1; i < filtered_view.GetSizeY(); ++i) {
    sprint_id = filtered_view.GetView(0, i);
    if (!sprint_id.empty()) {
      if (!last_sprint_id.empty() &&
          std::string(last_sprint_id).compare(std::string(sprint_id)) != 0) {
        result_view.AppendRow(HistogramRow(tmp_view, last_sprint_id));
        tmp_view.AppendRow(0, filtered_view);
      }
      last_sprint_id = sprint_id;
    }
    tmp_view.AppendRow(i, filtered_view);
  }
  result_view.AppendRow(HistogramRow(tmp_view, last_sprint_id));

  return result_view;
}

std::vector<std::string_view> ScrumPrinter::HistogramRow(
    DataFrameView& view, std::string_view sprint_id) {
  auto extra_time = view.MaxValue("ExtraTime");
  auto sprint_time = view.MaxValue("SprintTime");
  auto sprint_focus = view.MaxValue("SprintFocus");
  auto focus_time = (1.f - (sprint_focus / 100.f)) * sprint_time;
  sprint_time -= focus_time + extra_time;
  view.Clear();

  return {sprint_id, StoreCustom(std::to_string(sprint_time)),
          StoreCustom(std::to_string(focus_time)),
          StoreCustom(std::to_string(extra_time))};
}

DataFrameView ScrumPrinter::PrepareHBurndownData(DataFrameView& view) {
  DataFrameView tmp_view(view.GetName());
  return tmp_view;
}

std::string_view ScrumPrinter::StoreCustom(const std::string& value) {
  string_buffer_.push_back(value);
  return std::string_view(string_buffer_.back().data(),
                          string_buffer_.back().size());
}
