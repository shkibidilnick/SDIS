#include "notification.h"

Notification::Notification(std::string id, Guest* guest, std::string msg,
                           NotificationType nt)
    : notificationId(std::move(id)),
      recipient(guest),
      message(std::move(msg)),
      type(nt),
      isSent(false),
      isRead(false) {}
void Notification::send() {
  if (isSent) {
    return;
  }
  std::ofstream logFile("notifications.log", std::ios::app);
  if (logFile.is_open()) {
    logFile << "[Notification ID: " << notificationId << "]\n"
            << "  Type: "
            << (type == NotificationType::EMAIL ? "Email"
                : type == NotificationType::SMS ? "SMS"
                                                : "PUSH")
            << "\n"
            << "  To Guest: " << (recipient ? recipient->getId() : "Unknown")
            << "\n"
            << "  Message: " << message << "\n"
            << "  Status: SENT\n";
    logFile.close();
  }
  isSent = true;
}
void Notification::markAsRead() {
  isRead = true;
  std::ofstream logFile("notifications.log", std::ios::app);
  if (logFile.is_open()) {
    logFile << "[Notification ID: " << notificationId << "]\n"
            << "  Status: READ by guest\n";
    logFile.close();
  }
}
