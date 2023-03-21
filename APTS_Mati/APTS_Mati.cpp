#include <iostream>
#include<fstream>

struct Barber {
    unsigned int id;
    unsigned int lastServiceTime;


    Barber(unsigned int id) {
        this->id = id;
        this->lastServiceTime = 0;
    }

    // can the barber complete the service without interrupting the break time
    bool canComplete(unsigned int startTime, unsigned int serviceLength) {
        // next or current break time since startTime
        unsigned int potentialBreakTime = startTime / 100;
        unsigned int hundreths = potentialBreakTime % 10;
        potentialBreakTime /= 10;

        if (hundreths == id) return false;

        potentialBreakTime = potentialBreakTime * 10 + id;
        potentialBreakTime *= 100;

        if (potentialBreakTime < startTime) {
            potentialBreakTime += 1000;
        }

        if (startTime + serviceLength - 1 < potentialBreakTime) {
            return true;
        }
        else return false;
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
            return id < other.id;
        }
    }

    bool operator==(const Barber& other) const {
        if (id == other.id && lastServiceTime == other.lastServiceTime)
            return true;
        else return false;
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
        if (head == nullptr || b < head->data) {
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

    void remove(const Barber& b) {
        Node* curr = head;
        Node* prev = nullptr;

        while (curr != nullptr && !(curr->data == b)) {
            prev = curr;
            curr = curr->next;
        }

        if (curr == nullptr) {
            // Barber not found in the queue
            return;
        }

        if (prev == nullptr) {
            // Barber is at the head of the queue
            head = curr->next;
        }
        else {
            prev->next = curr->next;
        }

        delete curr;
    }

    // Returns the highest priority barber from the queue
    Barber top() {
        return head->data;
    }

    // Returns the highest priority barber that can complete
    // the service without interrupting their break time
    Barber topCanComplete(unsigned int startTime, unsigned int serviceLength) {
        Node* current = head;

        while (current != nullptr) {
            Barber& b = current->data;

            if (b.canComplete(startTime, serviceLength)) {
                return b;
            }

            current = current->next;
        }

        return NULL;
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

struct Client {
    unsigned int id;
    unsigned int arrivalTime;
    unsigned int serviceLength;
    unsigned int serviceTime;
    unsigned int endTime;
    unsigned int barberID;


    Client() {}

    Client(unsigned int id, unsigned int arrivalTime, unsigned int serviceLength) {
        this->id = id;
        this->arrivalTime = arrivalTime;
        this->serviceLength = serviceLength;
        this->serviceTime = 0;
        this->endTime = 0;
        this->barberID = 0;
    }

    Client(Client& client) {
        this->id = client.id;
        this->arrivalTime = client.arrivalTime;
        this->serviceLength = client.serviceLength;
        this->serviceTime = client.serviceTime;
        this->endTime = client.endTime;
        this->barberID = client.barberID;
    }

    bool operator<(const Client& other) const {
        if (endTime != other.endTime) {
            return endTime < other.endTime;
        }
        else {
            return barberID < other.barberID;
        }
    }
};

// given that there may be many clients,
// and the ordering may change at any insertion (a client that arrives later
// may be already finished before a client arrives earlier)
// a binary search tree structure could do best for insertion time complexity O(log n)
struct BSTNode {
    Client data;
    BSTNode* left;
    BSTNode* right;

    //BSTNode() : data(nullptr), left(nullptr), right(nullptr) {};

    BSTNode(Client c) : data(c), left(nullptr), right(nullptr) {}
};

struct BST {
    BSTNode* root;

    BST() {
        root = NULL;
    }

    bool isEmpty() {
        return root = nullptr;
    }

    void insertNode(BSTNode* node) {
        if (root == nullptr) {
            root = node;
            return;
        }

        BSTNode* temp = root;

        while (temp != nullptr) {
            if (node->data < temp->data && temp->left == nullptr) {
                temp->left = node;
                break;
            }
            else if (node->data < temp->data) {
                temp = temp->left;
            }
            else if ((temp->data < node->data) && (temp->right == NULL)) {
                temp->right = node;
                break;
            }
            else {
                temp = temp->right;
            }
        }
    }

    void insertClient(Client client) {
        BSTNode* newNode = new BSTNode(client);
        insertNode(newNode);
    }

    void inOrderTraversal(BSTNode* root) {
        if (root == nullptr) {
            return;
        }
        inOrderTraversal(root->left);
        std::cout << root->data.endTime << " " << root->data.barberID << " " << root->data.id << '\n';
        inOrderTraversal(root->right);
    }

    void print() {
        inOrderTraversal(root);
    }
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

    // busy barbers
    //Barber busyBarbers[9] = {0,0,0,0,0,0,0,0,0};


    BST clients;

    
    unsigned int arrivalTime = 0;

    fin >> arrivalTime;

    while (arrivalTime != 0) {
        
        unsigned int id = 0;
        unsigned int serviceLength = 0;

        fin >> id;
        fin >> serviceLength;

        Client c(id, arrivalTime, serviceLength);


        // when the current client arrives, we check
        // if any busy barbers have completed the job

        // if there is a barber immediately available that can already complete the task
        // then we assign 
        Barber b = pq.topCanComplete(arrivalTime, serviceLength);

        if (!(b == NULL)) {
            unsigned int end = arrivalTime + serviceLength - 1;
            c.barberID = b.id;
            c.serviceTime = arrivalTime;
            c.endTime = end;
            clients.insertClient(c);
            pq.remove(b);
            b.lastServiceTime = end;
        }

        fin >> arrivalTime;
    }

    fin.close();

    clients.print();

    return 0;
}
