#pragma once

#include <string>

namespace HyprTab {

constexpr const char *HYPRLAND_INSTANCE_SIGNATURE =
    "HYPRLAND_INSTANCE_SIGNATURE";

constexpr const char *XDG_RUNTIME_DIR = "XDG_RUNTIME_DIR";

class CHyprlandChannel {
private:
  std::string mSocketPath;

  void initPaths();

public:
  CHyprlandChannel();
  ~CHyprlandChannel() = default;

  CHyprlandChannel(const CHyprlandChannel &) = delete;
  CHyprlandChannel &operator=(const CHyprlandChannel &) = delete;

  std::string request(const std::string &command) const;
};

} // namespace HyprTab
