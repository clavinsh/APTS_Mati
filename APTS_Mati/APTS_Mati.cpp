#include<fstream>

struct Barber {
    unsigned int id;
    unsigned int lastServiceTime;

    Barber() {
        this->id = 0;
        this->lastServiceTime = 0;
    }

    Barber(unsigned int id) {
        this->id = id;
        this->lastServiceTime = 0;
    }

    // can the barber complete the service without interrupting the break time
    bool canComplete(unsigned int startTime, unsigned int serviceLength) {
        // can't complete the service if it starts before/during the previous service
        if (startTime <= this->lastServiceTime) {
            return false;
        }

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

    // returns the moment this barber is available
    // after the current, oncoming break 
    unsigned int nextFreeTime(unsigned int currentTime) {

        unsigned int currentBreakTime = currentTime / 1000;
        currentBreakTime = currentBreakTime * 1000 + id * 100;

        if (currentTime > currentBreakTime + 99) {
            return currentBreakTime + 1100;
        }

        return currentBreakTime + 100;
    }



    unsigned int earliestAvailabilityForService(unsigned int startTime, unsigned int serviceLength) {
        // can we complete immediately 
        if (canComplete(startTime, serviceLength)) {
            return startTime;
        }

        // still doing the previous service
        if (startTime <= this->lastServiceTime) {
            // can serve immediately after
            if (canComplete(this->lastServiceTime + 1, serviceLength)) {
                return this->lastServiceTime + 1;
            }
            // break time interrupts
            else {
                return nextFreeTime(this->lastServiceTime + 1);
            }
        }

        // the break time interrupts, should be able to do it right after
        // break time
        else {
            return nextFreeTime(startTime);
        }
    }

    // comparison function
    // priority takes the one barber with the longest wait time
    // if barbers have equal wait time, then priority takes the one with
    // the smallest id
    bool operator<(const Barber& other) const {
        if (lastServiceTime != other.lastServiceTime) {
            return lastServiceTime < other.lastServiceTime;
        }
        else {
            return id < other.id;
        }
    }

    // equality function
    // checks if all the values match
    bool operator==(const Barber& other) const {
        if (id == other.id && lastServiceTime == other.lastServiceTime)
            return true;
        else return false;
    }
};

struct PQNode {
    Barber data;
    PQNode* next;

    PQNode(const Barber& b) : data(b), next(nullptr) {}
};

// priority queue implementation for barbers
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
            pop();
        }
    }

    // Add a Barber to the priority queue
    void push(const Barber& b) {
        PQNode* newNode = new PQNode(b);
        if (head == nullptr || b < head->data) {
            newNode->next = head;
            head = newNode;
        }
        else {
            PQNode* curr = head;
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

        Barber result = std::move(head->data);
        PQNode* temp = head;
        head = head->next;
        delete temp;
        return result;
    }

    void remove(const Barber& b) {
        PQNode* curr = head;
        PQNode* prev = nullptr;

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
        return head != nullptr ? head->data : NULL;
    }

    // Returns the lowest priority barber's lastServiceTime from the queue
    unsigned int bottomServiceTime() {
        PQNode* current = head;

        while (current->next != nullptr) {
            current = current->next;
        }

        return current->data.lastServiceTime;
    }

    // Returns the highest priority barber that can complete
    // the service without interrupting their break time
    Barber topCanComplete(unsigned int startTime, unsigned int serviceLength) {
        PQNode* current = head;

        while (current != nullptr) {
            Barber& b = current->data;

            if (b.canComplete(startTime, serviceLength)) {
                return b;
            }

            current = current->next;
        }

        return NULL;
    }

    Barber earliestAvailability(unsigned int startTime, unsigned int serviceLength) {
        PQNode* current = head;

        Barber earliest = current->data;
        unsigned int earliestTime = earliest.earliestAvailabilityForService(startTime, serviceLength);

        while (current != nullptr) {
            unsigned int currentTime = current->data.earliestAvailabilityForService(startTime, serviceLength);

            // earlier than previous one
            if (currentTime < earliestTime) {
                earliestTime = currentTime;
                earliest = current->data;
            }
            // same time but higher priority
            else if (currentTime == earliestTime && current->data < earliest) {
                earliestTime = currentTime;
                earliest = current->data;
            }
            current = current->next;    
        }

        return earliest;
    }

    // Check if the priority queue is empty
    bool empty() const {
        return head == nullptr;
    }

    // Get the size of the priority queue
    int size() const {
        int count = 0;
        PQNode* curr = head;
        while (curr != nullptr) {
            count++;
            curr = curr->next;
        }
        return count;
    }

private:
    PQNode* head = nullptr;
};

struct Client {
    unsigned int id;
    unsigned int arrivalTime;
    unsigned int serviceLength;
    unsigned int serviceTime;
    unsigned int endTime;
    unsigned int barberID;


    Client() {
        this->id = 0;
        this->arrivalTime = 0;
        this->serviceLength = 0;
        this->serviceTime = 0;
        this->endTime = 0;
        this->barberID = 0;
    }

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

    void inOrderTraversalToFile(BSTNode* root, std::ofstream& file) {
        if (root == nullptr) {
            return;
        }
        inOrderTraversalToFile(root->left,file);
        file << root->data.endTime << " " << root->data.barberID << " " << root->data.id << '\n';
        inOrderTraversalToFile(root->right,file);
    }

    void printToFile(std::ofstream& file) {
        inOrderTraversalToFile(root,file);
    }
};

