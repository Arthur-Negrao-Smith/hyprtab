#include <algorithm>
#include <gtkmm/enums.h>
#include <gtkmm/flowboxchild.h>
#include <gtkmm/object.h>
#include <gtkmm/window.h>
#include <iostream>
#include <string>

#include "icon.hpp"
#include "ipc.hpp"
#include "view.hpp"

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
  mSeenClasses.clear();
  mDuplicatedClasses.clear();

  if (!icons.is_array())
    return;

  // search for duplicates
  for (auto &icon : icons) {
    if (!icon.is_object())
      continue;

    std::string className = icon.value("class", "");

    // if can not insert, then is a duplicated
    if (!mSeenClasses.insert(className).second)
      mDuplicatedClasses.insert(className);
  }

  for (auto &icon : icons) {
    if (!icon.is_object())
      continue;

    std::string className = icon.value("class", "");
    std::string title = icon.value("title", "");
    std::string address = icon.value("address", "");
    int focusHistoryID = icon.value("focusHistoryID", -1);

    if (title.empty() || className.empty())
      continue;

    bool isDuplicated = mDuplicatedClasses.count(className) > 0;

    mAppDataVector.push_back(
        AppData(title, className, address, focusHistoryID, isDuplicated));
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

void CAppsView::moveSelection(bool backwards) {
  if (mAppDataVector.empty())
    return;

  int totalItems = mAppDataVector.size();
  int currentIndex = -1;

  auto selectedList = this->get_selected_children();
  if (!selectedList.empty())
    currentIndex = selectedList[0]->get_index();

  int nextIndex = 0;

  if (currentIndex != -1) {
    if (backwards) {
      nextIndex = (currentIndex - 1) % totalItems;
    } else {
      nextIndex = (currentIndex + 1) % totalItems;
    }
  }

  auto nextChild = this->get_child_at_index(nextIndex);
  if (nextChild) {
    this->select_child(*nextChild);
    nextChild->grab_focus();
  }
}

} // namespace HyprTab
