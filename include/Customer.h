#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Workout.h"
#include <algorithm>

typedef std::pair<int, int> ID_PRICE;

class Customer{
public:
    Customer(std::string c_name, int c_id);
    virtual ~Customer();
    Customer(const Customer &other);
    Customer(Customer &&other) noexcept ;
    Customer& operator=(const Customer& other);
    Customer& operator=(Customer &&other) noexcept ;
    virtual std::vector<int> order(const std::vector<Workout> &workout_options)=0;
    virtual std::string toString() const = 0;
    virtual Customer* clone()=0;
    std::string getName() const;
    int getId() const;
private:
    const std::string name;
    const int id;
};


class SweatyCustomer : public Customer {
public:
	SweatyCustomer(std::string name, int id);
    ~SweatyCustomer() override;
    SweatyCustomer(const SweatyCustomer &other);

    __attribute__((unused)) SweatyCustomer(SweatyCustomer &&other) noexcept ;
    SweatyCustomer& operator=(const SweatyCustomer& other);
    SweatyCustomer& operator=(SweatyCustomer &&other) noexcept ;
    std::vector<int> order(const std::vector<Workout> &workout_options) override;
    SweatyCustomer* clone() override;
    std::string toString() const override;
private:
};


class CheapCustomer : public Customer {
public:
	CheapCustomer(std::string name, int id);
    ~CheapCustomer() override;
    CheapCustomer(const CheapCustomer &other);
    CheapCustomer(CheapCustomer &&other) noexcept ;
    CheapCustomer& operator=(const CheapCustomer& other) ;
    CheapCustomer& operator=(CheapCustomer &&other) noexcept;
    std::vector<int> order(const std::vector<Workout> &workout_options) override;
    CheapCustomer* clone() override;
    std::string toString() const override;
private:
};


class HeavyMuscleCustomer : public Customer {
public:
	HeavyMuscleCustomer(std::string name, int id);
    ~HeavyMuscleCustomer() override;
    HeavyMuscleCustomer(const HeavyMuscleCustomer &other);
    HeavyMuscleCustomer(HeavyMuscleCustomer &&other) noexcept ;
    HeavyMuscleCustomer& operator=(const HeavyMuscleCustomer& other);
    HeavyMuscleCustomer& operator=(HeavyMuscleCustomer &&other) noexcept ;
    std::vector<int> order(const std::vector<Workout> &workout_options) override;
    HeavyMuscleCustomer* clone() override;
    std::string toString() const override;
private:
};


class FullBodyCustomer : public Customer {
public:
	FullBodyCustomer(std::string name, int id);
    ~FullBodyCustomer() override;
    FullBodyCustomer(const FullBodyCustomer &other);
    FullBodyCustomer(FullBodyCustomer &&other) noexcept ;
    FullBodyCustomer& operator=(const FullBodyCustomer& other);
    FullBodyCustomer& operator=(FullBodyCustomer &&other) noexcept;
    std::vector<int> order(const std::vector<Workout> &workout_options) override;
    FullBodyCustomer* clone() override;
    std::string toString() const override;
private:
};


#endif