template <typename T>
struct ListNode {
    T data;
    ListNode<T>* next;
};

template <typename T>
class LinkedList {
private:
    ListNode<T>* head;
    ListNode<T>* tail;
public:
    LinkedList() {
        head = nullptr;
        tail = nullptr;
    }
    ~LinkedList() {
        while (head != nullptr) {
            ListNode<T>* temp = head;
            head = head->next;
            delete temp;
        }
    }
    ListNode<T>* getHead() {
        return head;
    }

    void push_front(T data) {
        ListNode<T>* new_node = new ListNode<T>;
        new_node->data = data;
        new_node->next = head;
        head = new_node;
        if (tail == nullptr) {
            tail = new_node;
        }
    }
    void push_back(T data) {
        ListNode<T>* new_node = new ListNode<T>;
        new_node->data = data;
        new_node->next = nullptr;
        if (tail != nullptr) {
            tail->next = new_node;
        }
        tail = new_node;
        if (head == nullptr) {
            head = new_node;
        }
    }
    T& front() {
        return head->data;
    }
    T& back() {
        return tail->data;
    }

    void erase(int index) {
        if (head == nullptr) {
            return;
        }
        if (index == 0) {
            ListNode<T>* temp = head;
            head = head->next;
            if (tail == temp) {
                tail = nullptr;
            }
            delete temp;
            return;
        }
        ListNode<T>* current = head;
        for (int i = 0; i < index - 1; i++) {
            if (current->next == nullptr) {
                return;
            }
            current = current->next;
        }
        ListNode<T>* temp = current->next;
        if (temp == nullptr) {
            return;
        }
        current->next = temp->next;
        if (tail == temp) {
            tail = current;
        }
        delete temp;
    }
};

// move the no longer busy barbers to the priorityqueue 
// if they have finished their service by 'time'
void freeUpBarbers(PriorityQueue& pq, LinkedList<Barber>& busyBarbers, unsigned int time) {
    ListNode<Barber>* current = busyBarbers.getHead();
    int i = 0;
    while (current != nullptr) {
        Barber b = current->data;
        if (b.lastServiceTime < time) {
            current = current->next;
            busyBarbers.erase(i);
            pq.push(b);
        }
        else {
            i++;
            current = current->next;
        }
    }
}

void freeAllBusyBarbers(PriorityQueue& pq, LinkedList<Barber>& busyBarbers) {
    ListNode<Barber>* current = busyBarbers.getHead();
    while (current != nullptr) {
        current = current->next;
        busyBarbers.erase(0);
    }
}

void freeEarliestBarbers(PriorityQueue& pq, LinkedList<Barber>& busyBarbers) {
    ListNode<Barber>* current = busyBarbers.getHead();

    unsigned int lowest = current->data.lastServiceTime;
    while (current != nullptr) {
        if (current->data.lastServiceTime < lowest) {
            lowest = current->data.lastServiceTime;
        }
    }

    freeUpBarbers(pq, busyBarbers, lowest);
}

//unsigned int getLatestServeTime(LinkedList<Barber>& busyBarbers) {
//    ListNode<Barber>* current = busyBarbers.getHead();
//
//    unsigned int result
//    
//}

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

    // binary search tree for the served clients
    BST clients;

    // busy barbers
    LinkedList<Barber> busyBarbers;

    // not served clients
    LinkedList<Client> notServedClients;

    unsigned int currentTime = 0;

    unsigned int arrivalTime = 0;

    fin >> arrivalTime;

    while (arrivalTime != 0) {

        unsigned int id = 0;
        unsigned int serviceLength = 0;

        fin >> id;
        fin >> serviceLength;

        Client c(id, arrivalTime, serviceLength);

        // when the current client arrives, we check if any busy 
        // barbers have completed the job at the current arrival time
        // then remove and add them to the pq

        freeUpBarbers(pq, busyBarbers, arrivalTime);

        // if there is a barber immediately available that can already serve the client
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
            busyBarbers.push_back(b);
        }
        else {
            {
                // if no barber is available immediately
                // find the one with earliest availability & priority 
                ListNode<Barber>* current = busyBarbers.getHead();
                Barber earliest = current->data;
                unsigned int earliestTime = earliest.earliestAvailabilityForService(arrivalTime, serviceLength);

                int i = 0;
                int earliestIndex = 0;

                while (current != nullptr) {
                    unsigned int currentTime = current->data.earliestAvailabilityForService(arrivalTime, serviceLength);
                    // earlier than the previous top one
                    if (currentTime < earliestTime) {
                        earliestTime = currentTime;
                        earliest = current->data;
                        earliestIndex = i++;
                        current = current->next;
                    }
                    // same availability but higher priority
                    else if (currentTime == earliestTime && current->data < earliest) {
                        earliestTime = currentTime;
                        earliest = current->data;
                        earliestIndex = i++;
                        current = current->next;
                    }
                    // skip
                    else {
                        i++;
                        current = current->next;
                    }
                }

                unsigned int end = earliestTime + serviceLength - 1;
                c.barberID = earliest.id;
                c.serviceTime = earliestTime;
                c.endTime = end;
                earliest.lastServiceTime = end;
                clients.insertClient(c);
                busyBarbers.erase(earliestIndex);
                busyBarbers.push_back(earliest);
            };
        }

        fin >> arrivalTime;
    }

    fin.close();

    
    busyBarbers.~LinkedList();
    notServedClients.~LinkedList();
    pq.~PriorityQueue();

    std::ofstream fout("hair.out");

    clients.printToFile(fout);

    return 0;
}
