// Weekly Performance Evaluator - Week 6

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;


// Constants
const double PRO_TRAIN_HRS = 6.0;
const double INT_TRAIN_HRS = 3.0;
const double SLEEP_MIN_OK = 7.0;
const double SLEEP_GOOD_MIN = 7.5;
const double SLEEP_GOOD_MAX = 9.0;
const double W_TRAIN = 10.0;
const double W_SLEEP = 5.0;

const int    INPUT_FLUSH = 10000;
const int    MAX_SESSIONS = 5;     // max number of sessions user can enter
const double SHIFT_MIN = 10.0;

// No magic numbers:
const int    MIN_AGE = 1;
const double MIN_TRAINING_HOURS = 0.0;
const double MIN_SLEEP_HOURS = 0.01;

const int    MENU_MIN_CHOICE = 0;
const int    MENU_MAX_CHOICE = 3;

const double PRO_TECH_MINS = 50.0;
const double PRO_COND_MINS = 30.0;
const double INT_TECH_MINS = 45.0;
const double INT_COND_MINS = 25.0;
const double BEG_TECH_MINS = 35.0;
const double BEG_COND_MINS = 20.0;

const int    REST_DAYS_HIGH = 2;
const int    REST_DAYS_MODERATE = 1;
const int    REST_DAYS_LOW = 0;


// Enum for player performance level
enum PlayerLevel { LEVEL_BEGINNER, LEVEL_INTERMEDIATE, LEVEL_PRO };


// Function Prototypes
void setConsoleColor();
void showBanner();

int    getValidInt(const string& prompt, int minValue);
double getValidDouble(const string& prompt, double minValue);

void collectPlayerBasics(string& player, int& age);

// Array-related functions:
void fillTrainingSessions(double sessions[], int& sessionCount, int maxSessions);
void computeTrainingStats(const double sessions[], int sessionCount,
    double& totalTrain, double& avgTrain);

void evaluateLevel(double avgTrainingHours, double sleepHours,
    PlayerLevel& level, double& readinessScore, string& advice);

int getMenuChoice();

void handleLevelReport(const string& player, int age,
    double avgTrainingHours, double sleepHours,
    PlayerLevel level, double readinessScore,
    const string& advice,
    const double trainingSessions[], int sessionCount,
    double totalTraining);

void handleTrainingPlanReport(const string& player, int age,
    PlayerLevel level, double sleepHours);

void handleRecoveryReport(const string& player, int age,
    double totalTraining, double sleepHours);

string levelToString(PlayerLevel level);

// -------------------------------
// FUNCTIONS
// -------------------------------

// console color 
void setConsoleColor()
{
    system("color 1E");
}

// Display program banner
void showBanner()
{
    cout << "===================================\n";
    cout << "    Weekly Performance Evaluator\n";
    cout << "===================================\n\n";
}

// Safe integer input validation
int getValidInt(const string& prompt, int minValue)
{
    int value;
    cout << prompt;
    while (!(cin >> value) || value < minValue) {
        cin.clear();
        cin.ignore(INPUT_FLUSH, '\n');
        cout << "Invalid input. " << prompt;
    }
    cin.ignore(INPUT_FLUSH, '\n');
    return value;
}

// Safe double input validation
double getValidDouble(const string& prompt, double minValue)
{
    double value;
    cout << prompt;
    while (!(cin >> value) || value < minValue) {
        cin.clear();
        cin.ignore(INPUT_FLUSH, '\n');
        cout << "Invalid input. " << prompt;
    }
    cin.ignore(INPUT_FLUSH, '\n');
    return value;
}

// Collect player name and age
void collectPlayerBasics(string& player, int& age)
{
    cout << "Enter player's full name: ";
    getline(cin, player);

    age = getValidInt("Enter player's age: ", MIN_AGE);

    cout << fixed << showpoint << setprecision(2);
}

// Ask for number of sessions + fill array with user input
void fillTrainingSessions(double sessions[], int& sessionCount, int maxSessions)
{
    cout << "\nHow many training sessions did you have this week (1-"
        << maxSessions << ")? ";

    while (!(cin >> sessionCount) || sessionCount < 1 || sessionCount > maxSessions) {
        cin.clear();
        cin.ignore(INPUT_FLUSH, '\n');
        cout << "Invalid. Enter a number between 1 and "
            << maxSessions << ": ";
    }
    cin.ignore(INPUT_FLUSH, '\n');

    cout << "\nEnter training hours for each session:\n";

    for (int i = 0; i < sessionCount; ++i) {
        sessions[i] = getValidDouble("  Session " + to_string(i + 1) + ": ",
            MIN_TRAINING_HOURS);
    }

    // Fill remaining unused array elements with zeros
    for (int i = sessionCount; i < maxSessions; ++i) {
        sessions[i] = 0.0;
    }
}

