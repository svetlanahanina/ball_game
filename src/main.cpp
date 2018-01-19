#include <QApplication>
#include <QMainWindow>
#include <QResource>

#include "src/kernel/future_cpp_features.h"
#include "src/gui/geometry_data_gui.h"
#include "src/gui/game_process_view.h"

//Test commit changes
int main (int argc, char *argv[])
{
  QApplication app (argc, argv);

  QResource::registerResource("/path/to/myresource.rcc");

  QMainWindow *window = new QMainWindow;

  auto geom_data = std::make_unique<geometry_data_gui> ();
  game_process_view *graph_area = new game_process_view (window, geom_data.get ());

  window->setCentralWidget (graph_area);
  window->setWindowTitle ("Ball Game");
  window->show ();
  int ret = app.exec ();
  delete graph_area;
  delete window;
  return ret;
}
