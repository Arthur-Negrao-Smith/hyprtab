#pragma once

#include "gdkmm/enums.h"
#include "view.hpp"
#include <gtkmm-4.0/gtkmm.h>

namespace HyprTab {

class HyprtabWindow : public Gtk::Window {
private:
  CAppsView mAppsBox;

  bool onWindowKeyPressed(guint keyval, guint keycode, Gdk::ModifierType state);
  void onWindowClick(int n_press, double click_x, double click_y);

public:
  HyprtabWindow();
};

} // namespace HyprTab
