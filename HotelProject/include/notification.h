#pragma once
#include <string>
#include <fstream>
#include "guest.h"

enum class NotificationType{EMAIL, SMS, PUSH};

class Notification {
private:
	std::string notificationId;
	Guest* recipient; //recipient - получатель
	std::string message;
	NotificationType type;
	bool isSent;
	bool isRead;
public:
	Notification(std::string id, Guest* guest, std::string msg, NotificationType nt);
	// Simulates sending notification by logging to a file
	void send();
	void markAsRead();
	bool getIsSent() const { return isSent; }
	bool getIsRead() const { return isRead; }
	std::string getNotificationId() const { return notificationId; }
};