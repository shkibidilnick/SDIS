| Entity | Fields | Behaviors | Associations |
|--------|--------|-----------|-------------|
| Department | 5 | 11 | → Employee |
| Employee | 6 | 10 | → Role |
| Factory | 12 | 22 | → Department, Employee, ProductionLine, Warehouse, Budget, SafetyIncident, DeliveryOrder |
| Schedule | 4 | 8 | → Shift, Department |
| Shift | 6 | 9 | → Employee |
| Budget | 6 | 11 | → Department |
| Invoice | 9 | 12 | → Customer |
| Payroll | 4 | 8 | → Employee |
| Transaction | 8 | 9 | → Account |
| InventoryItem | 6 | 12 | → Warehouse |
| RawMaterial | 4 | 7 | → InventoryItem, Supplier |
| Supplier | 7 | 12 | → RawMaterial |
| Warehouse | 5 | 14 | → InventoryItem |
| MaintenanceRequest | 9 | 15 | → Machine, Employee, SparePart |
| MaintenanceSchedule | 7 | 12 | → MaintenanceRequest |
| SparePart | 4 | 7 | → InventoryItem, Machine |
| AssemblyStep | 9 | 15 | → Machine, Employee |
| Machine | 9 | 14 | → Employee |
| Product | 9 | 13 | → RawMaterial |
| ProductionLine | 6 | 14 | → Machine |
| WorkOrder | 9 | 15 | → Product, ProductionLine, RawMaterial |
| Defect | 7 | 11 | → QualityCheck, Product |
| QualityCheck | 8 | 12 | → Product, QualityStandard, Employee |
| QualityStandard | 5 | 9 | → Criterion |
| TestProcedure | 6 | 11 | → QualityStandard |
| SafetyIncident | 10 | 11 | → Employee |
| SafetyTraining | 6 | 11 | → SafetyIncident, Employee |
| Carrier | 8 | 10 | → Service |
| DeliveryOrder | 10 | 14 | → Customer, Product, Carrier |
| Shipment | 9 | 15 | → DeliveryOrder |
| FactoryException | 1 | 1 | |
| FinanceException | 1 | 1 | → FactoryException |
| InventoryException | 1 | 1 | → FactoryException |
| MaintenanceException | 1 | 1 | → FactoryException |
| PayrollException | 1 | 1 | → FactoryException |
| ProductionException | 1 | 1 | → FactoryException |
| QualityException | 1 | 1 | → FactoryException |
| SafetyException | 1 | 1 | → FactoryException |
| ShippingException | 1 | 1 | → FactoryException |
| ComplianceException | 1 | 1 | → FactoryException |
| IReportable | 0 | 2 | |
| ITrackable | 0 | 2 | |
| IValidatable | 0 | 1 | |
| DateTimeUtils | 0 | 4 | |
| IDGenerator | 1 | 1 | |
| Logger | 2 | 4 | |
| Validator | 0 | 5 | |
| Role | 0 | 0 | |
| PurchaseOrder | 10 | 12 | → RawMaterial, Supplier |
| AuditLog | 3 | 12 | → LogEntry |

## Итоговая статистика

- **Поля:** 239
- **Поведения:** 402
- **Ассоциации:** 61
- **Всего классов:** 50 классов
