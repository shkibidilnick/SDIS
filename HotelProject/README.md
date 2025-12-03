| Entity | Fields | Behaviors | Associations |
|--------|--------|-----------|-------------|
| Amenity | 3 | 5 | → Room |
| Analytics | 3 | 5 | → Report, Booking |
| Booking | 7 | 11 | → Guest, Room, Payment |
| ChannelManager | 4 | 4 | → Booking, RatePlan |
| Commission | 5 | 4 | → Booking, Payment |
| Concierge | 4 | 4 | → Guest, Transportation, Event |
| Deposit | 5 | 5 | → Booking, Payment |
| Employee | 5 | 7 | |
| Event | 5 | 5 | → Guest |
| Facility | 5 | 5 | → Service, Employee |
| Feedback | 5 | 4 | → Guest, Employee |
| FrontDesk | 3 | 4 | → Employee, Booking |
| Guest | 7 | 11 | |
| Hotel | 9 | 22 | → Room, Guest, Employee, Booking, Amenity |
| Housekeeping | 4 | 4 | → Room, Employee |
| Inventory | 5 | 7 | |
| Invoice | 9 | 10 | → Booking, Tax, Penalty, Payment |
| Keycard | 4 | 5 | → Room, Guest |
| LostFound | 4 | 5 | → Guest |
| LoyaltyProgram | 3 | 5 | → Guest |
| Maintenance | 4 | 4 | → Room, Employee |
| MenuItem | 4 | 4 | |
| NightAudit | 3 | 5 | |
| Notification | 6 | 6 | → Guest |
| Order | 5 | 7 | → Guest, MenuItem |
| Package | 5 | 5 | → Service, Guest |
| Parking | 5 | 5 | → Vehicle, Guest |
| Payment | 5 | 7 | |
| Penalty | 8 | 7 | → Booking, Invoice, Payment |
| RatePlan | 5 | 5 | → Room |
| Refund | 5 | 4 | → Payment, Booking |
| Report | 5 | 6 | → Booking, Payment |
| Restaurant | 4 | 5 | → MenuItem, Order |
| Review | 5 | 5 | → Guest |
| Room | 5 | 8 | |
| Security | 3 | 5 | → Employee |
| Service | 5 | 8 | |
| SpecialOffer | 6 | 7 | → Guest, Room |
| Tax | 7 | 6 | → Invoice, Payment |
| Transportation | 6 | 6 | → Guest |
| Upgrade | 5 | 4 | → Booking, Room |
| Vehicle | 3 | 3 | |
| Vendor | 4 | 5 | → Inventory, Maintenance |
| Voucher | 4 | 4 | → Guest |
| WaitList | 4 | 4 | → Guest, Room |
| IPricable | 0 | 1 | |
| IBookable | 0 | 2 | |
| IPayable | 0 | 2 | |
| BookingStatus | 0 | 0 | |
| Role | 0 | 0 | |
| NotificationType | 0 | 0 | |
| PaymentMethod | 0 | 0 | |
| ReportType | 0 | 0 | |
| ServiceType | 0 | 0 | |
| ServiceStatus | 0 | 0 | |
| RateType | 0 | 0 | |
| PenaltyType | 0 | 0 | |
| TaxType | 0 | 0 | |
| TransportType | 0 | 0 | |
| VoucherStatus | 0 | 0 | |
| RoomType | 0 | 0 | |

## Итоговая статистика

- **Всего классов:** 58 
- **Поля:** 202
- **Поведения:** 272
- **Ассоциации:** 70
