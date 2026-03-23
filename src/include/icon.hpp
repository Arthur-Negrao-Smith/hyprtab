#pragma once

#include <gtkmm-4.0/gtkmm/box.h>
#include <gtkmm-4.0/gtkmm/image.h>
#include <gtkmm-4.0/gtkmm/label.h>
#include <gtkmm/enums.h>
#include <gtkmm/object.h>
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

    auto image = Gtk::make_managed<Gtk::Image>();
    auto iconTitle =
        Gtk::make_managed<Gtk::Label>(app.isDuplicated ? fullName : className);

    // set the icon image
    image->set_from_icon_name(className);
    image->set_pixel_size(ICON_SIZE);

    append(*image);
    append(*iconTitle);
  }
};

} // namespace HyprTab
