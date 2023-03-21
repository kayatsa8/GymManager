# include "../include/Trainer.h"

Trainer::Trainer(int t_capacity):capacity(t_capacity), freeSpots(t_capacity), open(false), customersList(), orderList(), salary(0){

}

int Trainer::getCapacity() const {
    return capacity;
}

void Trainer::addCustomer(Customer *customer) {
    if(!open | (freeSpots == 0) | (customer == nullptr) || (containsCustomer(customer->getId()))){
        throw std::exception();
    }

    customersList.push_back(customer);
    freeSpots--;
}

void Trainer::removeCustomer(int id) {

    if(!open | !containsCustomer(id)){
        throw std::exception();
    }

    std::vector<Customer*> temp = std::vector<Customer*>();

    for(unsigned int i=0; i<customersList.size(); i++){
        if(customersList[i]->getId() != id){
            temp.push_back(customersList[i]);
        }
    }

    std::swap(customersList, temp);

    std::vector<OrderPair> temp2 = std::vector<OrderPair>();

    for(unsigned int i=0; i<orderList.size(); i++){
        if(orderList.at(i).first != id){
            temp2.push_back(orderList[i]);
        }
    }

    std::swap(orderList, temp2);

    if(customersList.size() == 0){
        closeTrainer();
    }

    freeSpots++;

}

Customer* Trainer::getCustomer(int id) {
    if(customersList.empty()){
        return nullptr;
    }

    for(unsigned int i=0; i<customersList.size(); i++){
        if(customersList[i]->getId()==id){
            return customersList[i];
        }
    }

    return nullptr;
}

std::vector<Customer*>& Trainer::getCustomers() {
    std::vector<Customer*>& vec = customersList;
    return vec;
}

std::vector<OrderPair>& Trainer::getOrders(){
    std::vector<OrderPair>& vec = orderList;
    return vec;
}

void Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options){
    if(!open | !containsCustomer(customer_id)){
        throw std::exception();
    }

    bool found;
    for(unsigned int i=0; i<workout_ids.size(); i++){
        found = false;
        for(unsigned int j=0; (j<workout_options.size()) & (!found); j++){
            if(workout_ids[i]==workout_options[j].getId()){
                orderList.push_back(OrderPair (customer_id, Workout(workout_options[j])));
                found = true;
            }
        }
    }
}

void Trainer::openTrainer() {
    if(open){
        throw std::exception();
    }

    open = true;
}

void Trainer::closeTrainer() {
    if(!open){
        throw std::exception();
    }

    salary += getSalary();

    deleteCustomers();
    deleteOrders();

    freeSpots = capacity;
    open = false;

}

int Trainer::getCurrentSalary() {
    return salary;
}

void Trainer::deleteCustomers(){
    for(unsigned int i=0; i<customersList.size(); i++){
        delete customersList[i];
        customersList[i]= nullptr;
    }

    customersList.clear();
}

void Trainer::deleteOrders() {
    orderList.clear();
}

int Trainer::getSalary() {
    int salary = 0;

    for(unsigned int i=0; i<orderList.size(); i++){
        salary += orderList[i].second.getPrice();
    }

    return salary;
}

bool Trainer::isOpen() {
    return open;
}

int Trainer::getFreeSpots() {
    return  freeSpots;
}

Trainer::~Trainer(){
    deleteCustomers();
    deleteOrders();
}

Trainer::Trainer(const Trainer &other):capacity(other.capacity), freeSpots(other.freeSpots), open(other.open), customersList(), orderList(), salary(other.salary) {
    capacity = other.capacity;
    freeSpots = other.freeSpots;
    open = other.open;

    for(unsigned int i=0; i<other.customersList.size(); i++){
        customersList.push_back(customersList[i]->clone());
    }

    for(unsigned int i=0; i<other.orderList.size(); i++){
        orderList.push_back(OrderPair(other.orderList[i].first, other.orderList[i].second));
    }
}

Trainer::Trainer(Trainer &&other):capacity(other.capacity), freeSpots(other.freeSpots), open(other.open), customersList(), orderList(), salary(other.salary){

    for(unsigned int i=0; i<other.customersList.size(); i++){
        customersList.push_back(other.customersList[i]);
        other.customersList[i] = nullptr;
    }

    other.customersList.clear();

    for(unsigned int i=0; i<other.orderList.size(); i++){
        orderList.push_back(OrderPair(other.orderList[i].first, other.orderList[i].second));
    }

    other.orderList.clear();

}

Trainer& Trainer::operator=(const Trainer& other){
    if(&other == this){
        return *this;
    }

    deleteCustomers();
    deleteOrders();

    capacity = other.capacity;
    freeSpots = other.freeSpots;
    open = other.open;

    for(unsigned int i=0; i<other.customersList.size(); i++){
        customersList.push_back((other.customersList)[i]->clone());
    }

    for(unsigned int i=0; i<other.orderList.size(); i++){
        orderList.push_back((OrderPair(other.orderList[i].first, other.orderList[i].second)));
    }

    return *this;
}

Trainer& Trainer::operator=(Trainer &&other){

    if(&other == this){
        return *this;
    }

    deleteCustomers();
    deleteOrders();

    capacity = other.capacity;
    freeSpots = other.freeSpots;
    open = other.open;

    for(unsigned int i=0; i<other.customersList.size(); i++){
        customersList.push_back((other.customersList[i]));
        other.customersList[i] = nullptr;
    }

    other.customersList.clear();

    for(unsigned int i=0; i<other.orderList.size(); i++){
        orderList.push_back(OrderPair(other.orderList[i].first, other.orderList[i].second));
    }

    other.orderList.clear();

    return *this;
}

bool Trainer::containsCustomer(int id) {
    for(unsigned int i=0; i<customersList.size(); i++){
        if(customersList[i]->getId() == id){
            return true;
        }
    }

    return false;
}