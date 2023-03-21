# include "../include/Workout.h"

#include <utility>

Workout::Workout(int w_id, std::string w_name, int w_price, WorkoutType w_type):id(w_id), name(std::move(w_name)), price(w_price), type(w_type){

}

int Workout::getId() const{
    return id;
}

std::string Workout::getName() const{
    return name;
}

int Workout::getPrice() const{
    return price;
}

WorkoutType Workout::getType() const{
    return type;
}

Workout::~Workout()= default;

Workout::Workout(const Workout &other) = default;

Workout::Workout(const Workout &&other) noexcept:id(other.id), name(other.name), price(other.price), type(other.type){

}

Workout& Workout::operator=(const Workout& other){
    if(&other == this){
        return *this;
    }

    // Cannot assign value to const property

    throw std::exception();
}

Workout& Workout::operator=(Workout &&other)noexcept{
    if(&other == this){
        return *this;
    }



    // Cannot assign value to const property


    return *this; // Cannot assign, cannot throw exception according to compiler
}

std::string Workout::toString() {
    std::string s = name+" ";

    if(type == ANAEROBIC){
        s+= "Anaerobic";
    }
    if(type == MIXED){
        s+= "Mixed";
    }
    if(type == CARDIO){
        s+= "Cardio";
    }

    s+=" " + std::to_string(price);

    return s;
}