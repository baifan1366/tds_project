#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <ctime>
#include <conio.h>
#include <cstdlib>
#include <sstream>

using namespace std;

// Custom minimum function
int min(int a, int b) {
    return (a < b) ? a : b;
}

double min(double a, double b) {
    return (a < b) ? a : b;
}

/**
 * Restaurant base class for the restaurant management system
 * Contains common functionality shared by inventory and menu subsystems
 * @time_complexity Varies by method
 * @space_complexity Varies by method
 */
class Restaurant {
protected:
    int itemCount;                     // Total number of items in the system
    
    // Protected constructor to prevent direct instantiation
    Restaurant() : itemCount(0) {}
    
    // Copy constructor
    Restaurant(const Restaurant& other) : itemCount(other.itemCount) {}
    
public:
    // Static utility methods for UI
    // Clearscreen function
    static void clearScreen() {
        system("cls");
    }
    
    // Print header for display
    static void printHeader(const string& title) {
        cout << "\n" << string(100, '=') << endl;
        cout << string(35, ' ') << title << endl;
        cout << string(100, '=') << endl;
    }
    
    // Print footer for display
    static void printFooter() {
        cout << string(100, '-') << endl;
    }

    // Virtual destructor to ensure proper cleanup in derived classes
    virtual ~Restaurant() {}
    
    // Get current number of items in the system
    virtual int getItemCount() const {
        return itemCount;
    }
    
    // Pure virtual functions to be implemented by derived classes
    virtual bool loadFromFile(const string& filename) = 0;
    virtual bool saveToFile(const string& filename, bool sorted = false) = 0;
    virtual void displayAll() = 0;
    virtual void displaySorted(bool byName = true) = 0;

    // Friend function to display system statistics
    friend void displayRestaurantStats(const Restaurant& restaurant);
};

// Food item structure for restaurant inventory
struct FoodItem {
    string id;            // Unique identifier
    string name;          // Food item name
    double price;         // Price per unit
    string category;      // Category (e.g., "Meat", "Vegetable", "Spice")
    int quantity;         // Quantity in stock
    string receiveDate;   // Date when item was received
    
    // Default constructor
    FoodItem() : price(0.0), quantity(0) {
        id = "";
        name = "";
        category = "";
        receiveDate = "";
    }
    
    // Parameterized constructor
    FoodItem(const string& _id, const string& _name, double _price, const string& _category, int _quantity = 0) 
        : id(_id), name(_name), price(_price), category(_category), quantity(_quantity) {
        // Set receive date to current time
        time_t now = time(0);
        string dateStr = ctime(&now);
        // Remove the newline character from ctime output
        if (!dateStr.empty() && dateStr[dateStr.length()-1] == '\n') {
            dateStr.erase(dateStr.length()-1);
        }
        receiveDate = dateStr;
    }
};

// Food Menu Item structure for restaurant menu 
struct MenuItem {
    string id;            // Unique identifier
    string name;          // Menu item name
    double price;         // Price for this menu item
    string description;   // Description of the menu item
    string category;      // Category (e.g., "Appetizer", "Main Course", "Dessert")
    
    // Ingredients needed for this menu item - format: "foodItemId:quantity"
    string* ingredients;  // Array of ingredient IDs with quantities
    int ingredientCount;  // Number of ingredients
    
    // Default constructor - initializes empty menu item
    MenuItem() : price(0.0), ingredientCount(0), ingredients(nullptr) {
        id = "";
        name = "";
        description = "";
        category = "";
    }
    
    // Parameterized constructor - creates menu item with basic information
    MenuItem(const string& _id, const string& _name, double _price, 
             const string& _description, const string& _category)
        : id(_id), name(_name), price(_price), description(_description), 
          category(_category), ingredientCount(0), ingredients(nullptr) {}
          
    // Copy constructor - performs deep copy of other MenuItem including ingredients
    MenuItem(const MenuItem& other) 
        : id(other.id), name(other.name), price(other.price), 
          description(other.description), category(other.category),
          ingredientCount(other.ingredientCount) {
        
        // Deep copy of ingredients array
        if (other.ingredientCount > 0 && other.ingredients != nullptr) {
            ingredients = new string[other.ingredientCount];
            for (int i = 0; i < other.ingredientCount; i++) {
                ingredients[i] = other.ingredients[i];
            }
        } else {
            ingredients = nullptr;
        }
    }
    
     // Assignment operator - handles proper deep copy during assignment
    MenuItem& operator=(const MenuItem& other) {
        // Self-assignment check to prevent issues
        if (this == &other) {
            return *this;
        }
        
        // Copy basic properties
        id = other.id;
        name = other.name;
        price = other.price;
        description = other.description;
        category = other.category;
        
        // Clean up existing ingredients to prevent memory leaks
        if (ingredients != nullptr) {
            delete[] ingredients;
            ingredients = nullptr;
        }
        
        ingredientCount = other.ingredientCount;
        
        // Deep copy the ingredients array
        if (other.ingredientCount > 0 && other.ingredients != nullptr) {
            ingredients = new string[other.ingredientCount];
            for (int i = 0; i < other.ingredientCount; i++) {
                ingredients[i] = other.ingredients[i];
            }
        }
        
        return *this;
    }
    
    // Destructor - ensures proper cleanup of dynamically allocated memory
    ~MenuItem() {
        if (ingredients != nullptr) {
            delete[] ingredients;
            ingredients = nullptr;
        }
    }
};

// Node structure for menu linked list
// Used as building blocks for the ADTLinkedList class
struct MenuNode {
    MenuItem data;       // The menu item stored in this node
    MenuNode* next;      // Pointer to the next node in the list
    
