#include <iostream>
#include <ctime>

using namespace std;
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
};

// Universal Hash Function constants
const int PRIME = 31;
const int MAX_HASH_KEY = 101; // Prime number for hash table size

class RestaurantInventorySystem {
private:
    // In practice it always turns out that it is better to have an index range that is a prime number.
    // This way you do not get so many COLLISIONS.
    static const int TABLE_SIZE = 101; // Prime number for better hash distribution
    ADTLinkedQueue* hashTable;            // Array of linked queues (buckets)
    int itemCount;

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
};

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

