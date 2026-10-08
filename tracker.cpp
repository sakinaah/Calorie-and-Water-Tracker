#include "tracker.h"
#include <fstream>

void saveGoals(int dailycaloriegoal, int dailywatergoal) {

    std::ofstream outFile("dailydata.txt");

    outFile << "Calorie Goal: " << dailycaloriegoal << ".\n";
    outFile << "Water Goal: " << dailywatergoal << ".\n";

    outFile.close();
}
