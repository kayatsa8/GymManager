#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <fstream>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"


class Studio{		
public:
	Studio();
    Studio(const std::string &configFilePath);
    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();
    Studio& operator=(const Studio& s);
    void setActionsLog(BaseAction* b);
    std::vector<Trainer*>& getTrainers();
    void close();

private:
    bool open;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
    int customerID;

    void callAction();

    void deleteTrainers();
    void deleteWorkoutOptions();
    void deleteActionLog();
    void createTrainers(std::vector<std::string>& reader);
    void createWorkoutOptions(std::vector<std::string>& reader);
};

#endif