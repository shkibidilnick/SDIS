#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "room.h"
#include "guest.h"
#include "employee.h"
#include "booking.h"
#include "amenity.h"
#include "report.h"
#include "exceptions.h"
#include <fstream>

class Hotel {
private:
    std::string name;
    std::vector<Room*> rooms;
    std::vector<Guest*> guests;
    std::vector<Employee*> staff;
    std::vector<Booking*> bookings;
    std::vector<Amenity*> amenities;
    std::unordered_map<std::string, Room*> roomIndex;
    std::unordered_map<std::string, Guest*> guestIndex;
    std::unordered_map<std::string, Employee*> employeeIndex;

public:
    explicit Hotel(std::string hotelName);
    ~Hotel();

    void addRoom(Room* room);
    void addGuest(Guest* guest);
    void addEmployee(Employee* employee);
    void addBooking(Booking* booking);
    void addAmenity(Amenity* amenity);

    Room* findRoom(const std::string& number) const;
    Guest* findGuest(const std::string& id) const;
    Employee* findEmployee(const std::string& id) const;
    Amenity* findAmenity(const std::string& id) const;

    bool isRoomAvailable(const std::string& roomNumber) const;
    void checkInGuest(const std::string& guestId, const std::string& roomNumber);
    void checkOutGuest(const std::string& roomNumber);
    Report* generateOccupancyReport(const std::string& reportId) const;
    double getTotalRevenue() const;
    int getAvailableRoomCount() const;

    int getTotalRooms() const { return static_cast<int>(rooms.size()); }
    int getTotalGuests() const { return static_cast<int>(guests.size()); }
    int getTotalBookings() const { return static_cast<int>(bookings.size()); }

    std::string getName() const { return name; }
};
