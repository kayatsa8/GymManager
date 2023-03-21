#include "../include/Action.h"
#include "../include/Studio.h"
using namespace std;

//Studio studioBackUp;
Studio* studioBackUp = new Studio();



//BaseAction
BaseAction::BaseAction():errorMsg(std::move("")), status(ERROR){

}

BaseAction::~BaseAction(){

}

void BaseAction::complete(){
    this->status = COMPLETED;
}

void BaseAction::error(std::string errorMsg){
    this->errorMsg = errorMsg;
}

std::string BaseAction::getErrorMsg() const{
    return this->errorMsg;
}

void BaseAction::setErrorMsg(std::string msg){
    errorMsg=msg;
}

void BaseAction::setStatus(ActionStatus status1){
    status=status1;
}

ActionStatus BaseAction::getStatus() const{
    return status;
}



//OpenTrainer
OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList):BaseAction(), trainerId(id), customers(customersList){

}

OpenTrainer::~OpenTrainer(){
    for(Customer* c :customers){
        delete c;
    }
}

void OpenTrainer::act(Studio &studio){
    Trainer* t1 = studio.getTrainer(this->trainerId);
    if(t1!=nullptr && !t1->isOpen()) {
        t1->openTrainer();

        for (Customer *customer: customers) {
            t1->addCustomer(customer);
        }
        complete();
        studio.setActionsLog(this);

    }
    else
    {
        setErrorMsg("Workout session does not exist or is already open");
        setStatus(ERROR);
    }




}

std::string OpenTrainer::toString() const{
    string output = "OpenTrainer "+std::to_string(trainerId);
    for(Customer* c : customers)
    {
        output+=c->toString()+" ";
    }
    return output;
}

BaseAction* OpenTrainer::clone() {

    vector<Customer*> vec;

    for(unsigned int i=0; i<customers.size(); i++){
        vec.push_back(customers[i]->clone());
    }

    return new OpenTrainer(this->trainerId, vec);
}



//Order
Order::Order(int id):BaseAction(), trainerId(id){

}

Order::~Order(){

}

void Order::act(Studio &studio){
    Trainer* t1 = studio.getTrainer(this->trainerId);
    if(t1!=nullptr && t1->isOpen()){
        std::vector<Customer*> customers = t1->getCustomers();
        for(Customer* c : customers){
            t1->order(c->getId(),
                      c->order(studio.getWorkoutOptions()),
                      studio.getWorkoutOptions());
        }


        for(OrderPair o: t1->getOrders())
        {
            cout<<t1->getCustomer(o.first)->getName()+" is doing "+o.second.getName()<<endl;
        }
        complete();
        studio.setActionsLog(this);

    }
    else{
        setErrorMsg("Trainer does not exist or is not open");
        setStatus(ERROR);
    }

}

std::string Order::toString() const{
    return "Order "+trainerId;
}

BaseAction* Order::clone() {
    return new Order(trainerId);
}



//MoveCustomer
MoveCustomer::MoveCustomer(int src, int dst,int customerId):BaseAction(), srcTrainer(src), dstTrainer(dst),id(customerId){

}

MoveCustomer::~MoveCustomer(){

}

void MoveCustomer::act(Studio &studio){
    Trainer* src = studio.getTrainer(this->srcTrainer);
    Trainer* dst = studio.getTrainer(this->dstTrainer);
    Customer* cst = src->getCustomer(this->id);
    if(src!=nullptr && dst!=nullptr && src->isOpen() && dst->isOpen() && src!=nullptr && dst->getFreeSpots()>0)
    {
        src->removeCustomer(id);
        dst->addCustomer(cst);
        complete();
        studio.setActionsLog(this);

    }
    else
    {
        setErrorMsg("Cannot move customer");
        setStatus(ERROR);
    }
}

std::string MoveCustomer::toString() const{
    return "MoveCustomer " + std::to_string(id) + " from " + std::to_string(dstTrainer)+" to " + std::to_string(srcTrainer);
}

BaseAction* MoveCustomer::clone() {
    return new MoveCustomer(srcTrainer, dstTrainer, id);
}



//Close
Close::Close(int id):BaseAction(), trainerId(id){

}
Close::~Close(){

}
void Close::act(Studio &studio){
    Trainer* t1 = studio.getTrainer(trainerId);

    if(t1!=nullptr && t1->isOpen())
    {
        t1->closeTrainer();
        cout<<"Trainer "+std::to_string(trainerId)+" closed. Salary "+std::to_string(t1->getCurrentSalary())+"NIS"<<endl;
        complete();
        studio.setActionsLog(this);

    }
    else
    {
        setErrorMsg("Trainer does not exist or is not open");
        setStatus(ERROR);
    }
}
std::string Close::toString() const{
    return "Close "+std::to_string(trainerId);
}
BaseAction* Close::clone() {
    return new Close(trainerId);
}



