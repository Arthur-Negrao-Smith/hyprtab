#include <gdk/gdkkeysyms.h>
#include <gtk4-layer-shell/gtk4-layer-shell.h>
#include <gtkmm/eventcontrollerkey.h>
#include <gtkmm/gestureclick.h>
#include <iostream>
#include <sigc++/functors/mem_fun.h>

#include "window.hpp"

namespace HyprTab {

HyprtabWindow::HyprtabWindow() {

  gtk_layer_init_for_window(this->gobj());

  // overlay layer
  gtk_layer_set_layer(this->gobj(), GTK_LAYER_SHELL_LAYER_OVERLAY);

  // remove anchors to centralize
  gtk_layer_set_anchor(this->gobj(), GTK_LAYER_SHELL_EDGE_TOP, true);
  gtk_layer_set_anchor(this->gobj(), GTK_LAYER_SHELL_EDGE_BOTTOM, true);
  gtk_layer_set_anchor(this->gobj(), GTK_LAYER_SHELL_EDGE_LEFT, true);
  gtk_layer_set_anchor(this->gobj(), GTK_LAYER_SHELL_EDGE_RIGHT, true);

  // keyboard input
  gtk_layer_set_keyboard_mode(this->gobj(),
                              GTK_LAYER_SHELL_KEYBOARD_MODE_ON_DEMAND);

  set_title("hyprtab");
  set_default_size(200, 200);

  set_resizable(true);
  set_decorated(false);

  auto controller = Gtk::EventControllerKey::create();
  controller->signal_key_pressed().connect(
      sigc::mem_fun(*this, &HyprTab::HyprtabWindow::onWindowKeyPressed), false);

  add_controller(controller);

  auto click_controller = Gtk::GestureClick::create();
  click_controller->signal_pressed().connect(
      sigc::mem_fun(*this, &HyprtabWindow::onWindowClick));
  add_controller(click_controller);

  set_child(mAppsBox);
}

bool HyprtabWindow::onWindowKeyPressed(guint keyval, guint keycode,
                                       Gdk::ModifierType state) {
  if (keyval == GDK_KEY_q || keyval == GDK_KEY_Escape) {
    this->close();

    std::cout << "Done" << std::endl;

    return true;
  }

  return false;
}

void HyprtabWindow::onWindowClick(int n_press, double click_x, double click_y) {
  double flowbox_x, flowbox_y;

  mAppsBox.translate_coordinates(*this, 0, 0, flowbox_x, flowbox_y);

  const int width = mAppsBox.get_width();
  const int height = mAppsBox.get_height();

  if (click_x < flowbox_x || click_y < flowbox_y ||
      click_x > flowbox_x + width || click_y > flowbox_y + height)
    this->close();
}

} // namespace HyprTab
