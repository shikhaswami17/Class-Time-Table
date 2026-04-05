#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <vector>

using namespace std;

const int MAX_SUBJECTS = 10;
const int MAX_DAYS = 5;
const int MAX_TIMESLOTS = 10;

string days[MAX_DAYS] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};

class Subject {
public:
    string name;
    int lecturesPerWeek;
    int assignedLectures;
    string professor;

    void input() {
        cout << "Enter Subject Name: ";
        getline(cin, name);
        cout << "Enter Professor Name for " << name << ": ";
        getline(cin, professor);
        cout << "How many lectures per week?: ";
        cin >> lecturesPerWeek;
        assignedLectures = 0;
        cin.ignore();
    }
};

class Timetable {
private:
    string branch;
    int subjectCount;
    Subject subjects[MAX_SUBJECTS];

    int timeSlotCount;
    string timeSlots[MAX_TIMESLOTS];

    string schedule[MAX_TIMESLOTS][MAX_DAYS];
    vector<pair<int,int>> slotOrder;

    map<string, set<pair<int,int>>> profBusy;

    bool isValid(int t, int d, Subject &sub) {
        if (sub.assignedLectures >= sub.lecturesPerWeek)
            return false;

        for (int i = 0; i < timeSlotCount; i++) {
            if (schedule[i][d].find(sub.name) != string::npos)
                return false;
        }

        if (profBusy[sub.professor].count({t, d}))
            return false;

        return true;
    }

    bool backtrack(int pos) {
        if (pos == slotOrder.size())
            return true;

        int t = slotOrder[pos].first;
        int d = slotOrder[pos].second;

        for (int i = 0; i < subjectCount; i++) {
            Subject &sub = subjects[i];

            if (isValid(t, d, sub)) {
                schedule[t][d] = sub.name + " (" + sub.professor + ")";
                sub.assignedLectures++;
                profBusy[sub.professor].insert({t, d});

                if (backtrack(pos + 1))
                    return true;

                schedule[t][d] = "Free";
                sub.assignedLectures--;
                profBusy[sub.professor].erase({t, d});
            }
        }

        schedule[t][d] = "Free";
        return backtrack(pos + 1);
    }

    void prepareSlotOrder() {
        for (int d = 0; d < MAX_DAYS; d++) {
            for (int t = 0; t < timeSlotCount; t++) {
                slotOrder.push_back({t, d});
            }
        }
    }

public:
    void loadProfessorBusySlots(const string& filename) {
        ifstream fin(filename);
        string prof;
        int t, d;
        while (fin >> prof >> t >> d) {
            profBusy[prof].insert({t, d});
        }
        fin.close();
    }

    void inputDetails() {
        cout << "\nEnter Branch Name: ";
        getline(cin, branch);

        cout << "How many subjects?: ";
        cin >> subjectCount;
        cin.ignore();

        for (int i = 0; i < subjectCount; i++) {
            subjects[i].input();
        }

        cout << "Time slots per day?: ";
        cin >> timeSlotCount;
        cin.ignore();

        for (int i = 0; i < timeSlotCount; i++) {
            cout << "Enter Time Slot: ";
            getline(cin, timeSlots[i]);
        }

        for (int t = 0; t < MAX_TIMESLOTS; t++)
            for (int d = 0; d < MAX_DAYS; d++)
                schedule[t][d] = "Free";

        prepareSlotOrder();
    }

    void generateTimetable() {
        if (!backtrack(0)) {
            cout << "No valid timetable possible!\n";
        }
    }

    void displayTimetable() {
        cout << "\n=== Timetable for " << branch << " ===\n\n";

        cout << "Time\\Day\t";
        for (int d = 0; d < MAX_DAYS; d++)
            cout << days[d] << "\t";
        cout << "\n";

        for (int t = 0; t < timeSlotCount; t++) {
            cout << timeSlots[t] << "\t";
            for (int d = 0; d < MAX_DAYS; d++) {
                cout << schedule[t][d] << "\t";
            }
            cout << "\n";
        }
    }
};

int main() {
    Timetable t;

    t.inputDetails();
    t.generateTimetable();
    t.displayTimetable();

    return 0;
}
