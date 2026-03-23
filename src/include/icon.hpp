#pragma once

#include <gtkmm-4.0/gtkmm/box.h>
#include <gtkmm-4.0/gtkmm/image.h>
#include <gtkmm-4.0/gtkmm/label.h>
#include <gtkmm/enums.h>
#include <gtkmm/object.h>
#include <pangomm/layout.h>
#include <string>

namespace HyprTab {

constexpr int ICON_SIZE = 64;

struct AppData {
  std::string title;
  std::string className;
  std::string address;
  int focusHistoryID;
  bool isDuplicated;

  AppData(std::string title, std::string className, std::string address,
          int focusHistoryID, bool isDuplicated)
      : title(title), className(className), address(address),
        focusHistoryID(focusHistoryID), isDuplicated(isDuplicated) {}
};

class CAppIcon : public Gtk::Box {
public:
  const std::string title;
  const std::string className;
  const std::string address;
  const std::string fullName;
  const int focusHistoryID;
  Gtk::Image image;

  CAppIcon(AppData &app)
      : title(app.title), className(app.className), address(app.address),
        fullName(app.className + ": " + app.title),
        focusHistoryID(app.focusHistoryID) {

    this->set_orientation(Gtk::Orientation::VERTICAL);
    this->add_css_class("app-icon");

    this->set_size_request(100, 100);
    this->set_spacing(10);

    this->set_halign(Gtk::Align::CENTER);
    this->set_valign(Gtk::Align::CENTER);

    auto image = Gtk::make_managed<Gtk::Image>();
    auto iconTitle =
        Gtk::make_managed<Gtk::Label>(app.isDuplicated ? fullName : className);

    // set the icon image
    image->set_size_request(ICON_SIZE, ICON_SIZE);
    image->set_from_icon_name(className);
    image->set_pixel_size(ICON_SIZE);

    // label format
    iconTitle->set_ellipsize(Pango::EllipsizeMode::END);
    iconTitle->set_lines(1);
    iconTitle->set_max_width_chars(1);
    iconTitle->set_halign(Gtk::Align::FILL);

    append(*image);
    append(*iconTitle);
  }
};

} // namespace HyprTab
