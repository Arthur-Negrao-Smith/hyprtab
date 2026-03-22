#include <iostream>
#include <string>

#include <gtkmm/application.h>

#include "ipc.hpp"
#include "window.hpp"

int main(int argc, char *argv[]) {
  const HyprTab::CHyprlandChannel channel;

  std::string json = channel.request("j/clients");
  std::cout << "Resposta do Hyprland:\n" << json << std::endl;

  auto app = Gtk::Application::create("org.hyprtab");

  app->make_window_and_run<HyprTab::HyprtabWindow>(argc, argv);
  return 0;
}
