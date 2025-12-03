#pragma once

class IPricable {
public:
	virtual double getPrice() const = 0;
	virtual ~IPricable() = default;
};

class IBookable {
 public:
  virtual bool book() = 0;
  virtual void cancel() = 0;
  virtual ~IBookable() = default;
};

class IPayable {
 public:
  virtual void processPayment() = 0;
  virtual void refund() = 0;
  virtual ~IPayable() = default;
};
