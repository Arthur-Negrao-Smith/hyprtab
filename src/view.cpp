#include "view.hpp"
#include "gtkmm/enums.h"
#include "gtkmm/flowboxchild.h"
#include "gtkmm/object.h"
#include "gtkmm/window.h"
#include "icon.hpp"
#include "ipc.hpp"
#include <algorithm>
#include <iostream>
#include <string>

namespace HyprTab {

using json = nlohmann::json;

CAppsView::CAppsView() {
  this->set_halign(Gtk::Align::CENTER);
  this->set_valign(Gtk::Align::CENTER);
  this->set_max_children_per_line(MAX_CHILDRENS_PER_LINE);

  this->signal_child_activated().connect([this](Gtk::FlowBoxChild *child) {
    auto appIcon = dynamic_cast<CAppIcon *>(child->get_child());

    if (appIcon) {
      std::string command = "dispatch focuswindow address:" + appIcon->address;

      std::cout << "Running: " << command << std::endl;

      mChannel.request(command);

      auto window = dynamic_cast<Gtk::Window *>(this->get_root());
      if (window) {
        std::cout << "Changing focus to: " << appIcon->fullName;
        window->close();
      }
    }
  });

  auto response = json::parse(mChannel.request("j/clients"));
  setIcons(response);
}

void CAppsView::setIcons(const json &icons) {
  this->remove_all();
  mAppDataVector.clear();

  for (auto &icon : icons) {
    std::string title = icon["title"];
    std::string className = icon["class"];
    std::string address = icon["address"];
    int focusHistoryID = icon["focusHistoryID"];

    mAppDataVector.push_back(
        AppData(title, className, address, focusHistoryID));
  }

  std::sort(mAppDataVector.begin(), mAppDataVector.end(),
            [](const AppData &a, const AppData &b) {
              return a.focusHistoryID < b.focusHistoryID;
            });

  for (auto &app : mAppDataVector) {
    auto appIcon = Gtk::make_managed<CAppIcon>(app);

    append(*appIcon);
  }
}

} // namespace HyprTab
