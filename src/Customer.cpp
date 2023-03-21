#include "../include/Customer.h"

#include <utility>


// Customer
Customer::Customer(std::string c_name, int c_id):name(std::move(c_name)), id(c_id){

}

std::string Customer::getName() const {
    return name;
}

int Customer::getId() const {
    return id;
}

Customer::~Customer()= default;

Customer::Customer(const Customer &other) = default;

Customer::Customer(Customer &&other) noexcept:name(other.name), id(other.id){
    delete &other;
}

Customer& Customer::operator=(const Customer& other){

    if(&other == this){
        return *this;
    }

    // Cannot assign value to const property

    throw std::exception();
}

Customer& Customer::operator=(Customer &&other)noexcept{

    if(&other == this){
        return *this;
    }

    // Cannot assign value to const property

    return *this; // Must return something, can't return something else
}




// SweatyCustomer
SweatyCustomer::SweatyCustomer(std::string name, int id):Customer(std::move(name), id){

}

std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options){
    std::vector<int> myOrder;

    for(const auto & workout_option : workout_options){
        if(workout_option.getType() == CARDIO){
            myOrder.push_back(workout_option.getId());
        }
    }

    return myOrder;
}

std::string SweatyCustomer::toString() const {
    return getName() + ",swt";
}

SweatyCustomer::~SweatyCustomer() = default;

SweatyCustomer::SweatyCustomer(const SweatyCustomer &other) = default;

SweatyCustomer::SweatyCustomer(SweatyCustomer &&other) noexcept: Customer(other){

}

SweatyCustomer& SweatyCustomer::operator=(const SweatyCustomer& other)= default;

SweatyCustomer& SweatyCustomer::operator=(SweatyCustomer &&other)noexcept{
    Customer::operator=(other);
    return *this;
}

SweatyCustomer* SweatyCustomer::clone(){
    return new SweatyCustomer(getName(), getId());
}




// CheapCustomer
CheapCustomer::CheapCustomer(std::string name, int id): Customer(std::move(name), id) {

}

std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options){
    std::vector<int> myOrder;

    if(workout_options.size()==0){
        return myOrder;
    }

    int min = workout_options[0].getPrice(), minID = 0;

    for(unsigned int i=1; i<workout_options.size(); i++){
        if(workout_options[i].getPrice() < min){
            min = workout_options[i].getPrice();
            minID = workout_options[i].getId();
        }
    }

    myOrder.push_back(minID);

    return myOrder;
}

std::string CheapCustomer::toString() const{
    return getName() + ",chp";
}

CheapCustomer::~CheapCustomer()= default;

CheapCustomer::CheapCustomer(const CheapCustomer &other)= default;

CheapCustomer::CheapCustomer(CheapCustomer &&other) noexcept: Customer(other){

}

CheapCustomer& CheapCustomer::operator=(const CheapCustomer& other)= default;

CheapCustomer& CheapCustomer::operator=(CheapCustomer &&other) noexcept{
    Customer::operator=(other);
    return *this;
}

CheapCustomer* CheapCustomer::clone(){
    return new CheapCustomer(getName(), getId());
}




// HeavyMuscleCustomer
HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id): Customer(std::move(name), id){

}

std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options){
    std::vector<ID_PRICE> myOrderTemp;
    std::vector<int> myOrder;

    for(const auto & workout_option : workout_options){
        if(workout_option.getType() == ANAEROBIC){
            myOrderTemp.emplace_back(workout_option.getId(), workout_option.getPrice());
        }
    }

    std::sort(myOrderTemp.begin(), myOrderTemp.end(), [](ID_PRICE ip1, ID_PRICE ip2){
        return ip1.second > ip2.second;
    });

    for(auto & i : myOrderTemp){
        myOrder.push_back(i.first);
    }

    return myOrder;
}

std::string HeavyMuscleCustomer::toString() const{
    return getName() + ",mcl";
}

HeavyMuscleCustomer::~HeavyMuscleCustomer()= default;

HeavyMuscleCustomer::HeavyMuscleCustomer(const HeavyMuscleCustomer &other) = default;

HeavyMuscleCustomer::HeavyMuscleCustomer(HeavyMuscleCustomer &&other) noexcept: Customer(other){

}

HeavyMuscleCustomer& HeavyMuscleCustomer::operator=(const HeavyMuscleCustomer& other)= default;

HeavyMuscleCustomer& HeavyMuscleCustomer::operator=(HeavyMuscleCustomer &&other) noexcept{
    Customer::operator=(other);
    return *this;
}

HeavyMuscleCustomer* HeavyMuscleCustomer::clone(){
    return new HeavyMuscleCustomer(getName(), getId());
}




// FullBodyCustomer
FullBodyCustomer::FullBodyCustomer(std::string name, int id): Customer(std::move(name), id){

}

std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options){
    std::vector<int> myOrder;
    ID_PRICE ip_cardio(-1, -1);
    ID_PRICE  ip_mix(-1, -1);
    ID_PRICE ip_anaerobic(-1, -1);
    bool found_cardio = false;
    bool found_mix = false;
    bool found_anaerobic = false;

    for(unsigned int i=0; (i<workout_options.size()) & !(found_cardio & found_anaerobic & found_mix); i++){
        if((workout_options[i].getType() == CARDIO) & !found_cardio){
            ip_cardio.first = workout_options[i].getId();
            ip_cardio.second = workout_options[i].getPrice();
            found_cardio = true;
        }
        if((workout_options[i].getType() == ANAEROBIC) & !found_anaerobic){
            ip_anaerobic.first = workout_options[i].getId();
            ip_anaerobic.second = workout_options[i].getPrice();
            found_anaerobic = true;
        }
        if((workout_options[i].getType() == MIXED) & !found_mix){
            ip_mix.first = workout_options[i].getId();
            ip_mix.second = workout_options[i].getPrice();
            found_mix = true;
        }
    }

    if((ip_cardio.first == -1) | (ip_mix.first == -1) | (ip_anaerobic.first == -1)){ // means one of the required workouts weren't found
        return myOrder;
    }

    for(const auto & workout_option : workout_options){
        if((workout_option.getType() == CARDIO) & (workout_option.getPrice() < ip_cardio.second)){
            ip_cardio.first = workout_option.getId();
            ip_cardio.second = workout_option.getPrice();
        }
        if((workout_option.getType() == MIXED) & (workout_option.getPrice() > ip_mix.second)){
            ip_mix.first = workout_option.getId();
            ip_mix.second = workout_option.getPrice();
        }
        if((workout_option.getType() == ANAEROBIC) & (workout_option.getPrice() < ip_anaerobic.second)){
            ip_anaerobic.first = workout_option.getId();
            ip_anaerobic.second = workout_option.getPrice();
        }
    }

    myOrder.push_back(ip_cardio.first);
    myOrder.push_back(ip_mix.first);
    myOrder.push_back(ip_anaerobic.first);

    return myOrder;

}

std::string FullBodyCustomer::toString() const{
    return getName() + ",fbd";
}

FullBodyCustomer::~FullBodyCustomer()= default;

FullBodyCustomer::FullBodyCustomer(const FullBodyCustomer &other)= default;

FullBodyCustomer::FullBodyCustomer(FullBodyCustomer &&other) noexcept: Customer(other){

}

FullBodyCustomer& FullBodyCustomer::operator=(const FullBodyCustomer& other)= default;

FullBodyCustomer& FullBodyCustomer::operator=(FullBodyCustomer &&other) noexcept{
    Customer::operator=(other);
    return *this;
}

FullBodyCustomer* FullBodyCustomer::clone(){
    return new FullBodyCustomer(getName(), getId());
}
