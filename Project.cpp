#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
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
        cout << "How many lectures per week for " << name << "?: ";
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
    int slotOrder[MAX_DAYS * MAX_TIMESLOTS][2];

    
    map<string, set<pair<int, int>>> profBusy;

    void prepareSlotOrder() {
        int index = 0;
        for (int d = 0; d < MAX_DAYS; d++) {
            for (int t = 0; t < timeSlotCount; t++) {
                slotOrder[index][0] = t;
                slotOrder[index][1] = d;
                index++;
            }
        }

        for (int i = 0; i < index; i++) {
            int swapWith = (i * 3 + 2) % index;
            int tmpT = slotOrder[i][0], tmpD = slotOrder[i][1];
            slotOrder[i][0] = slotOrder[swapWith][0];
            slotOrder[i][1] = slotOrder[swapWith][1];
            slotOrder[swapWith][0] = tmpT;
            slotOrder[swapWith][1] = tmpD;
        }
    }

    bool isSubjectOnSameDay(int day, string subjectName) {
        for (int t = 0; t < timeSlotCount; t++) {
            if (schedule[t][day].find(subjectName) != string::npos) {
                return true;
            }
        }
        return false;
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

        cout << "How many time slots per day?: ";
        cin >> timeSlotCount;
        cin.ignore();

        for (int i = 0; i < timeSlotCount; i++) {
            cout << "Enter Time Slot " << i + 1 << ": ";
            getline(cin, timeSlots[i]);
        }

        for (int t = 0; t < MAX_TIMESLOTS; t++) {
            for (int d = 0; d < MAX_DAYS; d++) {
                schedule[t][d] = "Free";
            }
        }

        prepareSlotOrder();
    }

    void generateTimetable(bool saveProfBusy = false, const string& outputFile = "") {
        int totalSlots = MAX_DAYS * timeSlotCount;
        int subjIndex = 0;

        ofstream fout;
        if (saveProfBusy && !outputFile.empty()) {
            fout.open(outputFile);
        }

        for (int i = 0; i < totalSlots; i++) {
            int t = slotOrder[i][0];
            int d = slotOrder[i][1];
            bool assigned = false;

            for (int j = 0; j < subjectCount; j++) {
                int index = (subjIndex + j) % subjectCount;
                Subject& sub = subjects[index];

                if (sub.assignedLectures < sub.lecturesPerWeek &&
                    !isSubjectOnSameDay(d, sub.name) &&
                    profBusy[sub.professor].count({t, d}) == 0) {

                    schedule[t][d] = sub.name + " (" + sub.professor + ")";
                    sub.assignedLectures++;
                    subjIndex = index + 1;
                    assigned = true;

                    if (saveProfBusy) {
                        fout << sub.professor << " " << t << " " << d << endl;
                    }

                    break;
                }
            }

            if (!assigned) {
                schedule[t][d] = "Free";
            }
        }

        if (fout.is_open()) fout.close();
    }

    void displayTimetable() {
        cout << "\n=== Timetable for Branch: " << branch << " ===\n\n";
        cout << "Time\\Day\t";
        for (int d = 0; d < MAX_DAYS; d++) {
            cout << days[d] << "\t";
        }
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
    
    Timetable t1;
    cout << "Enter details for Branch 1 (e.g., CSE):";
    t1.inputDetails();
    t1.generateTimetable(true, "prof_schedule.txt");
    t1.displayTimetable();

    
    Timetable t2;
    cout << "\nEnter details for Branch 2 (e.g., IT):";
    t2.loadProfessorBusySlots("prof_schedule.txt");
    t2.inputDetails();
    t2.generateTimetable(false);
    t2.displayTimetable();

    return 0;
}
