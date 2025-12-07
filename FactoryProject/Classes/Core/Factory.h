#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "../Finance/Budget.h"
#include "../Inventory/Warehouse.h"
#include "../Production/ProductionLine.h"
#include "../Safety/SafetyIncident.h"
#include "../Shipping/DeliveryOrder.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/FactoryException.h"
#include "Department.h"
#include "Employee.h"

// Main factory management system
// Central aggregator for all factory operations and modules
class Factory {
 private:
  std::string factoryId;
  std::string factoryName;

  // Core modules
  std::vector<std::shared_ptr<Department>> departments;
  std::vector<std::shared_ptr<Employee>> employees;
  std::vector<std::shared_ptr<ProductionLine>> productionLines;

  // Inventory modules
  std::vector<std::shared_ptr<Warehouse>> warehouses;

  // Finance modules
  std::vector<std::shared_ptr<Budget>> budgets;

  // Safety modules
  std::vector<std::shared_ptr<SafetyIncident>> safetyIncidents;

  // Shipping modules
  std::vector<std::shared_ptr<DeliveryOrder>> deliveryOrders;

  // Index maps for fast lookup
  std::unordered_map<std::string, std::shared_ptr<Employee>> employeeIndex;
  std::unordered_map<std::string, std::shared_ptr<Department>> departmentIndex;
  std::unordered_map<std::string, std::shared_ptr<ProductionLine>> lineIndex;

 public:
  Factory(const std::string& name);
  ~Factory();

  // Core operations
  void addDepartment(std::shared_ptr<Department> dept);
  void addEmployee(std::shared_ptr<Employee> emp);
  void addProductionLine(std::shared_ptr<ProductionLine> line);

  // Inventory operations
  void addWarehouse(std::shared_ptr<Warehouse> warehouse);

  // Finance operations
  void addBudget(std::shared_ptr<Budget> budget);

  // Safety operations
  void addSafetyIncident(std::shared_ptr<SafetyIncident> incident);

  // Shipping operations
  void addDeliveryOrder(std::shared_ptr<DeliveryOrder> order);

  // Lookup methods
  std::shared_ptr<Employee> findEmployee(const std::string& employeeId) const;
  std::shared_ptr<Department> findDepartment(const std::string& deptId) const;
  std::shared_ptr<ProductionLine> findProductionLine(
      const std::string& lineId) const;
  std::shared_ptr<Warehouse> findWarehouse(
      const std::string& warehouseId) const;

  // Reporting
  std::string generateFactoryReport() const;

  // Operations
  void assignEmployeeToDepartment(const std::string& empId,
                                  const std::string& deptId);

  // Get aggregate statistics
  size_t getTotalEmployeeCount() const { return employees.size(); }
  size_t getTotalDepartmentCount() const { return departments.size(); }
  size_t getTotalIncidentCount() const { return safetyIncidents.size(); }
  double getTotalBudget() const;
  double getTotalInventoryValue() const;

  std::string getFactoryId() const { return factoryId; }
  std::string getFactoryName() const { return factoryName; }
};
