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

struct earliestAvailabilityPair {
    Barber* b;
    unsigned int earliestTime;
    earliestAvailabilityPair(Barber* barber, unsigned int time) {
        this->b = barber;
        this->earliestTime = time;
    }
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

    Barber remove(const Barber& b) {
        PQNode* curr = head;
        PQNode* prev = nullptr;
        Barber cpy = b;

        while (curr != nullptr && !(curr->data == b)) {
            prev = curr;
            curr = curr->next;
        }

        if (curr == nullptr) {
            // Barber not found in the queue
            return b;
        }

        if (prev == nullptr) {
            // Barber is at the head of the queue
            head = curr->next;
        }
        else {
            prev->next = curr->next;
        }

        delete curr;

        return cpy; 
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
    Barber* topCanComplete(unsigned int startTime, unsigned int serviceLength) {
        PQNode* current = head;

        while (current != nullptr) {
            if (current->data.canComplete(startTime, serviceLength)) {
                return &(current->data);
            }

            current = current->next;
        }

        return nullptr;
    }

    earliestAvailabilityPair earliestAvailability(unsigned int startTime, unsigned int serviceLength) {
        PQNode* current = head;

        Barber* earliest = &(current->data);
        unsigned int earliestTime = earliest->earliestAvailabilityForService(startTime, serviceLength);

        while (current != nullptr) {
            unsigned int currentTime = current->data.earliestAvailabilityForService(startTime, serviceLength);

            // earlier than previous one
            if (currentTime < earliestTime) {
                earliestTime = currentTime;
                earliest = &(current->data);
            }
            // same time but higher priority
            else if (currentTime == earliestTime && current->data < *earliest) {
                earliestTime = currentTime;
                earliest = &(current->data);
            }
            current = current->next;    
        }
        
        return earliestAvailabilityPair(earliest, earliestTime);
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
    unsigned int endTime;
    unsigned int barberID;


    Client() {
        this->id = 0;
        this->endTime = 0;
        this->barberID = 0;
    }

    Client(int id, int endTime, int barberId) : id(id), endTime(endTime), barberID(barberId) {}

    Client(const Client& other) : id(other.id), endTime(other.endTime), barberID(other.barberID) {}

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

    unsigned int currentTime = 0;

    unsigned int arrivalTime = 0;

    fin >> arrivalTime;

    while (arrivalTime != 0) {

        unsigned int id = 0;
        unsigned int serviceLength = 0;

        fin >> id;
        fin >> serviceLength;

        // if there is a barber immediately available that can already serve the client
        // then we assign
        Barber* b = pq.topCanComplete(arrivalTime, serviceLength);
        if (!(b == nullptr)) {
            unsigned int end = arrivalTime + serviceLength - 1;
            clients.insertClient(Client(id, end, b->id));
            b->lastServiceTime = end;
            pq.push(pq.remove(*b));
        }
        else {
            // if no barber is available immediately
            // find the one with earliest availability & priority

            earliestAvailabilityPair pair = pq.earliestAvailability(arrivalTime, serviceLength);

            // something has gone wrong because by this would mean there are no barbers in the pq
            if ((pair.b == nullptr)) {
                return -1; 
            }

            unsigned int end = pair.earliestTime + serviceLength - 1;
            clients.insertClient(Client(id, end, pair.b->id));
            pair.b->lastServiceTime = end;
            pq.push(pq.remove(*pair.b));
        }

        fin >> arrivalTime;
    }

    fin.close();

    std::ofstream fout("hair.out");

    clients.printToFile(fout);

    fout.close();

    return 0;
}
