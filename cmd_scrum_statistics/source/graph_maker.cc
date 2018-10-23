#include "graph_maker.h"

void GraphMaker::MakeGraph(const std::string& out, DataFrameView& view) {
  std::string out_command = "set output '" + out + ".png'\n";

  FILE* fp;
  fp = popen("gnuplot", "w");
  fprintf(fp, "reset\n");
  fprintf(fp, "set terminal png font arial 14 size 800,600\n");
  fprintf(fp, "%s", out_command.c_str());
  fprintf(fp, "%s", SetStyle().c_str());
  fprintf(fp, "%s", GeneratePlot(view).c_str());

  pclose(fp);
}

std::string GraphMaker::GeneratePlot(DataFrameView& view) {
  std::string out = "$data << EOD\n";

  std::string_view str_view;
  std::vector<std::string_view> last_value(view.GetSizeX());
  for (auto y = 1; y < view.GetSizeY(); ++y) {
    for (auto x = 0; x < view.GetSizeX(); ++x) {
      str_view = view.GetView(x, y);
      out +=
          str_view.empty() ? std::string(last_value[x]) : std::string(str_view);
      if (x < view.GetSizeX() - 1) out += " ";
      if (!str_view.empty()) last_value[x] = str_view;
    }
    out += "\n";
  }

  out += "EOD\n";
  out += "plot ";
  for (auto i = 1; i < view.GetSizeX(); ++i) {
    out += "'$data' " + std::to_string(1) + ":" + std::to_string(i + 1) +
           " t \"" + std::string(view.GetView(i, 0)) +
           "\" with linespoints ls " + std::to_string(i);
    out += i == view.GetSizeX() - 1 ? "" : ",\\\n";
  }
  return out;
}

std::string GraphMaker::SetStyle() {
  return "set style line 1 lt rgb \"#A00000\" lw 2 pt 1\n"
         "set style line 2 lt rgb \"#00A000\" lw 2 pt 6\n"
         "set style line 3 lt rgb \"#5060D0\" lw 2 pt 2\n"
         "set style line 4 lt rgb \"#F25900\" lw 2 pt 9\n"
         // Line style for axes
         "set style line 80 lt rgb \"#808080\"\n"
         // Line style for grid
         "set style line 81 lt 0  # dashed\n"
         "set style line 81 lt rgb \"#808080\"  # grey\n"
         "set grid back linestyle 81\n"
         "set border 3 back linestyle 80\n"
         "set xtics nomirror\n"
         "set ytics nomirror\n";
}