    // Constructor initializes node with a menu item and null next pointer
    MenuNode(const MenuItem& item) : data(item), next(nullptr) {}
};

// Singly Linked List for menu items
// Implements a dynamic list structure for storing and managing menu items
class ADTLinkedList {
private:
    MenuNode* head;      // Pointer to the first node in the list
    MenuNode* tail;      // Pointer to the last node in the list (for efficient append)
    int size;            // Current number of elements in the list

public:
    // Constructor - initializes an empty linked list
    ADTLinkedList() : head(nullptr), tail(nullptr), size(0) {}
    
    // Destructor - cleans up all dynamically allocated nodes to prevent memory leaks
    ~ADTLinkedList() {
        MenuNode* current = head;
        while (current != nullptr) {
            MenuNode* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
        tail = nullptr;
    }
    
    // Returns the current number of elements in the list
    int getSize() const {
        return size;
    }
    
    // Checks if the list contains no elements
    bool isEmpty() const {
        return head == nullptr;
    }
    
    // Adds a new menu item to the end of the list in O(1) time
    // Uses the tail pointer for efficient append operations
    void append(const MenuItem& item) {
        MenuNode* newNode = new MenuNode(item);
        
        if (head == nullptr) {
            // List is empty, set both head and tail to the new node
            head = tail = newNode;
        } else {
            // List has elements, append to the end
            tail->next = newNode;
            tail = newNode;
        }
        
        size++;
    }
    
    // Adds a new menu item to the beginning of the list in O(1) time
    void prepend(const MenuItem& item) {
        MenuNode* newNode = new MenuNode(item);
        
        if (head == nullptr) {
            // List is empty, set both head and tail to the new node
            head = tail = newNode;
        } else {
            // List has elements, prepend to the beginning
            newNode->next = head;
            head = newNode;
        }
        
        size++;
    }
    
    // Inserts a menu item at the specified position
    bool insertAt(int position, const MenuItem& item) {
        // Check if position is valid
        if (position < 0 || position > size) {
            return false;
        }
        
        // Special cases: insert at beginning or end
        if (position == 0) {
            prepend(item);
            return true;
        } else if (position == size) {
            append(item);
            return true;
        }
        
        // Insert in the middle of the list
        MenuNode* newNode = new MenuNode(item);
        MenuNode* current = head;
        
        // Navigate to the node before the insertion point
        for (int i = 0; i < position - 1; i++) {
            current = current->next;
        }
        
        // Insert the new node between current and current->next
        newNode->next = current->next;
        current->next = newNode;
        size++;
        
        return true;
    }
    
    // Removes a menu item by its ID
    bool removeById(const string& id) {
        if (head == nullptr) {
            return false;
        }
        
        // Special case: if the head node contains the target ID
        if (head->data.id == id) {
            MenuNode* temp = head;
            head = head->next;
            
            // Update tail if list becomes empty
            if (head == nullptr) {
                tail = nullptr;
            }
            
            delete temp;
            size--;
            return true;
        }
        
        // General case: search for the node with matching ID
        MenuNode* current = head;
        MenuNode* prev = nullptr;
        
        while (current != nullptr && current->data.id != id) {
            prev = current;
            current = current->next;
        }
        
        // If ID not found in the list
        if (current == nullptr) {
            return false;
        }
        
        // Link previous node to the node after current (bypass current)
        prev->next = current->next;
        
        // Update tail pointer if we're removing the last node
        if (current == tail) {
            tail = prev;
        }
        
        // Free memory and update size
        delete current;
        size--;
        return true;
    }
    
    // Searches for a menu item by ID and returns a copy if found
    MenuItem* findById(const string& id) const {
        MenuNode* current = head;
        
        // Traverse the list looking for matching ID
        while (current != nullptr) {
            if (current->data.id == id) {
                return new MenuItem(current->data);
            }
            current = current->next;
        }
        
        return nullptr; // Item not found
    }

    // Creates a dynamically allocated array containing all menu items
    MenuItem* toArray() const {
        if (isEmpty()) {
            return nullptr;
        }
        
        // Allocate an array of the exact size needed
        MenuItem* items = new MenuItem[size];
        MenuNode* current = head;
        int index = 0;
        
        // Copy each menu item into the array
        while (current != nullptr) {
            items[index++] = current->data;
            current = current->next;
        }
        
        return items;
    }
};

// Node structure for linked queue
// Basic building block for the ADTLinkedQueue class
struct Node {
    FoodItem data;      // The food item stored in this node
    Node* next;         // Pointer to the next node in the queue
    
    // Constructor initializes node with a food item and null next pointer
    Node(const FoodItem& item) : data(item), next(nullptr) {}
};

// Linked Queue for processing inventory items with FIFO order
// Implements a First-In-First-Out (FIFO) structure for food items
class ADTLinkedQueue {
private:
    Node *front, *rear;  // Pointers to the front and rear of the queue
    int size;            // Current number of elements in the queue

public:
    // Constructor - initializes an empty queue
    ADTLinkedQueue() : front(nullptr), rear(nullptr), size(0) {}
    
    // Copy constructor - performs deep copy of another queue
    ADTLinkedQueue(const ADTLinkedQueue& other) : front(nullptr), rear(nullptr), size(0) {
        // Copy nodes from other queue in order
        Node* current = other.front;
        while (current != nullptr) {
            enqueue(current->data);
            current = current->next;
        }
    }
    
    // Destructor - cleans up all nodes to prevent memory leaks
    ~ADTLinkedQueue() {
        while (!isEmpty()) {
            dequeue();
        }
    }
    
