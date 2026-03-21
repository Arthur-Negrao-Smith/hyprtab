#include <iostream>
#include <string>

#include "include/ipc.hpp"

int main() {
  const HyprTab::CHyprlandChannel channel;

  std::string json = channel.request("j/clients");
  std::cout << "Resposta do Hyprland:\n" << json << std::endl;
  return 0;
}
