#include <iostream>
#include <string>
using namespace std;

struct Node {
    string name;
    string gender;
    int contact;
    int priority;
    Node *next;
};

class Linkedlist {
public:
    Node *head;
    Linkedlist() { head = NULL; }

    void addPatient(string n, string g, int c, int p) {
        Node *nn = new Node;
        nn->name = n;
        nn->gender = g;
        nn->contact = c;
        nn->priority = p;
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
                 << ", Contact: " << temp->contact << ", Priority: " << temp->priority << endl;
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

        if (L1.head->priority <= 5 && general_wards > 0) {
            L2.addPatient(L1.head->name, L1.head->gender, L1.head->contact, L1.head->priority);
            cout << L1.head->name << " assigned to General ward.\n";
            L1.removeHead();
            general_wards--;
        } else if (L1.head->priority > 5 && L1.head->priority <= 8 && icu_wards > 0) {
            L3.addPatient(L1.head->name, L1.head->gender, L1.head->contact, L1.head->priority);
            cout << L1.head->name << " assigned to ICU ward.\n";
            L1.removeHead();
            icu_wards--;
        } else if (surgical_wards > 0) {
            L4.addPatient(L1.head->name, L1.head->gender, L1.head->contact, L1.head->priority);
            cout << L1.head->name << " assigned to Surgical ward.\n";
            L1.removeHead();
            surgical_wards--;
        } else {
            cout << "All wards are full. Cannot accommodate.\n";
        }
    }

    void assignDoctor_general_ward() {
        if (L2.head != NULL && general_doctor > 0) {
            T1.addPatient(L2.head->name, L2.head->gender, L2.head->contact, L2.head->priority);
            cout << L2.head->name << " assigned a general doctor. Treatment in progress.\n";
            L2.removeHead();
            general_doctor--;
        } else if (L2.head == NULL) {
            cout << "No patients in General Ward.\n";
        } else {
            cout << "General doctors busy. Please wait.\n";
        }
    }

    void assignDoctor_icu_ward() {
        if (L3.head != NULL && pseudo_specialist > 0) {
            T2.addPatient(L3.head->name, L3.head->gender, L3.head->contact, L3.head->priority);
            cout << L3.head->name << " assigned a pseudo specialist. Treatment in progress.\n";
            L3.removeHead();
            pseudo_specialist--;
        } else if (L3.head == NULL) {
            cout << "No patients in ICU Ward.\n";
        } else {
            cout << "ICU doctors busy. Please wait.\n";
        }
    }

    void assignDoctor_surgery_ward() {
        if (L4.head != NULL && specialist > 0) {
            T3.addPatient(L4.head->name, L4.head->gender, L4.head->contact, L4.head->priority);
            cout << L4.head->name << " assigned a specialist. Treatment in progress.\n";
            L4.removeHead();
            specialist--;
        } else if (L4.head == NULL) {
            cout << "No patients in Surgery Ward.\n";
        } else {
            cout << "Specialists busy. Please wait.\n";
        }
    }

    void discharge(int ward) {
        if (ward == 1 && T1.head != NULL) {
            cout << "Discharging from General Ward: " << T1.head->name << endl;
            T1.removeHead();
            if (general_wards < 4) general_wards++;
            if (general_doctor < 3) general_doctor++;
    
        } else if (ward == 2 && T2.head != NULL) {
            cout << "Discharging from ICU Ward: " << T2.head->name << endl;
            T2.removeHead();
            if (icu_wards < 3) icu_wards++;
            if (pseudo_specialist < 2) pseudo_specialist++;
    
        } else if (ward == 3 && T3.head != NULL) {
            cout << "Discharging from Surgery Ward: " << T3.head->name << endl;
            T3.removeHead();
            if (surgical_wards < 2) surgical_wards++;
            if (specialist < 3) specialist++;
    
        } else {
            cout << "Invalid option or no patients available in the treatment list for discharge.\n";
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
