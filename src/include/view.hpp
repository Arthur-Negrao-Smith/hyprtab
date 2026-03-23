#pragma once

#include <gtkmm/flowbox.h>
#include <gtkmm/liststore.h>
#include <vector>

#include "icon.hpp"
#include "ipc.hpp"
#include "json.hpp"

namespace HyprTab {

using json = nlohmann::json;

constexpr int MAX_CHILDRENS_PER_LINE = 5;

class CAppsView : public Gtk::FlowBox {
private:
  std::vector<AppData> mAppDataVector;
  CHyprlandChannel mChannel;

public:
  CAppsView();

  void setIcons(const json &icons);
};

} // namespace HyprTab
