class Patient {
    constructor(name, gender, contact, priority) {
        this.name = name;
        this.gender = gender;
        this.contact = contact;
        this.priority = priority;
        this.next = null;
        this.head = null;
    }
}

class PriorityList {
    constructor() {
        this.head = null;
        this.beds = 10; // Number of available hospital beds
    }
    clearBed() {
        let message;
        if (this.beds < 10) {
            this.beds += 1;
            message = "A bed has been cleared.";
        } else {
            message = "Beds are at full capacity";
        }
        document.getElementById("output").innerHTML = message;
    }
    
    

    addToList(name, gender, contact, priority) {
       
        let newPatient = new Patient(name, gender, contact, priority);
        if (this.head === null || this.head.priority < priority) {
            newPatient.next = this.head;
            this.head = newPatient;
        } else {
            let temp = this.head;
            while (temp.next !== null && temp.next.priority >= priority) {
                temp = temp.next;
            }
            newPatient.next = temp.next;
            temp.next = newPatient;
        }
    }

    showList() {
        let result = "";
        let temp = this.head;
        if (this.head === null) {
            result = "Empty List";
        } else {
            while (temp !== null) {
                result += `${temp.name} ${temp.gender} ${temp.contact}<br>`;
                temp = temp.next;
            }
        }
        return result;
    }

    assignBed() {
        if (this.head === null) {
            return "No patients in the list";
        } else if (this.beds !== 0) {
            let assignedPatient = this.head;
            this.head = this.head.next;
            this.beds -= 1;
            return `${assignedPatient.name} has been assigned a bed`;
        } else {
            return "Sorry, no beds available";
        }
    }

    checkBedAvailability() {
        if(this.beds === 0){
            return 'No beds Available';
        }else{
            return `Beds Available: ${this.beds}`;
        }
    }

    searchPatient(name) {
        let temp = this.head;
        while (temp !== null) {
            if (temp.name === name) {
                return `Name: ${temp.name}, Gender: ${temp.gender}, Contact: ${temp.contact}, Severity: ${temp.priority}`;
            }
            temp = temp.next;
        }
        return "Patient not found";
    }

    patientStatus(name) {
        let count = 0;
        let temp = this.head;
        while (temp !== null) {
            if (temp.name === name) {
                document.getElementById('clearPatient').innerText = `Patient ${name} is ${count}th in the queue`;
                return true;
            }
            count++;
            temp = temp.next;
        }
        document.getElementById('clearPatient').innerText = "Patient not found";
        return false;
    }

    countPatients() {
        let temp = this.head;
        let count = 0;
        while (temp !== null) {
            count++;
            temp = temp.next;
        }
        return `Total Patients: ${count}`;
    }

    clearPatientList() {
        this.head = null;
        return "Patient list cleared";
    }

    resetBeds() {
        this.beds = 10;
        return "Beds reset to 10";
    }
}
    


const priorityList = new PriorityList();

function addPatient() {
    const name = document.getElementById("name").value.trim();
    const gender = document.getElementById("gender").value;
    const contact = document.getElementById("contact").value.trim();
    const priority = document.getElementById("priority").value.trim();

    // Check for empty inputs
    if (name === "" || gender === "Not Selected" || contact === "" || priority === "") {
        return alert("Please fill in all the required fields to admit the patient.");
    }

    // Check if priority is a valid number
    if (priority < 1 || priority > 10) {
        return alert("Please enter a valid priority between 1 and 10.");
    }

    priorityList.addToList(name, gender, contact, parseInt(priority)); // Ensure priority is passed as an integer
    document.getElementById("output").innerHTML = `Patient ${name} added to the list`;
    document.getElementById('name').value = "";
    document.getElementById('gender').value = "Not Selected";
    document.getElementById('contact').value = "";
    document.getElementById('priority').value = "";
}

function showPatients() {
    const list = priorityList.showList();
    document.getElementById("output").innerHTML = list;
}

function assignBed() {
    const message = priorityList.assignBed();
    document.getElementById("output").innerHTML = message;
}

function checkBed() {
    const message = priorityList.checkBedAvailability();
    document.getElementById("output").innerHTML = message;
}

function searchPatient() {
    const name = prompt("Enter patient name:");
    const result = priorityList.searchPatient(name);
    document.getElementById("output").innerHTML = result;
}

function countPatients() {
    const result = priorityList.countPatients();
    document.getElementById("output").innerHTML = result;
}

function clearList() {
    const result = priorityList.clearPatientList();
    document.getElementById("output").innerHTML = result;
}

function resetBeds() {
    const result = priorityList.resetBeds();
    document.getElementById("output").innerHTML = result;
}
document.getElementById('name').addEventListener('keypress',function(btn){
    if(btn.key === 'Enter'){
        addPatient();
    }
})
document.getElementById('priority').addEventListener('keypress',function(btn){
    if(btn.key === 'Enter'){
        addPatient();
    }
})
document.getElementById('contact').addEventListener('keypress',function(btn){
    if(btn.key === 'Enter'){ 
        addPatient();
    }
})
