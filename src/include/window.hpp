#pragma once

#include "gdkmm/enums.h"
#include "gtkmm/flowbox.h"
#include "icon.hpp"
#include <gtkmm-4.0/gtkmm.h>
#include <vector>

namespace HyprTab {

class HyprtabWindow : public Gtk::Window {
private:
  Gtk::FlowBox mAppsBox;
  std::vector<AppIcon> apps;

  bool onWindowKeyPressed(guint keyval, guint keycode, Gdk::ModifierType state);
  void onWindowClick(int n_press, double click_x, double click_y);

public:
  HyprtabWindow();
};

} // namespace HyprTab
