#include <iostream>
#include <string>
using namespace std;

const int MAX_SUBJECTS = 10;
const int MAX_SLOTS = 5;
const int MAX_TIMESLOTS = 5;

string days[MAX_SLOTS] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};

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
    string schedule[MAX_SLOTS][MAX_TIMESLOTS]; 

public:
    void inputDetails() {
        cout << "Enter Branch Name: ";
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

       
        for (int i = 0; i < MAX_SLOTS; i++) {
            for (int j = 0; j < MAX_TIMESLOTS; j++) {
                schedule[i][j] = "Free";
            }
        }
    }

    bool isSubjectOnSameDay(int day, string subjectName) {
        for (int t = 0; t < timeSlotCount; t++) {
            if (schedule[day][t].find(subjectName) != string::npos) {
                return true;
            }
        }
        return false;
    }

    void generateTimetable() {
        int currentSubject = 0;
        for (int day = 0; day < MAX_SLOTS; day++) {
            for (int slot = 0; slot < timeSlotCount; slot++) {
                if (currentSubject >= subjectCount)
                    currentSubject = 0;

                bool assigned = false;
                for (int s = 0; s < subjectCount; s++) {
                    int index = (currentSubject + s) % subjectCount;
                    Subject &sub = subjects[index];

                    if (sub.assignedLectures < sub.lecturesPerWeek && !isSubjectOnSameDay(day, sub.name)) {
                        schedule[day][slot] = sub.name + " (" + sub.professor + ")";
                        sub.assignedLectures++;
                        assigned = true;
                        currentSubject = index + 1;
                        break;
                    }
                }

                if (!assigned) {
                    schedule[day][slot] = "Free";
                }
            }
        }
    }

    void displayTimetable() {
        cout << "\n=== Timetable for Branch: " << branch << " ===\n\n";

       
        cout << "Day\\Time\t";
        for (int t = 0; t < timeSlotCount; t++) {
            cout << timeSlots[t] << "\t";
        }
        cout << "\n";

       
        for (int d = 0; d < MAX_SLOTS; d++) {
            cout << days[d] << "\t";
            for (int t = 0; t < timeSlotCount; t++) {
                cout << schedule[d][t] << "\t";
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
