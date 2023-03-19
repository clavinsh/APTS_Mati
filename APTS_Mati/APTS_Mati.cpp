#include <iostream>
#include<fstream>

struct Barber {
    unsigned int id;
    unsigned int lastServiceTime;

    Barber(unsigned int id) {
        this->id = id;
        this->lastServiceTime = 0;
    }

    // comparison function
    // priority takes the one barber with the longest wait time
    // if barbers have equal wait time, then priority takes the one with
    // the smallest id
    bool operator<(const Barber& other) const {
        if (lastServiceTime != other.lastServiceTime) {
            return lastServiceTime > other.lastServiceTime;
        }
        else {
            return id > other.id;
        }
    }
};

struct Client {
    unsigned int id;
    unsigned int arrivalTime;
    unsigned int serviceLength;

    Client(unsigned int id, unsigned int arrivalTime, unsigned int serviceLength) {
        this->id = id;
        this->arrivalTime = arrivalTime;
        this->serviceLength = serviceLength;
    }
};

// priority queue implementation for barbers using a heap

struct Node {
    Barber data;
    Node* next;

    Node(const Barber& b) : data(b), next(nullptr) {}
};

class PriorityQueue {
public:
    PriorityQueue(int numOfBarbers) {
        for (int i = 1; i <= numOfBarbers; i++) {
            Barber b(i);
            push(b);
        }
    }

    ~PriorityQueue() {
        while (!empty()) {
            Barber* b = new Barber(pop());
            delete b;
        }
    }

    // Add a Barber to the priority queue
    void push(const Barber& b) {
        Node* newNode = new Node(b);
        if (head == nullptr || b < head-> data) {
            newNode->next = head;
            head = newNode;
        }
        else {
            Node* curr = head;
            while (curr->next != nullptr && curr->next->data < b) {
                curr = curr->next;
            }
            newNode->next = curr->next;
            curr->next = newNode;
        }
    }

    // Remove and return the highest priority Barber from the queue
    Barber pop() {
        if (empty()) {
            throw std::out_of_range("Priority queue is empty");
        }

        Barber result = head->data;
        Node* temp = head;
        head = head->next;
        delete temp;
        return result;
    }

    // Check if the priority queue is empty
    bool empty() const {
        return head == nullptr;
    }

    // Get the size of the priority queue
    int size() const {
        int count = 0;
        Node* curr = head;
        while (curr != nullptr) {
            count++;
            curr = curr->next;
        }
        return count;
    }

private:
    Node* head = nullptr;
};


const unsigned int MAX_TIME = 2000000000;



int main() {
    std::fstream fin("hair.in");

    if (!fin.is_open()) {
        return -1;
    }

    unsigned int barbers = 0;

    fin >> barbers;

    // barber count constraints
    if (barbers < 1 || barbers > 9) {
        return -1;
    }

    // pq initialization
    PriorityQueue pq(barbers);


    // for each arrival event we do sth
    unsigned int id = 0;

    fin >> id;

    while (id != 0) {
        unsigned int arrivalTime = 0;
        unsigned int serviceLength = 0;

        fin >> arrivalTime;
        fin >> serviceLength;

        Client c(id, arrivalTime, serviceLength);
        //main logic here ig


        fin >> id;
    }

    fin.close();


    
    return 0;
}
