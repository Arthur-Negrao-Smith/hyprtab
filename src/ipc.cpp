#include <array>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "include/ipc.hpp"

namespace HyprTab {

CHyprlandChannel::CHyprlandChannel() { initPaths(); }

void CHyprlandChannel::initPaths() {
  const char *his = std::getenv(HYPRLAND_INSTANCE_SIGNATURE);
  const char *xdg = std::getenv(XDG_RUNTIME_DIR);

  if (his && xdg) {
    mSocketPath = std::string(xdg) + "/hypr/" + his + "/.socket.sock";
    return;
  }
}

std::string CHyprlandChannel::request(const std::string &command) const {
  if (mSocketPath.empty()) {
    std::cerr << "Erro: Hyprland variables did not find. "
              << "Are you running him?\n";

    return "";
  }

  int sock = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sock == -1) {
    std::cerr << "Erro: Fail to create a socket.\n";
    return "";
  }

  struct sockaddr_un address;
  address.sun_family = AF_UNIX;
  snprintf(address.sun_path, sizeof(address.sun_path), "%s",
           mSocketPath.c_str());

  // Hyprland connect
  if (connect(sock, (struct sockaddr *)&address, sizeof(address)) == -1) {
    std::cerr << "Erro: Fail to connect with Hyprland.\n";
    close(sock);
    return "";
  }

  if (write(sock, command.c_str(), command.length()) == -1) {
    std::cerr << "Erro: Fail to request the windows.\n";
    close(sock);
    return "";
  }

  std::string response = "";
  std::array<char, 4096> buffer; // read 4KB
  ssize_t readedBytes;

  while ((readedBytes = read(sock, buffer.data(), buffer.size() - 1)) > 0) {
    buffer[readedBytes] = '\0'; // Add null terminator
    response += buffer.data();
  }

  close(sock);

  return response;
}

} // namespace HyprTab
