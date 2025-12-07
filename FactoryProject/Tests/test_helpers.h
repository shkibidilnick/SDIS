#pragma once
#include <iostream>
#include <ostream>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Core/Employee.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Core/Role.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Production/Machine.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Production/Product.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Quality/QualityCheck.h"

inline std::ostream& operator<<(std::ostream& os, const Role& role) {
    switch (role) {
    case Role::MANAGER:
        return os << "MANAGER";
    case Role::ENGINEER:
        return os << "ENGINEER";
    case Role::TECHNICIAN:
        return os << "TECHNICIAN";
    case Role::OPERATOR:
        return os << "OPERATOR";
    case Role::QUALITY_INSPECTOR:
        return os << "QUALITY_INSPECTOR";
    case Role::WAREHOUSE_STAFF:
        return os << "WAREHOUSE_STAFF";
    case Role::MAINTENANCE_STAFF:
        return os << "MAINTENANCE_STAFF";
    case Role::SAFETY_OFFICER:
        return os << "SAFETY_OFFICER";
    default:
        return os << "UNKNOWN";
    }
}
