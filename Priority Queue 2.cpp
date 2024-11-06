#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <atomic>

using namespace std;
using namespace chrono;

struct Node {
    string name;
    string gender;
    int contact;
    int priority;
    int treatmentTime;    // Treatment time in seconds
    time_point<steady_clock> admissionTime; // Store admission time
    Node *next;
};

class Linkedlist {
public:
    Node *head;
    Linkedlist() { head = NULL; }

    void addPatient(string n, string g, int c, int p, int t = 0) {
        Node *nn = new Node;
        nn->name = n;
        nn->gender = g;
        nn->contact = c;
        nn->priority = p;
        nn->treatmentTime = t;
        nn->admissionTime = steady_clock::now(); // Set current time
        nn->next = NULL;

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
        Node *temp = head;
        while (temp != NULL) {
            cout << "Name: " << temp->name << ", Gender: " << temp->gender
                 << ", Contact: " << temp->contact << ", Priority: " << temp->priority
                 << ", Treatment Time: " << temp->treatmentTime << "s\n";
            temp = temp->next;
        }
    }

    void removeHead() {
        if (head != NULL) {
            Node *temp = head;
            head = head->next;
            delete temp;
        } else {
            cout << "No patients to discharge in this treatment list.\n";
        }
    }

    // Function to check and automatically discharge patients
    void dischargeByTime() {
        while (head != NULL) {
            auto currentTime = steady_clock::now();
            auto elapsed = duration_cast<seconds>(currentTime - head->admissionTime).count();

            if (elapsed >= head->treatmentTime) {
                cout << "Automatically discharging: " << head->name << " (Time up)\n";
                removeHead();
            } else {
                break; // Stop if the head patient is still within treatment time
            }
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
    atomic<bool> running;      // Atomic flag to control background thread

public:
    Lists() : running(true) {
        // Start background thread for automatic discharge
        thread([this]() {
            while (running) {
                T1.dischargeByTime();
                T2.dischargeByTime();
                T3.dischargeByTime();
                this_thread::sleep_for(seconds(5)); // Check every 5 second
            }
        }).detach();
    }

    ~Lists() {
        running = false; // Stop background thread
    }

    void addPatientToWaitlist() {
        string name, gender;
        int contact, priority;
        cout << "Enter Patient Name: ";
        cin >> name;
        cout << "Enter Gender (M/F): ";
        cin >> gender;
        cout << "Enter Contact Number: ";
        cin >> contact;
        cout << "Enter Priority (1-10): ";
        cin >> priority;
        L1.addPatient(name, gender, contact, priority);
        cout << "Patient added to waitlist.\n";
    }

    void assignWard() {
        if (L1.head == NULL) {
            cout << "No patients in the waitlist.\n";
            return;
        }

        int treatmentTime;
        cout << "Enter treatment time (in seconds): ";
        cin >> treatmentTime;

        if (L1.head->priority <= 5 && general_wards > 0) {
            L2.addPatient(L1.head->name, L1.head->gender, L1.head->contact, L1.head->priority, treatmentTime);
            cout << L1.head->name << " assigned to General ward.\n";
            L1.removeHead();
            general_wards--;
        } else if (L1.head->priority > 5 && L1.head->priority <= 8 && icu_wards > 0) {
            L3.addPatient(L1.head->name, L1.head->gender, L1.head->contact, L1.head->priority, treatmentTime);
            cout << L1.head->name << " assigned to ICU ward.\n";
            L1.removeHead();
            icu_wards--;
        } else if (surgical_wards > 0) {
            L4.addPatient(L1.head->name, L1.head->gender, L1.head->contact, L1.head->priority, treatmentTime);
            cout << L1.head->name << " assigned to Surgical ward.\n";
            L1.removeHead();
            surgical_wards--;
        } else {
            cout << "All wards are full. Cannot accommodate.\n";
        }
    }

    // Other existing functions (assignDoctor_general_ward, etc.) remain unchanged
};

int main() {
    Lists hospital;
    int choice;

    cout << "\n--- Hospital Management Menu ---\n";
    cout << "1) Add Patient to Waitlist\n";
    cout << "2) Show Waitlist\n";
    cout << "3) Assign Ward\n";
    cout << "4) Show Ward List\n";
    cout << "5) Exit\n";

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
            case 5: return 0;
            default: cout << "Invalid choice. Try again.\n";
        }

        // Delay to observe automatic discharge
        this_thread::sleep_for(seconds(1));
    }
}
