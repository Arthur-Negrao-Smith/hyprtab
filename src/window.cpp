#include <gdk/gdkkeysyms.h>
#include <gtk4-layer-shell/gtk4-layer-shell.h>
#include <gtkmm/eventcontrollerkey.h>
#include <gtkmm/gestureclick.h>
#include <iostream>
#include <sigc++/functors/mem_fun.h>

#include "gdkmm/enums.h"
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
                              GTK_LAYER_SHELL_KEYBOARD_MODE_EXCLUSIVE);

  this->set_title("hyprtab");
  this->set_default_size(200, 200);

  this->set_resizable(true);
  this->set_decorated(false);

  this->set_name("hyprtab-window");

  this->loadCss();

  auto controller = Gtk::EventControllerKey::create();
  controller->signal_key_pressed().connect(
      sigc::mem_fun(*this, &HyprTab::HyprtabWindow::onWindowKeyPressed), false);
  controller->signal_key_released().connect(
      sigc::mem_fun(*this, &HyprtabWindow::onWindowKeyReleased), false);

  add_controller(controller);

  auto click_controller = Gtk::GestureClick::create();
  click_controller->signal_pressed().connect(
      sigc::mem_fun(*this, &HyprtabWindow::onWindowClick));
  add_controller(click_controller);

  set_child(mAppsBox);
}

void HyprtabWindow::loadCss() {
  auto cssProvider = Gtk::CssProvider::create();

  std::string configPath =
      std::string(std::getenv("HOME")) + "/.config/hyprtab/style.css";
  auto cssFile = Gio::File::create_for_path(configPath);

  try {
    if (cssFile->query_exists()) {
      cssProvider->load_from_file(cssFile);
      std::cout << "Load a custom Css: " << configPath << std::endl;
    } else {
      // default global css
      cssProvider->load_from_data(
          "window#hyprtab-window, window#hyprtab-window decoration { "
          "  background: transparent; "
          "  background-color: rgba(0, 0, 0, 0.0); "
          "  box-shadow: none; "
          "} "
          "flowbox { "
          "  border-radius: 12px; "
          "  background-color: rgba(0.234, 0.234, 0.234, 1.0); "
          "} "
          "flowboxchild { "
          "  min-width: 120px;"
          "  min-height: 120px;"
          "  border-radius: 12px; "
          "  padding: 8px; "
          "} "
          "flowboxchild:selected { "
          "  background-color: rgba(255, 255, 255, 0.15); "
          "} "
          ".app-icon label { "
          "  color: white; "
          "  font-weight: bold; "
          "  margin-top: 5px; "
          "}");
    }

    Gtk::StyleContext::add_provider_for_display(
        Gdk::Display::get_default(), cssProvider,
        GTK_STYLE_PROVIDER_PRIORITY_USER);
  } catch (const std::exception &ex) {
    std::cerr << "Erro to load the CSS: " << ex.what() << std::endl;
  }
}

bool HyprtabWindow::onWindowKeyPressed(guint keyval, guint /*keycode*/,
                                       Gdk::ModifierType state) {
  // navigation
  switch (keyval) {
  case GDK_KEY_Tab:
  case GDK_KEY_ISO_Left_Tab: {
    bool backwards = (state & Gdk::ModifierType::SHIFT_MASK) !=
                         Gdk::ModifierType::NO_MODIFIER_MASK ||
                     keyval == GDK_KEY_ISO_Left_Tab;
    mAppsBox.moveSelection(backwards);
    return true;
  }

  // modifiers, arrow navigation and Alt
  case GDK_KEY_Left:
  case GDK_KEY_Right:
  case GDK_KEY_Down:
  case GDK_KEY_Up:
  case GDK_KEY_Shift_L:
  case GDK_KEY_Shift_R:
  case GDK_KEY_Alt_L:
  case GDK_KEY_Alt_R:
    return false;

  // confirmation
  case GDK_KEY_Return:
  case GDK_KEY_KP_Enter:
  case GDK_KEY_space: {
    auto selectedList = mAppsBox.get_selected_children();
    if (!selectedList.empty()) {
      mAppsBox.changeFocus(selectedList[0]);
    } else {
      this->close();
    }
    return true;
  }

  default:
    std::cout << "Quit event emited" << std::endl;
    this->close();
    return true;
  }
}

void HyprtabWindow::onWindowKeyReleased(guint keyval, guint /*keycode*/,
                                        Gdk::ModifierType /*state*/) {
  if (keyval == GDK_KEY_Alt_L || keyval == GDK_KEY_Alt_R) {

    auto selectedList = mAppsBox.get_selected_children();

    if (!selectedList.empty()) {
      mAppsBox.changeFocus(selectedList[0]);
    } else {
      this->close();
    }
  }
}

void HyprtabWindow::onWindowClick(int /*n_press*/, double click_x,
                                  double click_y) {
  double flowbox_x, flowbox_y;

  mAppsBox.translate_coordinates(*this, 0, 0, flowbox_x, flowbox_y);

  const int width = mAppsBox.get_width();
  const int height = mAppsBox.get_height();

  if (click_x < flowbox_x || click_y < flowbox_y ||
      click_x > flowbox_x + width || click_y > flowbox_y + height)
    this->close();
}

} // namespace HyprTab