// Calculates total and average hours from array
void computeTrainingStats(const double sessions[], int sessionCount,
    double& totalTrain, double& avgTrain)
{
    totalTrain = 0.0;

    for (int i = 0; i < sessionCount; ++i) {
        totalTrain += sessions[i];
    }

    avgTrain = (sessionCount > 0) ? totalTrain / sessionCount : 0.0;
}

// Evaluate player level using average training + sleep quality
void evaluateLevel(double avgTrainingHours, double sleepHours,
    PlayerLevel& level, double& readinessScore, string& advice)
{
    readinessScore = (avgTrainingHours * W_TRAIN)
        + ((sleepHours - SLEEP_MIN_OK) * W_SLEEP);

    if (avgTrainingHours >= PRO_TRAIN_HRS &&
        (sleepHours >= SLEEP_GOOD_MIN && sleepHours <= SLEEP_GOOD_MAX))
        level = LEVEL_PRO;
    else if (avgTrainingHours >= INT_TRAIN_HRS && sleepHours >= SLEEP_MIN_OK)
        level = LEVEL_INTERMEDIATE;
    else
        level = LEVEL_BEGINNER;

    if (sleepHours < SLEEP_MIN_OK || avgTrainingHours < INT_TRAIN_HRS)
        advice = "Build base: 3 sessions + 8h sleep.";
    else if (avgTrainingHours >= PRO_TRAIN_HRS &&
        !(sleepHours >= SLEEP_GOOD_MIN && sleepHours <= SLEEP_GOOD_MAX))
        advice = "Improve sleep routine.";
    else
        advice = "Good balance.";
}

// Menu system (same as Week 5)
int getMenuChoice()
{
    int choice;
    cout << "\nMenu:\n";
    cout << "  1) Evaluate Level\n";
    cout << "  2) Training Plan\n";
    cout << "  3) Recovery\n";
    cout << "  0) Quit\n";
    cout << "Enter choice: ";

    while (!(cin >> choice) || choice < MENU_MIN_CHOICE || choice > MENU_MAX_CHOICE) {
        cin.clear();
        cin.ignore(INPUT_FLUSH, '\n');
        cout << "Invalid. Enter " << MENU_MIN_CHOICE << "-" << MENU_MAX_CHOICE << ": ";
    }

    cin.ignore(INPUT_FLUSH, '\n');
    return choice;
}

// Level Report + array output to file
void handleLevelReport(const string& player, int age,
    double avgTrainingHours, double sleepHours,
    PlayerLevel level, double readinessScore,
    const string& advice,
    const double trainingSessions[], int sessionCount,
    double totalTraining)
{
    cout << "\n----- LEVEL REPORT -----\n";
    cout << "Player: " << player << "\n";
    cout << "Age: " << age << "\n";
    cout << "Level: " << levelToString(level) << "\n";
    cout << "Total training this week: " << totalTraining << "\n";
    cout << "Average training per session: " << avgTrainingHours << "\n";
    cout << "Average sleep: " << sleepHours << "\n";
    cout << "Readiness Score: " << readinessScore << "\n";
    cout << "Advice: " << advice << "\n";

    ofstream out("report.txt");
    if (out) {
        out << "WEEKLY PERFORMANCE REPORT (Week 6)\n";
        out << "Section: LEVEL\n";
        out << "----------------------------------\n";
        out << left << setw(22) << "Player:" << right << setw(20) << player << "\n";
        out << left << setw(22) << "Age:" << right << setw(20) << age << "\n";
        out << left << setw(22) << "Total training:" << right << setw(20) << totalTraining << "\n";
        out << left << setw(22) << "Avg training:" << right << setw(20) << avgTrainingHours << "\n";
        out << left << setw(22) << "Avg sleep:" << right << setw(20) << sleepHours << "\n";
        out << left << setw(22) << "Level:" << right << setw(20) << levelToString(level) << "\n";
        out << left << setw(22) << "Readiness:" << right << setw(20) << readinessScore << "\n";
        out << left << setw(22) << "Advice:" << right << setw(20) << advice << "\n\n";

        out << "Session details:\n";
        out << left << setw(10) << "Session"
            << setw(15) << "Hours" << "\n";

        for (int i = 0; i < sessionCount; ++i) {
            out << left << setw(10) << (i + 1)
                << setw(15) << trainingSessions[i] << "\n";
        }

        out.close();
    }
}