    // Checks if the queue is empty (contains no elements)
    bool isEmpty() const {
        return front == nullptr;
    }
    
    // Returns the current number of elements in the queue
    int getSize() const {
        return size;
    }
    
    // Adds a new food item to the end of the queue
    // Time complexity: O(1)
    void enqueue(const FoodItem& item) {
        Node* newNode = new Node(item);
        
        if (rear == nullptr) {
            // Queue is empty, set both front and rear to the new node
            front = rear = newNode;
        } else {
            // Queue has elements, add to the end
            rear->next = newNode;
            rear = newNode;
        }
        
        size++;
    }
    
    // Removes and returns the food item from the front of the queue
    // Returns an empty FoodItem if the queue is empty
    // Time complexity: O(1)
    FoodItem dequeue() {
        FoodItem item;
        
        if (!isEmpty()) {
            Node* temp = front;
            item = temp->data;
            
            // Move front pointer to the next node
            front = front->next;
            
            // If queue becomes empty, update rear pointer
            if (front == nullptr) {
                rear = nullptr;
            }
            
            // Free memory and update size
            delete temp;
            size--;
        }
        
        return item;
    }
    
    // Returns the food item at the front without removing it
    // Returns an empty FoodItem if the queue is empty
    FoodItem peek() const {
        if (!isEmpty()) {
            return front->data;
        }
        return FoodItem(); // Return empty item if queue is empty
    }

    // Creates a dynamically allocated array containing all food items in the queue
    // Returns nullptr if the queue is empty
    // Caller is responsible for deleting the returned array
    FoodItem* toArray() const {
        if (isEmpty()) {
            return nullptr;
        }
        
        // Allocate an array of the exact size needed
        FoodItem* items = nullptr;
        items = new FoodItem[size];
        Node* current = front;
        int index = 0;
        
        // Copy each food item into the array in queue order
        while (current != nullptr) {
            items[index++] = current->data;
            current = current->next;
        }
        
        return items;
    }
    
    // Assignment operator - performs deep copy of another queue
    // Properly handles self-assignment and memory management
    ADTLinkedQueue& operator=(const ADTLinkedQueue& other) {
        // Self-assignment check to prevent issues
        if (this == &other) {
            return *this; // if same then return same
        }
        
        // Clear current queue contents
        while (!isEmpty()) {
            dequeue();
        }
        
        // Copy nodes from other queue in order
        Node* current = other.front;
        while (current != nullptr) {
            enqueue(current->data);
            current = current->next;
        }
        
        return *this;
    }
};

// Universal Hash Function constants
const int PRIME = 31;
const int MAX_HASH_KEY = 101; // Prime number for hash table size

class RestaurantInventorySystem  : public Restaurant {
private:
    // In practice it always turns out that it is better to have an index range that is a prime number.
    // This way you do not get so many COLLISIONS.
    static const int TABLE_SIZE = 101; // Prime number for better hash distribution
    ADTLinkedQueue* hashTable;            // Array of linked queues (buckets)

    // Universal hash function for strings
    // Implements a polynomial rolling hash
    int universalHash(const string& key) {
        unsigned long hash = 0;
        int length = key.length();
        
        // Universal hash function: h(k) = ((a*k + b) mod p) mod m
        // hash = ((hash * PRIME + key[i]) % MAX_HASH_KEY) % TABLE_SIZE;
        // Using polynomial rolling hash: h(k) = sum(k[i]*PRIME^(n-i-1)) mod TABLE_SIZE
        for (int i = 0; i < length; i++) {
            hash = (hash * PRIME + key[i]) % MAX_HASH_KEY;
        }
        
        return hash % TABLE_SIZE;
    }
    
    // Quadratic probing function for collision resolution
    int quadraticProbing(int hashValue, int attempt) {
        // Quadratic probing formula: h'(k, i) = (h(k) + c1*i + c2*i^2) mod TABLE_SIZE
        // Using c1=0, c2=1: h'(k, i) = (h(k) + i^2) mod TABLE_SIZE
        return (hashValue + attempt * attempt) % TABLE_SIZE;
    }

    // Find position using quadratic probing
    int findPosition(const string& id) {
        int hashValue = universalHash(id);
        int attempt = 0;
        
        while (attempt < TABLE_SIZE) {
            int position = quadraticProbing(hashValue, attempt);
            ADTLinkedQueue queue = hashTable[position];
            
            if (queue.isEmpty()) {
                return position; // Empty bucket found
            }
            
            // Check if item exists in this bucket
            FoodItem* items = queue.toArray();
            bool found = false;
            
            for (int i = 0; i < queue.getSize(); i++) {
                if (items[i].id == id) {
                    found = true;
                    delete[] items;
                    return position;
                }
            }
            
            delete[] items;
            
            if (!found) {
                attempt++;
            } else {
                return position;
            }
        }
        
        // If table is full, return -1
        return -1;
    }
    
public:
    // Make TABLE_SIZE accessible publicly
    static const int MAX_BUCKETS = 101; // Prime number Same as TABLE_SIZE
    
    // Static utility methods
    // Clearscreen function
    static void clearScreen() {
        system("cls");
    }
    
    // Print header for display
    static void printHeader(const string& title) {
        cout << "\n" << string(100, '=') << endl;
        cout << string(35, ' ') << title << endl;
        cout << string(100, '=') << endl;
    }
    
    // Print footer for display
    static void printFooter() {
        cout << string(100, '-') << endl;
    }
        
