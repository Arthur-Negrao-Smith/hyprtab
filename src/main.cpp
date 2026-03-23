#include <gtkmm/application.h>

#include "window.hpp"

using json = nlohmann::json;

int main(int argc, char *argv[]) {
  auto app = Gtk::Application::create("org.hyprtab");

  app->make_window_and_run<HyprTab::HyprtabWindow>(argc, argv);
  return 0;
}
