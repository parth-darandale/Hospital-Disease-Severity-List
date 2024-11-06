#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>
using namespace std;

struct Node {
    string name;
    string gender;
    int contact;
    int priority;
    int treatmentTime; // Treatment time in seconds
    Node *next;
};

class Linkedlist {
public:
    Node *head;
    Linkedlist() { head = NULL; }
    mutex listMutex; // Mutex for thread safety

    void addPatient(string n, string g, int c, int p, int t) {
        Node *nn = new Node;
        nn->name = n;
        nn->gender = g;
        nn->contact = c;
        nn->priority = p;
        nn->treatmentTime = t;
        nn->next = NULL;

        lock_guard<mutex> lock(listMutex); // Lock for thread safety
        if (head == NULL || head->priority < p) {
            nn->next = head;
            head = nn;
        } else {
            Node *temp = head;
            while (temp->next != NULL && temp->next->priority >= p) {
                temp = temp->next;
            }
            nn->next = temp->next;
            temp->next = nn;
        }
    }

    void showList() {
        lock_guard<mutex> lock(listMutex); // Lock for thread safety
        Node *temp = head;
        while (temp != NULL) {
            cout << "Name: " << temp->name << ", Gender: " << temp->gender
                 << ", Contact: " << temp->contact << ", Priority: " << temp->priority
                 << ", Treatment Time: " << temp->treatmentTime << "s" << endl;
            temp = temp->next;
        }
    }

    void removeHead() {
        lock_guard<mutex> lock(listMutex); // Lock for thread safety
        if (head != NULL) {
            Node *temp = head;
            head = head->next;
            delete temp;
        }
    }
};

class Lists {
    Linkedlist L1, L2, L3, L4; // Ward lists
    Linkedlist T1, T2, T3;     // Treatment lists
    int general_wards = 4;
    int icu_wards = 3;
    int surgical_wards = 2;
    int general_doctor = 3;
    int pseudo_specialist = 2;
    int specialist = 3;

public:
    void addPatientToWaitlist() {
        string name, gender;
        int contact, priority, treatmentTime;
        cout << "Enter Patient Name: ";
        cin >> name;
        cout << "Enter Gender (M/F): ";
        cin >> gender;
        cout << "Enter Contact Number: ";
        cin >> contact;
        cout << "Enter Priority (1-10): ";
        cin >> priority;
        cout << "Enter Treatment Time in seconds: ";
        cin >> treatmentTime;
        L1.addPatient(name, gender, contact, priority, treatmentTime);
        cout << "Patient added to waitlist.\n";
    }

    void assignDoctor(Linkedlist &ward, Linkedlist &treatmentList, int &doctorCount) {
        if (ward.head != NULL && doctorCount > 0) {
            treatmentList.addPatient(ward.head->name, ward.head->gender, ward.head->contact, ward.head->priority, ward.head->treatmentTime);
            int treatmentTime = ward.head->treatmentTime; // Get treatment time for thread
            cout << ward.head->name << " assigned a doctor. Treatment in progress for " << treatmentTime << " seconds.\n";
            ward.removeHead();
            doctorCount--;

            // Start a new thread to discharge the patient automatically after treatmentTime seconds
            thread([this, &treatmentList, treatmentTime]() {
                this_thread::sleep_for(chrono::seconds(treatmentTime));
                if (treatmentList.head != NULL) {
                    cout << "Auto-discharging: " << treatmentList.head->name << " from treatment list.\n";
                    treatmentList.removeHead();
                }
            }).detach();
        } else if (ward.head == NULL) {
            cout << "No patients in the selected ward.\n";
        } else {
            cout << "Doctors busy. Please wait.\n";
        }
    }

    void assignDoctor_general_ward() {
        assignDoctor(L2, T1, general_doctor);
    }

    void assignDoctor_icu_ward() {
        assignDoctor(L3, T2, pseudo_specialist);
    }

    void assignDoctor_surgery_ward() {
        assignDoctor(L4, T3, specialist);
    }

    void discharge(int ward) {
        Linkedlist *treatmentList = nullptr;
        int *wardCount = nullptr;
        int *doctorCount = nullptr;

        switch (ward) {
            case 1:
                treatmentList = &T1;
                wardCount = &general_wards;
                doctorCount = &general_doctor;
                break;
            case 2:
                treatmentList = &T2;
                wardCount = &icu_wards;
                doctorCount = &pseudo_specialist;
                break;
            case 3:
                treatmentList = &T3;
                wardCount = &surgical_wards;
                doctorCount = &specialist;
                break;
            default:
                cout << "Invalid option.\n";
                return;
        }

        if (treatmentList->head != NULL) {
            cout << "Manually discharging: " << treatmentList->head->name << endl;
            treatmentList->removeHead();
            if (*wardCount < 4) (*wardCount)++;
            if (*doctorCount < 3) (*doctorCount)++;
        } else {
            cout << "No patients available in the selected treatment list for discharge.\n";
        }
    }

    void showWaitlist() {
        cout << "Waitlist:\n";
        L1.showList();
    }

    void showWardList(int ward) {
        if (ward == 2) {
            cout << "General Ward Patients:\n";
            L2.showList();
        } else if (ward == 3) {
            cout << "ICU Ward Patients:\n";
            L3.showList();
        } else if (ward == 4) {
            cout << "Surgery Ward Patients:\n";
            L4.showList();
        } else {
            cout << "Invalid ward selection.\n";
        }
    }

    void showTreatmentList(int treatment) {
        if (treatment == 1) {
            cout << "General Ward Treatment Patients:\n";
            T1.showList();
        } else if (treatment == 2) {
            cout << "ICU Ward Treatment Patients:\n";
            T2.showList();
        } else if (treatment == 3) {
            cout << "Surgery Ward Treatment Patients:\n";
            T3.showList();
        } else {
            cout << "Invalid treatment list selection.\n";
        }
    }
};

int main() {
    Lists hospital;
    int choice;

    cout << "\n--- Hospital Management Menu ---\n";
    cout << "1) Add Patient to Waitlist\n";
    cout << "2) Show Waitlist\n";
    cout << "3) Assign Ward\n";
    cout << "4) Show Ward List\n";
    cout << "5) Assign General Ward Doctor\n";
    cout << "6) Assign ICU Ward Doctor\n";
    cout << "7) Assign Surgery Ward Doctor\n";
    cout << "8) Show Ward Treatment List\n";
    cout << "9) Discharge\n";
    cout << "10) Exit\n";

    while (true) {
        cout << "Enter choice: ";
        cin >> choice;

        int ward;
        switch (choice) {
            case 1: hospital.addPatientToWaitlist(); break;
            case 2: hospital.showWaitlist(); break;
            case 3: hospital.assignWard(); break;
            case 4: 
                cout << "1) General Ward \n2) ICU Ward \n3) Surgery Ward" << endl;
                cin >> ward;
                hospital.showWardList(ward); break;
            case 5: hospital.assignDoctor_general_ward(); break;
            case 6: hospital.assignDoctor_icu_ward(); break;
            case 7: hospital.assignDoctor_surgery_ward(); break;
            case 8:
                cout << "1) General Ward \n2) ICU Ward \n3) Surgery Ward" << endl;
                cin >> ward;
                hospital.showTreatmentList(ward); break;
            case 9:
                cout << "1) General Ward \n2) ICU Ward \n3) Surgery Ward" << endl;
                cin >> ward;
                hospital.discharge(ward);
                break;
            case 10: return 0;
            default: cout << "Invalid choice. Try again.\n";
        }
    }
}