// Training Plan: single overall plan 
void handleTrainingPlanReport(const string& player, int age,
    PlayerLevel level, double sleepHours)
{
    string planFocus;
    double techMins;
    double condMins;

    // Choose base plan by level
    switch (level)
    {
    case LEVEL_PRO:
        techMins = PRO_TECH_MINS;
        condMins = PRO_COND_MINS;
        planFocus = "High Tempo";
        break;
    case LEVEL_INTERMEDIATE:
        techMins = INT_TECH_MINS;
        condMins = INT_COND_MINS;
        planFocus = "Basics";
        break;
    default:
        techMins = BEG_TECH_MINS;
        condMins = BEG_COND_MINS;
        planFocus = "Ball Mastery";
        break;
    }

    // Adjust conditioning if sleep is low
    if (sleepHours < SLEEP_MIN_OK && condMins >= SHIFT_MIN) {
        techMins += SHIFT_MIN;
        condMins -= SHIFT_MIN;
    }

    cout << "\n------- TRAINING PLAN -------\n";
    cout << left << setw(20) << "Player:" << right << setw(20) << player << "\n";
    cout << left << setw(20) << "Level:" << right << setw(20) << levelToString(level) << "\n";
    cout << left << setw(20) << "Focus:" << right << setw(20) << planFocus << "\n";
    cout << left << setw(20) << "Technical work:" << right << setw(20) << techMins << " min\n";
    cout << left << setw(20) << "Conditioning:" << right << setw(20) << condMins << " min\n";

    ofstream out("report.txt");
    if (out) {
        out << "WEEKLY PERFORMANCE REPORT (Week 6)\n";
        out << "Section: TRAINING PLAN\n";
        out << "----------------------------------\n";
        out << left << setw(20) << "Player:" << right << setw(20) << player << "\n";
        out << left << setw(20) << "Level:" << right << setw(20) << levelToString(level) << "\n";
        out << left << setw(20) << "Focus:" << right << setw(20) << planFocus << "\n";
        out << left << setw(20) << "Technical work:" << right << setw(20) << techMins << " min\n";
        out << left << setw(20) << "Conditioning:" << right << setw(20) << condMins << " min\n";
        out.close();
    }
}

// Recovery Report 
void handleRecoveryReport(const string& player, int age,
    double totalTraining, double sleepHours)
{
    string fatigue = "Low";
    int    rest = REST_DAYS_LOW;
    string tip;

    if (sleepHours < SLEEP_MIN_OK)
        fatigue = "High";
    else if (totalTraining >= PRO_TRAIN_HRS ||
        totalTraining < INT_TRAIN_HRS)
        fatigue = "Moderate";

    if (fatigue == "High")
        rest = REST_DAYS_HIGH;
    else if (fatigue == "Moderate")
        rest = REST_DAYS_MODERATE;

    if (sleepHours < SLEEP_MIN_OK)
        tip = "Aim for 8h sleep.";
    else if (totalTraining >= PRO_TRAIN_HRS)
        tip = "Foam roll.";
    else
        tip = "Light stretching.";

    cout << "\n---- RECOVERY ----\n";
    cout << left << setw(18) << "Fatigue:" << right << setw(20) << fatigue << "\n";
    cout << left << setw(18) << "Rest Days:" << right << setw(20) << rest << "\n";
    cout << left << setw(18) << "Tip:" << right << setw(20) << tip << "\n";

    ofstream out("report.txt");
    if (out) {
        out << "WEEKLY PERFORMANCE REPORT (Week 6)\n";
        out << "Section: RECOVERY\n";
        out << "----------------------------------\n";
        out << left << setw(18) << "Fatigue:" << right << setw(20) << fatigue << "\n";
        out << left << setw(18) << "Rest Days:" << right << setw(20) << rest << "\n";
        out << left << setw(18) << "Tip:" << right << setw(20) << tip << "\n";
        out.close();
    }
}

// Convert enum to string
string levelToString(PlayerLevel level)
{
    switch (level)
    {
    case LEVEL_PRO:          return "Pro";
    case LEVEL_INTERMEDIATE: return "Intermediate";
    case LEVEL_BEGINNER:     return "Beginner";
    default:                 return "Unknown";
    }
}

// -------------------------------
// MAIN
// -------------------------------
int main()
{
    setConsoleColor();
    showBanner();

    string player;
    int    age;

    collectPlayerBasics(player, age);

    // Arrays for training sessions
    double trainingSessions[MAX_SESSIONS] = { 0.0 };
    int    sessionCount = 0;

    fillTrainingSessions(trainingSessions, sessionCount, MAX_SESSIONS);

    double totalTraining = 0.0;
    double avgTraining = 0.0;
    computeTrainingStats(trainingSessions, sessionCount,
        totalTraining, avgTraining);

    double sleepHours = getValidDouble("Avg sleep hours per night: ",
        MIN_SLEEP_HOURS);

    PlayerLevel level;
    double      readinessScore;
    string      advice;

    evaluateLevel(avgTraining, sleepHours, level, readinessScore, advice);

    // Menu loop 
    int choice;
    do {
        choice = getMenuChoice();

        switch (choice)
        {
        case 1:
            handleLevelReport(player, age, avgTraining, sleepHours,
                level, readinessScore, advice,
                trainingSessions, sessionCount, totalTraining);
            break;

        case 2:
            handleTrainingPlanReport(player, age, level, sleepHours);
            break;

        case 3:
            handleRecoveryReport(player, age, totalTraining, sleepHours);
            break;

        case 0:
            cout << "\nExiting...\n";
            break;
        }

    } while (choice != 0);

    return 0;
}