    // Merge function (part of Tim Sort)
    // Merges two sorted subarrays into one sorted array
    // Parameters: array, left boundary, middle point, right boundary, and sort criteria
    static void merge(FoodItem arr[], int left, int mid, int right, bool byName) {
        int len1 = mid - left + 1;
        int len2 = right - mid;
        
        // Create temporary arrays to hold the two subarrays
        FoodItem* leftArr = new FoodItem[len1];
        FoodItem* rightArr = new FoodItem[len2];
        
        // Copy data to temporary arrays
        for (int i = 0; i < len1; i++)
            leftArr[i] = arr[left + i];
        for (int i = 0; i < len2; i++)
            rightArr[i] = arr[mid + 1 + i];
        
        // Merge the temporary arrays back into the original array
        int i = 0, j = 0, k = left;
        
        while (i < len1 && j < len2) {
            if (byName) {
                // Sort by name (alphabetical order)
                if (leftArr[i].name <= rightArr[j].name) {
                    arr[k] = leftArr[i];
                    i++;
                } else {
                    arr[k] = rightArr[j];
                    j++;
                }
            } else {
                // Sort by quantity (numerical order)
                if (leftArr[i].quantity <= rightArr[j].quantity) {
                    arr[k] = leftArr[i];
                    i++;
                } else {
                    arr[k] = rightArr[j];
                    j++;
                }
            }
            k++;
        }
        
        // Copy any remaining elements from the left subarray
        while (i < len1) {
            arr[k] = leftArr[i];
            i++;
            k++;
        }
        
        // Copy any remaining elements from the right subarray
        while (j < len2) {
            arr[k] = rightArr[j];
            j++;
            k++;
        }
        
        // Free allocated memory for temporary arrays
        delete[] leftArr;
        delete[] rightArr;
    }

    // Insertion sort (part of Tim Sort)
    // Efficiently sorts small subarrays using in-place insertion sort
    // Parameters: array, left boundary, right boundary, and sort criteria
    static void insertionSort(FoodItem arr[], int left, int right, bool byName) {
        for (int i = left + 1; i <= right; i++) {
            // Store current element as temporary
            FoodItem temp = arr[i];
            int j = i - 1;
            
            if (byName) {
                // Sort by name (alphabetical order)
                // Move elements greater than temp to one position ahead
                while (j >= left && arr[j].name > temp.name) {
                    arr[j + 1] = arr[j];
                    j--;
                }
            } else {
                // Sort by quantity (numerical order)
                // Move elements greater than temp to one position ahead
                while (j >= left && arr[j].quantity > temp.quantity) {
                    arr[j + 1] = arr[j];
                    j--;
                }
            }
            
            // Place temp in its correct position
            arr[j + 1] = temp;
        }
    }

    // Tim Sort main function
    // A hybrid sorting algorithm combining insertion sort and merge sort
    // Average time complexity: O(n log n)
    static void timSort(FoodItem arr[], int n, bool byName = true) {
        // Run size for insertion sort (optimal value based on typical array sizes)
        const int RUN = 32;
        
        // First, sort individual subarrays of size RUN using insertion sort
        // This is efficient for small arrays and creates sorted runs
        for (int i = 0; i < n; i += RUN) {
            insertionSort(arr, i, min(i + RUN - 1, n - 1), byName);
        }
        
        // Start merging sorted runs from size RUN
        // Each iteration doubles the size of subarrays being merged
        for (int size = RUN; size < n; size = 2 * size) {
            // Pick starting points of different subarrays of current size
            for (int left = 0; left < n; left += 2 * size) {
                // Find ending points of subarrays
                int mid = left + size - 1;
                int right = min(left + 2 * size - 1, n - 1);
                
                // Merge subarrays arr[left...mid] and arr[mid+1...right]
                if (mid < right) {
                    merge(arr, left, mid, right, byName);
                }
            }
        }
    }
    
    // Interpolation Search - O(log log n) average case for uniformly distributed data
    // A searching algorithm that works on uniformly distributed sorted data
    // Parameters: sorted array, array size, and ID to search for
    static int interpolationSearch(FoodItem arr[], int n, const string& id) {
        // Find indexes of two corners (boundary points)
        int low = 0, high = n - 1;
        
        // While elements exist and key is within range of array
        while (low <= high && id >= arr[low].id && id <= arr[high].id) {
            // Probing the position with key value using interpolation formula
            // This estimates where the element might be based on its value
            double val1 = id.compare(arr[low].id);
            double val2 = arr[high].id.compare(arr[low].id);
            double val3 = high - low;
            int pos = low + (int)(val3 * (val1 / val2));
            
            // If element is found at the estimated position
            if (arr[pos].id == id)
                return pos;
                
            // If element at pos is smaller, search in right subarray
            if (arr[pos].id < id)
                low = pos + 1;
            else
                // If element at pos is larger, search in left subarray
                high = pos - 1;
        }
        
        return -1; // Element not found
    }

    // Constructor - initializes the hash table for storing food items
    // Creates an array of empty linked queues (buckets)
    RestaurantInventorySystem() : Restaurant() {
        hashTable = new ADTLinkedQueue[TABLE_SIZE];
    }
    
    // Destructor - frees all memory allocated for the hash table
    virtual ~RestaurantInventorySystem() override {
        delete[] hashTable;
    }

    // Loads food items from a TXT file into the inventory system
    // Returns true if at least one item was successfully loaded
    virtual bool loadFromFile(const string& filename) override {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not open file " << filename << endl;
            return false;
        }
        
        // Clear existing items to prevent duplicates
        for (int i = 0; i < TABLE_SIZE; i++) {
            while (!hashTable[i].isEmpty()) {
                hashTable[i].dequeue();
            }
        }
        itemCount = 0;
        
