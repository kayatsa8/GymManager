#include "../include/Studio.h"


Studio::Studio():open(false),trainers(),workout_options(),actionsLog(), customerID(0){

}

Studio::Studio(const std::string  &configFilePath):open(false), trainers(), workout_options(), actionsLog(), customerID(0){
    std::ifstream file(configFilePath);
    std::string line;

    std::vector<std::string> reader;

    while(getline(file, line)){
        if((line[0] - '#' != 0) & (line.length() > 0)){
            reader.push_back(line);
        }
    }

    createTrainers(reader);
    createWorkoutOptions(reader);

}

void Studio::createTrainers(std::vector<std::string>& reader) {
    int numOfTrainers = std::stoi(reader[0]);

    int capacity;
    unsigned int index = 0;

    for(int i=0; i<numOfTrainers; i++){
        capacity = 0;
        while((reader[1].length() > index) && (reader[1][index] - '0' >= 0) && (reader[1][index] - '9' <= 9)){
            capacity = capacity*10 + reader[1][index] - '0';
            index++;
        }

        index++;
        trainers.push_back(new Trainer (capacity));
    }


}

void Studio::createWorkoutOptions(std::vector<std::string> &reader) {

    std::string name;
    std::string type;
    int price;
    int index;
    int prevIndex;

    for(unsigned  int i=2; i<reader.size(); i++){ // i=0 --> number of trainers; i=1 --> trainers' capacities
       price = 0;
       index = 0;
       prevIndex = 0;
       while(reader[i][index] - ',' != 0){ // read workout's name
           index++;
       }
       name = reader[i].substr(prevIndex, index-prevIndex);
       index+=2;
       prevIndex = index;

        while(reader[i][index] - ',' != 0){ // read workout's type
            index++;
        }
        type = reader[i].substr(prevIndex, index-prevIndex);
        index+=2;
        prevIndex = index;

        while((int)reader[i].length() > index){ // read workout's price
            price = price*10 + reader[i][index] - '0';
            index++;
        }

        // determine the workout's type and insert a new workout
        if(type == "Anaerobic"){
            workout_options.push_back(Workout((int)i-2, name, price, ANAEROBIC));
        }
        if(type == "Mixed"){
            workout_options.push_back(Workout((int)i-2, name, price, MIXED));
        }
        if(type == "Cardio"){
            workout_options.push_back(Workout((int)i-2, name, price, CARDIO));
        }

    }
}



/*Studio::Studio(const std::string &configFilePath){
    std::string line;
    std::string text;

    int numOfTrainers;

    bool gotNumOfTrainers = false;
    bool gotCapacities = false;
    int counterID=0;

    this->open = false;

    std::ifstream MyreadFile(configFilePath);
    while(getLine(&:MyReadFile,&line)){
        if(!line[0].equals("#") | line.length!=0)
        {
            if(gotNumOfTrainers==false)
            {
                numOfTrainers=(line[0]) - '0';
                gotNumOfTrainers=true;
            }
            else
            {
                if(gotCapacities==false)
                {
                    Vector<int> capacities;

                    int i = 0;
                    while(i<line.length()) {

                        string number = "";
                        if (!line[i].equals(",")) {
                            number += line[i];
                        }
                        else
                        {
                            this.trainers.push_back(new trainer(stoi(number)));
                            number="";
                        }
                        i++;

                    }
                    gotCapacities=true;
                }
                else
                {
                    string name="";
                    string type="";
                    string price="";

                    bool gotName = false;
                    bool gotType = false;


                    int i = 0;
                    while(i<line.length()) {

                        if (!line[i].equals(","))
                        {
                            if(!gotName)
                            {
                                name+=line[i];
                            }
                            else
                            {
                                if(!gotType)
                                {
                                    type+=line[i];
                                }
                                else
                                {
                                    price+=line[i];
                                }
                            }


                        }
                        else
                        {
                            if(gotName==false)
                            {
                                gotName=true;
                            }
                            else
                            {
                                gotType=true;
                            }
                            i++;
                        }
                        i++;

                    }

                    if(type=="Anaerobic")
                    {
                        this.workout_options.push_back(Workout(counterID, name, stoi(price), ANAEROBIC));
                    }
                    else if(type=="Mixed")
                    {
                        this.workout_options.push_back(Workout(counterID, name, stoi(price), MIXED));
                    }
                    else
                    {
                        this.workout_options.push_back(Workout(counterID, name, stoi(price), CARDIO));
                    }
                    counterID++;


                }
            }
        }
    }
}*/

void Studio::start(){
    this->open = true;
    std::cout<<"Studio is now open!"<<std::endl;

    callAction();
}