//CloseAll
CloseAll::CloseAll():BaseAction(){

}

CloseAll::~CloseAll(){

}

void CloseAll::act(Studio &studio){
    vector<Trainer*> trainers = studio.getTrainers();
    int id = 0;
    for(Trainer* t : trainers)
    {
        if(t->isOpen()){
            t->closeTrainer();
            cout<<"Trainer "+std::to_string(id)+" closed. Salary "+std::to_string(t->getCurrentSalary())+"NIS"<<endl;
        }
        id++;
    }
    studio.close();
    complete();
    studio.setActionsLog(this);

}

std::string CloseAll::toString() const{
    return "CloseAll";
}

BaseAction* CloseAll::clone() {
    return new CloseAll();
}



//PrintWorkoutOptions
PrintWorkoutOptions::PrintWorkoutOptions():BaseAction() {

}

PrintWorkoutOptions::~PrintWorkoutOptions() {

}

void PrintWorkoutOptions::act(Studio &studio){
    for(Workout  w : studio.getWorkoutOptions()){
        cout<<w.toString()<<endl;
    }
    complete();
    studio.setActionsLog(this);
}

std::string PrintWorkoutOptions::toString() const{
    return "PrintWorkoutOptions";
}

BaseAction* PrintWorkoutOptions::clone() {
    return new PrintWorkoutOptions();
}



//PrintTrainerStatus
PrintTrainerStatus::PrintTrainerStatus(int id):BaseAction(),trainerId(id){

}
PrintTrainerStatus::~PrintTrainerStatus(){

}
void PrintTrainerStatus::act(Studio &studio) {
    if(studio.getTrainer(trainerId)->isOpen())
    {
        cout << "Trainer " + std::to_string(trainerId) + " status: open"<< endl;
        for (Customer *c: studio.getTrainer(trainerId)->getCustomers()) {
            cout << to_string(c->getId()) + " " + c->getName() << endl;
        }

        for (OrderPair o: studio.getTrainer(trainerId)->getOrders()) {
            cout << o.second.getName() + " " + std::to_string(o.second.getPrice()) + "NIS " + std::to_string(o.first) << endl;
        }
        cout << "Current Trainer's Salary: " + to_string(studio.getTrainer(trainerId)->getCurrentSalary())+"NIS" << endl;
    }
    else
    {
        cout << "Trainer " + std::to_string(trainerId) + " status: closed"<< endl;
    }




    complete();
    studio.setActionsLog(this);

}
std::string PrintTrainerStatus::toString() const{
    return "PrintTrainerStatus "+std::to_string(trainerId);
}
BaseAction* PrintTrainerStatus::clone() {
    return new PrintTrainerStatus(trainerId);
}



//PrintActionsLog
PrintActionsLog::PrintActionsLog():BaseAction(){

}
PrintActionsLog::~PrintActionsLog(){

}
void PrintActionsLog::act(Studio &studio){

    for(BaseAction* a : studio.getActionsLog())
    {
        cout<<a->toString()<<endl;
    }
    complete();
    studio.setActionsLog(this);

}
std::string PrintActionsLog::toString() const{
    return "PrintActionsLog";
}
BaseAction* PrintActionsLog::clone() {
    return new PrintActionsLog();
}



//BackupStudio
BackupStudio::BackupStudio():BaseAction(){

}

BackupStudio::~BackupStudio(){

}

void BackupStudio::act(Studio &studio){
    studioBackUp = new Studio(studio);
    complete();
    studio.setActionsLog(this);
}

std::string BackupStudio::toString() const{
    return "BackupStudio";
}

BaseAction* BackupStudio::clone() {
    return new BackupStudio();
}



//RestoreStudio
RestoreStudio::RestoreStudio():BaseAction(){

}

RestoreStudio::~RestoreStudio() {

}

void RestoreStudio::act(Studio &studio) {
    if(studioBackUp != nullptr) {
        studio = *studioBackUp;
        complete();
        studio.setActionsLog(this);
    }
    else{
        cout<<"No backup available"<<endl;
    }
}

std::string RestoreStudio::toString() const{
    return "RestoreStudio";
}

BaseAction* RestoreStudio::clone() {
    return new RestoreStudio();
}