        string line;
        int itemsLoaded = 0;
        
        // Process each line in the file
        while (getline(file, line)) {
            string id, name, category, receiveDate;
            double price;
            int quantity;
            
            // Parse TXT line (format: id,name,price,category,quantity,receiveDate)
            size_t pos = 0;
            string token;
            
            // Extract ID
            pos = line.find(",");
            if (pos == string::npos) continue; // Skip malformed lines
            id = line.substr(0, pos);
            line.erase(0, pos + 1);
            
            // Extract Name
            pos = line.find(",");
            if (pos == string::npos) continue; // Skip malformed lines
            name = line.substr(0, pos);
            line.erase(0, pos + 1);
            
            // Extract Price
            pos = line.find(",");
            if (pos == string::npos) continue; // Skip malformed lines
            price = stod(line.substr(0, pos));
            line.erase(0, pos + 1);
            
            // Extract Category
            pos = line.find(",");
            if (pos == string::npos) continue; // Skip malformed lines
            category = line.substr(0, pos);
            line.erase(0, pos + 1);
            
            // Extract Quantity and optional Receive Date
            pos = line.find(",");
            if (pos == string::npos) {
                quantity = stoi(line);
                receiveDate = ""; // No receive date in file
            } else {
                quantity = stoi(line.substr(0, pos));
                line.erase(0, pos + 1);
                // Extract Receive Date
                receiveDate = line;
            }
            
            // Create food item and insert into hash table
            FoodItem item(id, name, price, category, quantity);
            if (!receiveDate.empty()) {
                item.receiveDate = receiveDate;
            }
            
            // Always add as a new batch (like addExistingFoodItem does)
            // This ensures all items from the file are loaded, including multiple batches of the same ID
            int position = findPosition(id);
            if (position != -1) {
                // Add directly to the queue at the found position
                hashTable[position].enqueue(item);
                
                // Increment item count only for new unique IDs
                bool isNewId = true;
                for (int i = 0; i < position; i++) {
                    if (!hashTable[i].isEmpty()) {
                        ADTLinkedQueue tempQueue = hashTable[i];
                        FoodItem* queueItems = tempQueue.toArray();
                        for (int j = 0; j < tempQueue.getSize(); j++) {
                            if (queueItems[j].id == id) {
                                isNewId = false;
                                break;
                            }
                        }
                        delete[] queueItems;
                        if (!isNewId) break;
                    }
                }
                
                for (int i = position + 1; i < TABLE_SIZE && isNewId; i++) {
                    if (!hashTable[i].isEmpty()) {
                        ADTLinkedQueue tempQueue = hashTable[i];
                        FoodItem* queueItems = tempQueue.toArray();
                        for (int j = 0; j < tempQueue.getSize(); j++) {
                            if (queueItems[j].id == id) {
                                isNewId = false;
                                break;
                            }
                        }
                        delete[] queueItems;
                        if (!isNewId) break;
                    }
                }
                
                if (isNewId) {
                    this->itemCount++;
                }
                
                itemsLoaded++;
            }
        }
        
        file.close();
        cout << "Successfully loaded " << itemsLoaded << " food items from " << filename << endl;
        return itemsLoaded > 0;
    }

    // Saves all food items in the inventory to a TXT file
    // Optional sorting by name can be applied before saving
    virtual bool saveToFile(const string& filename, bool sorted = false) override {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not open file " << filename << " for writing" << endl;
            return false;
        }
        
        // First, count the total number of items across all buckets
        int totalItems = 0;
        for (int i = 0; i < TABLE_SIZE; i++) {
            if (!hashTable[i].isEmpty()) {
                totalItems += hashTable[i].getSize();
            }
        }
        
        // Get all food items as an array
        FoodItem* items = getAllItems();
        
        // Check if items were properly allocated
        if (items == nullptr && totalItems > 0) {
            cout << "Error: Failed to allocate memory for food items." << endl;
            file.close();
            return false;
        }
        
        // Sort items if requested
        if (sorted && totalItems > 0 && items != nullptr) {
            timSort(items, totalItems, true); // Sort by name
        }
        
        // Write to file
        if (totalItems > 0 && items != nullptr) {
            for (int i = 0; i < totalItems; i++) {
                file << items[i].id << ","
                     << items[i].name << ","
                     << items[i].price << ","
                     << items[i].category << ","
                     << items[i].quantity << ","
                     << items[i].receiveDate << endl;
            }
            delete[] items;
            cout << "Successfully saved " << totalItems << " food items to " << filename << endl;
        } else {
            // Write an empty file if no items
            cout << "No items to save to file." << endl;
        }
        
