#pragma once

// Enumerates all employee roles in the factory
// Used for access control and responsibility assignment
enum class Role {
  MANAGER,
  ENGINEER,
  TECHNICIAN,
  OPERATOR,
  QUALITY_INSPECTOR,
  WAREHOUSE_STAFF,
  MAINTENANCE_STAFF,
  SAFETY_OFFICER
};