void Studio::callAction() {
    std::string action;
    BaseAction* baseAction;

    while(open){
        getline(std::cin, action);
        int scanner = 0;

        while((int)action.length() > scanner && action[scanner] - ' ' != 0){
            scanner++;
        }

        std::string command = action.substr(0, scanner);
        scanner++;

        if(command == "open"){

            int id = 0; // find trainer's id
            while(action[scanner] - ' ' != 0){
                id = id*10 + action[scanner] - '0';
                scanner++;
            }

            scanner++;
            std::vector<Customer*> customers;


            while(scanner < action.length()){
                std::string name = "";
                std::string strategy = "";

                // find name
                while(action[scanner] - ',' != 0){
                    name += action[scanner];
                    scanner++;
                }

                scanner++;

                // find strategy
                while((int)action.length() > scanner && action[scanner] - ' ' != 0){
                    strategy += action[scanner];
                    scanner++;
                }

                scanner++;

                // insert the customer to the vector
                if(strategy == "swt"){
                    customers.push_back(new SweatyCustomer(name, customerID));
                }
                if(strategy == "chp"){
                    customers.push_back(new CheapCustomer(name, customerID));
                }
                if(strategy == "mcl"){
                    customers.push_back(new HeavyMuscleCustomer(name, customerID));
                }
                if(strategy == "fbd"){
                    customers.push_back(new FullBodyCustomer(name, customerID));
                }
                customerID++;

            }

            baseAction = new OpenTrainer(id, customers);

            baseAction->act(*this);
        }

        if(command == "order"){
            int id = 0; // find trainer's id
            while((int)action.length() > scanner && action[scanner] - ' ' != 0){
                id = id*10 + action[scanner] - '0';
                scanner++;
            }

            baseAction = new Order(id);

            baseAction->act(*this);
        }

        if(command == "move"){
            int originTrainer = 0;
            int destTrainer = 0;
            int customerID = 0;

            while(action[scanner] - ' ' != 0){
                originTrainer = originTrainer*10 + action[scanner] - '0';
                scanner++;
            }

            scanner++;

            while(action[scanner] - ' ' != 0){
                destTrainer = destTrainer*10 + action[scanner] - '0';
                scanner++;
            }

            scanner++;

            while((int)action.length() > scanner){
                customerID = customerID*10 + action[scanner] - '0';
                scanner++;
            }

            baseAction = new MoveCustomer(originTrainer, destTrainer, customerID);

            baseAction->act(*this);
        }

        if(command == "close"){
            int trainerID = 0;
            while((int)action.length() > scanner){
                trainerID = trainerID*10 + action[scanner] - '0';
                scanner++;
            }

            baseAction = new Close(trainerID);
            baseAction->act(*this);
        }

        if(command == "closeall"){
            baseAction = new CloseAll();

            baseAction->act(*this);
        }

        if(command == "workout_options"){
            baseAction = new PrintWorkoutOptions();

            baseAction->act(*this);
        }

        if(command == "status"){
            int trainerID = 0;
            while((int)action.length() > scanner){
                trainerID = trainerID*10 + action[scanner] - '0';
                scanner++;
            }

            baseAction = new PrintTrainerStatus(trainerID);

            baseAction->act(*this);
        }

        if(command == "log"){
            baseAction = new PrintActionsLog();

            baseAction ->act(*this);
        }

        if(command == "backup"){
            baseAction = new BackupStudio();

            baseAction ->act(*this);
        }

        if(command == "restore"){
            baseAction = new RestoreStudio();

            baseAction ->act(*this);
        }

        //delete baseAction;
        baseAction = nullptr;

    }
}

int Studio::getNumOfTrainers() const{
    return this->trainers.size();
}

Trainer* Studio::getTrainer(int tid){
    try{
        return trainers.at(tid);
    }
    catch(std::exception e){
        return nullptr;
    }
}

const std::vector<BaseAction*>& Studio::getActionsLog() const{
    return this->actionsLog;
} // Return a reference to the history of actions

std::vector<Workout>& Studio::getWorkoutOptions(){
    return this->workout_options;
}

Studio& Studio::operator=(const Studio& s){
    if(&s == this){
        return *this;
    }

    open = s.open;

    deleteActionLog();
    deleteWorkoutOptions();
    deleteTrainers();

    for(Trainer* t : s.trainers){
        trainers.push_back(new Trainer(*t));
    }

    for(Workout w : s.workout_options){
        workout_options.push_back(w);
    }

    for(BaseAction* a : s.actionsLog){
        actionsLog.push_back(a->clone());
    }

    return *this;

}

void Studio::deleteTrainers() {
    for(unsigned int i=0; i<trainers.size();i++){
        delete trainers[i];
        trainers[i] = nullptr;
    }

    trainers.clear();
}

void Studio::deleteWorkoutOptions() {
    workout_options.clear();
}

void Studio::deleteActionLog() {
    for(unsigned int i=0; i<actionsLog.size();i++){
        delete actionsLog[i];
        actionsLog[i] = nullptr;
    }

    actionsLog.clear();
}

void Studio::setActionsLog(BaseAction* a){
    actionsLog.push_back(a);
}

std::vector<Trainer*>& Studio::getTrainers(){
    return trainers;
}

void Studio::close() {
    open = false;
}