        file.close();
        return true;
    }

    // Insert a food item into the hash table
    // Parameters: food item to insert
    // Returns: true if successful, false if failed
    bool insertFoodItem(const FoodItem& item) {
        if (item.id.empty()) {
            cout << "Error: Food item ID cannot be empty." << endl;
            return false;
        }
        
        // Find position using hash function and quadratic probing
        int position = findPosition(item.id);
        if (position == -1) {
            cout << "Error: Hash table is full." << endl;
            return false;
        }
        
        // Check if item with this ID already exists
        FoodItem* existingItem = findFoodItem(item.id);
        if (existingItem != nullptr) {
            // Update quantity if item exists (combine quantities)
            FoodItem updated = *existingItem;
            updated.quantity += item.quantity;
            
            // Remove existing item and add updated one to maintain consistency
            removeFoodItem(item.id);
            hashTable[position].enqueue(updated);
            
            delete existingItem;
        } else {
            // Add new item to the appropriate bucket
            hashTable[position].enqueue(item);
            this->itemCount++;
        }
        
        return true;
    }    

    // Find a food item by ID
    // Parameters: ID to search for
    // Returns: pointer to a copy of the found item or nullptr if not found
    // Note: Caller is responsible for deleting the returned pointer
    FoodItem* findFoodItem(const string& id) {
        // Start with initial hash position
        int position = universalHash(id);
        int attempt = 0;
        
        // Try all possible positions using quadratic probing
        while (attempt < TABLE_SIZE) {
            int probePos = quadraticProbing(position, attempt);
            
            // If the bucket at this position is not empty
            if (!hashTable[probePos].isEmpty()) {
                // Search in the queue at this bucket
                ADTLinkedQueue tempQueue = hashTable[probePos];
                FoodItem* items = tempQueue.toArray();
                int size = tempQueue.getSize();
                
                // Use interpolation search for larger arrays (optimization)
                if (size > 1) {
                    // Create a sorted copy for interpolation search
                    FoodItem* sortedItems = new FoodItem[size];
                    for (int i = 0; i < size; i++) {
                        sortedItems[i] = items[i];
                    }
                    
                    // Sort by id for interpolation search (simple bubble sort)
                    for (int i = 0; i < size; i++) {
                        for (int j = i + 1; j < size; j++) {
                            if (sortedItems[i].id > sortedItems[j].id) {
                                FoodItem temp = sortedItems[i];
                                sortedItems[i] = sortedItems[j];
                                sortedItems[j] = temp;
                            }
                        }
                    }
                    
                    // Find using interpolation search (faster than linear for sorted data)
                    int pos = interpolationSearch(sortedItems, size, id);
                    if (pos != -1) {
                        // Create a copy of the found item
                        FoodItem* result = new FoodItem(sortedItems[pos]);
                        delete[] items;
                        delete[] sortedItems;
                        return result;
                    }
                    
                    delete[] sortedItems;
                } else {
                    // Linear search for small arrays (more efficient for few items)
                    for (int i = 0; i < size; i++) {
                        if (items[i].id == id) {
                            // Create a copy of the found item
                            FoodItem* result = new FoodItem(items[i]);
                            delete[] items;
                            return result;
                        }
                    }
                }
                
                // Clean up allocated memory
                delete[] items;
            }
            
            // Try next position using quadratic probing
            attempt++;
        }
        
        return nullptr; // Item not found after checking all possible positions
    }
    
    // Remove a food item by ID
    // Parameters: ID of the item to remove
    // Returns: true if found and removed, false if item not found
    bool removeFoodItem(const string& id) {
        // Start with initial hash position
        int position = universalHash(id);
        int attempt = 0;
        
        // Try all possible bucket positions using quadratic probing
        while (attempt < TABLE_SIZE) {
            int probePos = quadraticProbing(position, attempt);
            
            // Found a non-empty bucket, check if our item is here
            if (!hashTable[probePos].isEmpty()) {
                // Create a new queue that will contain all items except the one to remove
                ADTLinkedQueue tempQueue = hashTable[probePos];
                ADTLinkedQueue newQueue;
                bool found = false;
                
                // Process each item in the bucket
                while (!tempQueue.isEmpty()) {
                    FoodItem item = tempQueue.dequeue();
                    
                    // Keep all items except the one matching the ID we want to remove
                    if (item.id != id) {
                        newQueue.enqueue(item);
                    } else {
                        found = true;
                        this->itemCount--;
                    }
                }
                
                // If item was found and removed, update the bucket with the new queue
                if (found) {
                    hashTable[probePos] = newQueue;
                    return true;
                }
            }
            
            // Try next position
            attempt++;
        }
        
        return false; // Item not found after checking all possible positions
    }    

    // Display all food items (unsorted)
    // Shows a formatted table of all items in the inventory
    virtual void displayAll() override {
        // Print table header with title
        printHeader("Restaurant Inventory System - All Items");
        cout << left << setw(10) << "ID" 
             << setw(30) << "Name" 
             << setw(10) << "Price" 
             << setw(15) << "Category" 
             << setw(10) << "Quantity" 
             << setw(25) << "Receive Date" << endl;
        printFooter();
        
        // Count the total number of items across all buckets
        int totalItems = 0;
        for (int i = 0; i < TABLE_SIZE; i++) {
            if (!hashTable[i].isEmpty()) {
                totalItems += hashTable[i].getSize();
            }
        }
        
        // Get all items from the hash table
        FoodItem* items = getAllItems();
        
        // Handle empty inventory case
        if (totalItems == 0) {
            cout << "No items in inventory." << endl;
        } else {
            // Display each item with formatted columns
            for (int i = 0; i < totalItems; i++) {
                cout << left << setw(10) << items[i].id 
                     << setw(30) << items[i].name 
                     << setw(10) << fixed << setprecision(2) << items[i].price
                     << setw(15) << items[i].category
                     << setw(10) << items[i].quantity 
                     << setw(25) << items[i].receiveDate << endl;
            }
        }
        
        // Clean up allocated memory
        delete[] items;
    }    

    // Display all food items sorted by name or quantity
    // Parameter: byName - if true, sort by name; if false, sort by quantity
    virtual void displaySorted(bool byName = true) override {
        // Print table header with appropriate title based on sort criteria
        printHeader(byName ? "Restaurant Inventory System - Sorted by Name" 
                           : "Restaurant Inventory System - Sorted by Quantity (Total)");
        
        // Print column headers
        cout << left << setw(10) << "ID" 
             << setw(30) << "Name" 
             << setw(10) << "Price" 
             << setw(15) << "Category" 
             << setw(10) << "Quantity" 
             << setw(25) << "Receive Date" << endl;
        printFooter();
        
        // Get all items as a array from the hash table
        FoodItem* items = getAllItems();
        
        // Count the total number of items we got
        int totalItemCount = 0;
        for (int i = 0; i < TABLE_SIZE; i++) {
            if (!hashTable[i].isEmpty()) {
                totalItemCount += hashTable[i].getSize();
            }
        }
        
        // Handle empty inventory case
        if (totalItemCount == 0) {
            cout << "No items in inventory." << endl;
        } else {
            if (byName) {
                // Create a copy of the items array for sorting (preserve original)
                FoodItem* sortedItems = new FoodItem[totalItemCount];
                for (int i = 0; i < totalItemCount; i++) {
                    sortedItems[i] = items[i];
                }
                
                // Sort using Tim Sort algorithm based on name
                timSort(sortedItems, totalItemCount, true);
                
                // Display each item with formatted columns
                for (int i = 0; i < totalItemCount; i++) {
                    cout << left << setw(10) << sortedItems[i].id 
                         << setw(30) << sortedItems[i].name 
                         << setw(10) << fixed << setprecision(2) << sortedItems[i].price
                         << setw(15) << sortedItems[i].category
                         << setw(10) << sortedItems[i].quantity 
                         << setw(25) << sortedItems[i].receiveDate << endl;
                }
                
                // Clean up allocated memory for sorted array
                delete[] sortedItems;
            } else {
                // When sorting by quantity, we need to aggregate quantities for items with the same ID

                // First, identify all unique IDs
                int uniqueCount = 0;
                string* uniqueIds = new string[totalItemCount];
                
                // Find unique IDs
                for (int i = 0; i < totalItemCount; i++) {
                    bool isUnique = true;
                    
                    // Check if this ID is already in our uniqueIds array
                    for (int j = 0; j < uniqueCount; j++) {
                        if (uniqueIds[j] == items[i].id) {
                            isUnique = false;
                            break;
                        }
                    }
                    
                    // If it's a new ID, add it to our unique IDs array
                    if (isUnique) {
                        uniqueIds[uniqueCount++] = items[i].id;
                    }
                }
                
                // Create an array to hold the aggregated items
                FoodItem* aggregatedItems = new FoodItem[uniqueCount];
                
                // Initialize the aggregated items array with the first occurrence of each ID
                for (int i = 0; i < uniqueCount; i++) {
                    for (int j = 0; j < totalItemCount; j++) {
                        if (items[j].id == uniqueIds[i]) {
                            aggregatedItems[i] = items[j];
                            break;
                        }
                    }
                }
                
                // Sum up quantities for items with the same ID
                for (int i = 0; i < uniqueCount; i++) {
                    int totalQuantity = 0;
                    
                    // Sum quantities of all items with this ID
                    for (int j = 0; j < totalItemCount; j++) {
                        if (items[j].id == uniqueIds[i]) {
                            totalQuantity += items[j].quantity;
                        }
                    }
                    
                    // Update the quantity in our aggregated items array
                    aggregatedItems[i].quantity = totalQuantity;
                }
                
                // Sort using Tim Sort algorithm based on quantity
                timSort(aggregatedItems, uniqueCount, false);
                
                // Display each item with formatted columns
                for (int i = 0; i < uniqueCount; i++) {
                    cout << left << setw(10) << aggregatedItems[i].id 
                         << setw(30) << aggregatedItems[i].name 
                         << setw(10) << fixed << setprecision(2) << aggregatedItems[i].price
                         << setw(15) << aggregatedItems[i].category
                         << setw(10) << aggregatedItems[i].quantity 
                         << "(Total)" << endl;
                }
                
                // Clean up allocated memory
                delete[] uniqueIds;
                delete[] aggregatedItems;
            }
        }
        
        // Clean up allocated memory for original array
        delete[] items;
    }

    // Get all food items as an array from across all hash table buckets
    // Returns: dynamically allocated array of FoodItem objects
    // Note: Caller is responsible for deleting the returned array
    FoodItem* getAllItems() {
        // First, count the total number of items across all buckets
        int totalItems = 0;
        for (int i = 0; i < TABLE_SIZE; i++) {
            if (!hashTable[i].isEmpty()) {
                totalItems += hashTable[i].getSize();
            }
        }
        
        // Check if there are any items to return
        if (totalItems <= 0) {
            return nullptr;
        }
        
        // Try to allocate memory with exception handling to prevent crashes
        FoodItem* items = nullptr;
        try {
            // Allocate memory for the array to hold all items based on the actual count
            items = new FoodItem[totalItems];
            int index = 0;
            
            // Iterate through all hash table buckets
            for (int i = 0; i < TABLE_SIZE; i++) {
                // Process non-empty buckets only
                if (!hashTable[i].isEmpty()) {
                    // Get all items in the current bucket as an array
                    ADTLinkedQueue tempQueue = hashTable[i];
                    FoodItem* queueItems = tempQueue.toArray();
                    
                    // Copy each item from the bucket to the main array
                    if (queueItems != nullptr) {
                        for (int j = 0; j < tempQueue.getSize(); j++) {
                            if (index < totalItems) {
                                items[index++] = queueItems[j];
                            }
                        }
                        
                        // Clean up the temporary array
                        delete[] queueItems;
                    }
                }
            }
        } catch (const std::bad_alloc&) {
            // Handle memory allocation failure
            cout << "Memory allocation failed in getAllItems()" << endl;
            return nullptr;
        }
        
        return items;
    }
    
    // Search and display food item by ID
    // Presents formatted information about the item if found
    void searchById(const string& id) {
        // Find the item using the hash table lookup
        FoodItem* item = findFoodItem(id);
        
        // Print table header
        printHeader("Search Result");
        cout << left << setw(10) << "ID" 
             << setw(30) << "Name" 
             << setw(10) << "Price" 
             << setw(15) << "Category" 
             << setw(10) << "Quantity" 
             << setw(25) << "Receive Date" << endl;
        printFooter();
        
        // Display item if found, otherwise show not found message
        if (item != nullptr) {
            // Format and display the item details
            cout << left << setw(10) << item->id 
                 << setw(30) << item->name 
                 << setw(10) << fixed << setprecision(2) << item->price
                 << setw(15) << item->category
                 << setw(10) << item->quantity 
                 << setw(25) << item->receiveDate << endl;
            delete item;  // Clean up allocated memory
        } else {
            cout << "Item with ID " << id << " not found." << endl;
        }
    }
};

