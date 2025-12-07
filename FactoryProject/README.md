| Entity | Fields | Behaviors | Associations |
|--------|--------|-----------|-------------|
| Department | 5 | 11 | → Employee |
| Employee | 6 | 10 | → Role |
| Factory | 12 | 31 | → Department, Employee, ProductionLine, Warehouse, Budget, SafetyIncident, DeliveryOrder |
| Schedule | 4 | 8 | → Shift, Department |
| Shift | 6 | 9 | → Employee |
| Budget | 6 | 12 | → Department |
| Invoice | 9 | 13 | → Customer |
| Payroll | 4 | 9 | → Employee |
| Transaction | 9 | 10 | |
| InventoryItem | 6 | 12 | → Warehouse |
| RawMaterial | 4 | 7 | → InventoryItem, Supplier |
| Supplier | 7 | 12 | → RawMaterial |
| Warehouse | 5 | 15 | → InventoryItem |
| MaintenanceRequest | 10 | 16 | → Machine, Employee, SparePart |
| MaintenanceSchedule | 8 | 16 | → MaintenanceRequest |
| SparePart | 4 | 7 | → InventoryItem, Machine |
| AssemblyStep | 9 | 15 | → Machine, Employee |
| Machine | 9 | 14 | → Employee |
| Product | 9 | 14 | → RawMaterial |
| ProductionLine | 6 | 14 | → Machine |
| WorkOrder | 9 | 16 | → Product, ProductionLine, RawMaterial |
| Defect | 7 | 11 | → Product |
| QualityCheck | 8 | 13 | → Product, QualityStandard, Employee |
| QualityStandard | 5 | 10 | → Criterion |
| TestProcedure | 6 | 12 | → QualityStandard |
| SafetyIncident | 11 | 11 | → Employee |
| SafetyTraining | 7 | 18 | → Employee |
| Carrier | 8 | 10 | → Service |
| DeliveryOrder | 11 | 15 | → Customer, Product, Carrier |
| Shipment | 10 | 17 | → DeliveryOrder |
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
| Logger | 2 | 5 | |
| Validator | 0 | 5 | |
| Role | 0 | 0 | |
| PurchaseOrder | 11 | 17 | → RawMaterial, Supplier |
| AuditLog | 3 | 12 | → LogEntry |

## Итоговая статистика

- **Поля:** 248
- **Поведения:** 447
- **Ассоциации:** 59
- **Всего классов:** 50 классов
