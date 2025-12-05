#include "Factory.h"

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/IDGenerator.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Logger.h"

Factory::Factory(const std::string& name) : factoryName(name) {
  factoryId = IDGenerator::generate("FAC");

  if (name.empty()) {
    throw FactoryException("Factory name cannot be empty");
  }

  Logger::getInstance().log("Factory",
                            "Created factory " + factoryId + " - " + name);
}

Factory::~Factory() {
  Logger::getInstance().log("Factory", "Shutting down factory " + factoryId);
}

void Factory::addDepartment(std::shared_ptr<Department> dept) {
  if (!dept) {
    throw FactoryException("Cannot add null department");
  }
  departments.push_back(dept);
  departmentIndex[dept->getDepartmentId()] = dept;

  Logger::getInstance().log("Factory", "Added department " +
                                           dept->getDepartmentId() +
                                           " to factory " + factoryId);
}

void Factory::addEmployee(std::shared_ptr<Employee> emp) {
  if (!emp) {
    throw FactoryException("Cannot add null employee");
  }
  employees.push_back(emp);
  employeeIndex[emp->getEmployeeId()] = emp;

  Logger::getInstance().log(
      "Factory",
      "Added employee " + emp->getEmployeeId() + " to factory " + factoryId);
}

void Factory::addProductionLine(std::shared_ptr<ProductionLine> line) {
  if (!line) {
    throw FactoryException("Cannot add null production line");
  }
  productionLines.push_back(line);
  lineIndex[line->getLineId()] = line;

  Logger::getInstance().log("Factory", "Added production line " +
                                           line->getLineId() + " to factory " +
                                           factoryId);
}

void Factory::addWarehouse(std::shared_ptr<Warehouse> warehouse) {
  if (!warehouse) {
    throw FactoryException("Cannot add null warehouse");
  }
  warehouses.push_back(warehouse);

  Logger::getInstance().log(
      "Factory",
      "Added warehouse " + warehouse->getName() + " to factory " + factoryId);
}

void Factory::addBudget(std::shared_ptr<Budget> budget) {
  if (!budget) {
    throw FactoryException("Cannot add null budget");
  }
  budgets.push_back(budget);

  Logger::getInstance().log(
      "Factory",
      "Added budget " + budget->getTrackingId() + " to factory " + factoryId);
}

void Factory::addSafetyIncident(std::shared_ptr<SafetyIncident> incident) {
  if (!incident) {
    throw FactoryException("Cannot add null safety incident");
  }
  safetyIncidents.push_back(incident);

  Logger::getInstance().log("Factory", "Added safety incident " +
                                           incident->getIncidentId() +
                                           " to factory " + factoryId);
}

void Factory::addDeliveryOrder(std::shared_ptr<DeliveryOrder> order) {
  if (!order) {
    throw FactoryException("Cannot add null delivery order");
  }
  deliveryOrders.push_back(order);

  Logger::getInstance().log("Factory", "Added delivery order " +
                                           order->getDeliveryOrderId() +
                                           " to factory " + factoryId);
}

std::shared_ptr<Employee> Factory::findEmployee(
    const std::string& empId) const {
  auto it = employeeIndex.find(empId);
  return (it != employeeIndex.end()) ? it->second : nullptr;
}

std::shared_ptr<Department> Factory::findDepartment(
    const std::string& deptId) const {
  auto it = departmentIndex.find(deptId);
  return (it != departmentIndex.end()) ? it->second : nullptr;
}

std::shared_ptr<ProductionLine> Factory::findProductionLine(
    const std::string& lineId) const {
  auto it = lineIndex.find(lineId);
  return (it != lineIndex.end()) ? it->second : nullptr;
}

std::shared_ptr<Warehouse> Factory::findWarehouse(
    const std::string& warehouseId) const {
  for (const auto& wh : warehouses) {
    if (wh->getWarehouseId() == warehouseId) {
      return wh;
    }
  }
  return nullptr;
}

void Factory::assignEmployeeToDepartment(const std::string& empId,
                                         const std::string& deptId) {
  auto emp = findEmployee(empId);
  auto dept = findDepartment(deptId);

  if (!emp) {
    throw FactoryException("Employee not found: " + empId);
  }

  if (!dept) {
    throw FactoryException("Department not found: " + deptId);
  }

  dept->addEmployee(emp);

  Logger::getInstance().log(
      "Factory", "Assigned employee " + empId + " to department " + deptId);
}

double Factory::getTotalBudget() const {
  double total = 0.0;
  for (const auto& budget : budgets) {
    total += budget->getTotalBudgetAmount();
  }
  return total;
}

double Factory::getTotalInventoryValue() const {
  double total = 0.0;
  for (const auto& warehouse : warehouses) {
    total += warehouse->getTotalInventoryValue();
  }
  return total;
}

std::string Factory::generateFactoryReport() const {
  std::string report = "FACTORY REPORT: " + factoryName + "\n";
  report += "ID: " + factoryId + "\n";
  report += "=======================================\n\n";

  report += "Departments: " + std::to_string(departments.size()) + "\n";
  report += "Employees: " + std::to_string(employees.size()) + "\n";
  report +=
      "Production Lines: " + std::to_string(productionLines.size()) + "\n";
  report += "Warehouses: " + std::to_string(warehouses.size()) + "\n";
  report += "Total Budget: $" + std::to_string(getTotalBudget()) + "\n";
  report +=
      "Inventory Value: $" + std::to_string(getTotalInventoryValue()) + "\n";
  report +=
      "Safety Incidents: " + std::to_string(safetyIncidents.size()) + "\n";
  report += "Delivery Orders: " + std::to_string(deliveryOrders.size()) + "\n";

  return report;
}