// Utility sorting and searching functions for restaurant menu system

// Merge function (part of Tim Sort) for MenuItem objects
// Merges two sorted subarrays into one sorted array
// Parameters: array, left boundary, middle point, right boundary, sort criteria (name, price, or category)
void mergeMenuItems(MenuItem arr[], int left, int mid, int right, const string& sortBy = "name") {
    int len1 = mid - left + 1;
    int len2 = right - mid;
    
    // Create temporary arrays to hold the two subarrays
    MenuItem* leftArr = new MenuItem[len1];
    MenuItem* rightArr = new MenuItem[len2];
    
    // Copy data to temporary arrays
    for (int i = 0; i < len1; i++)
        leftArr[i] = arr[left + i];
    for (int i = 0; i < len2; i++)
        rightArr[i] = arr[mid + 1 + i];
    
    // Merge the temporary arrays back into the original array
    int i = 0, j = 0, k = left;
    
    while (i < len1 && j < len2) {
        bool compareResult = false;
        
        // Compare based on the sort criteria
        if (sortBy == "price") {
            // Sort by price (ascending order)
            compareResult = (leftArr[i].price <= rightArr[j].price);
        } 
        else if (sortBy == "category") {
            // Sort by category (alphabetical order)
            compareResult = (leftArr[i].category <= rightArr[j].category);
        }
        else {
            // Default: Sort by name (alphabetical order)
            compareResult = (leftArr[i].name <= rightArr[j].name);
        }
        
        if (compareResult) {
            arr[k] = leftArr[i];
            i++;
        } else {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }
    
    // Copy any remaining elements from the left subarray
    while (i < len1) {
        arr[k] = leftArr[i];
        i++;
        k++;
    }
    
    // Copy any remaining elements from the right subarray
    while (j < len2) {
        arr[k] = rightArr[j];
        j++;
        k++;
    }
    
    // Free allocated memory for temporary arrays
    delete[] leftArr;
    delete[] rightArr;
}

// Insertion sort (part of Tim Sort) for MenuItem objects
// Efficiently sorts small subarrays using in-place insertion sort
// Parameters: array, left boundary, right boundary, sort criteria (name, price, or category)
void insertionSortMenuItems(MenuItem arr[], int left, int right, const string& sortBy = "name") {
    for (int i = left + 1; i <= right; i++) {
        // Store current element as temporary
        MenuItem temp = arr[i];
        int j = i - 1;
        
        if (sortBy == "price") {
            // Sort by price (ascending order)
            while (j >= left && arr[j].price > temp.price) {
                arr[j + 1] = arr[j];
                j--;
            }
        } 
        else if (sortBy == "category") {
            // Sort by category (alphabetical order)
            while (j >= left && arr[j].category > temp.category) {
                arr[j + 1] = arr[j];
                j--;
            }
        }
        else {
            // Default: Sort by name (alphabetical order)
            while (j >= left && arr[j].name > temp.name) {
                arr[j + 1] = arr[j];
                j--;
            }
        }
        
        // Place temp in its correct position
        arr[j + 1] = temp;
    }
}

// Tim Sort main function for MenuItem objects
// A hybrid sorting algorithm combining insertion sort and merge sort
// Average time complexity: O(n log n)
// Parameters: array, array size, sort criteria (name, price, or category)
void timSortMenuItems(MenuItem arr[], int n, const string& sortBy = "name") {
    // Run size for insertion sort (optimal value based on typical array sizes)
    const int RUN = 32;
    
    // First, sort individual subarrays of size RUN using insertion sort
    // This is efficient for small arrays and creates sorted runs
    for (int i = 0; i < n; i += RUN) {
        insertionSortMenuItems(arr, i, min(i + RUN - 1, n - 1), sortBy);
    }
    
    // Start merging sorted runs from size RUN
    // Each iteration doubles the size of subarrays being merged
    for (int size = RUN; size < n; size = 2 * size) {
        // Pick starting points of different subarrays of current size
        for (int left = 0; left < n; left += 2 * size) {
            // Find ending points of subarrays
            int mid = left + size - 1;
            int right = min(left + 2 * size - 1, n - 1);
            
            // Merge subarrays arr[left...mid] and arr[mid+1...right]
            if (mid < right) {
                mergeMenuItems(arr, left, mid, right, sortBy);
            }
        }
    }
}


class RestaurantMenuSystem {
    private:
        ADTLinkedList menuList;  // Linked list to store menu items
        int itemCount;           // Total number of menu items

    public:
        // Constructor - initializes an empty menu system
        RestaurantMenuSystem() : itemCount(0) {}
        
        // Destructor - linked list handles its own memory cleanup
        ~RestaurantMenuSystem() {}
};

int main()
{
    return 0;
}

