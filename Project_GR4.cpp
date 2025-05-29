#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <ctime>
#include <conio.h>
#include <cstdlib>
#include <sstream>

using namespace std;

/**
 * ValidationCheck class provides static methods for validating various inputs
 * This class contains utility methods for common validation tasks throughout the application
 */
class ValidationCheck {
public:
    // Check if string length is within acceptable range
    static bool isValidStringLength(const string& str, int minLength, int maxLength) {
        int length = str.length();
        return (length >= minLength && length <= maxLength);
    }
    
    // Check if price is valid (minimum 1.00 and has exactly 2 decimal places)
    static bool isValidPrice(double price) {
        // Check minimum price
        if (price < 1.0) {
            return false;
        }
        
        // Check if price has exactly 2 decimal places
        // Convert to string and check decimal part
        string priceStr = to_string(price);
        size_t decimalPos = priceStr.find('.');
        
        if (decimalPos == string::npos) {
            // No decimal point
            return true;
        }
        
        // Get decimal part and check its length after removing trailing zeros
        string decimalPart = priceStr.substr(decimalPos + 1);
        while (!decimalPart.empty() && decimalPart.back() == '0') {
            decimalPart.pop_back();
        }
        
        return decimalPart.length() <= 2;
    }
    
    // Check if quantity is valid (between 1 and 999)
    static bool isValidQuantity(int quantity) {
        return (quantity >= 1 && quantity <= 999);
    }
    
    // Check if ID follows the format of 1 character followed by 3 digits
    static bool isValidID(const string& id) {
        // Check length
        if (id.length() != 4) {
            return false;
        }
        
        // Check first character is a letter
        if (!isalpha(id[0])) {
            return false;
        }
        
        // Check remaining 3 characters are digits
        for (int i = 1; i < 4; i++) {
            if (!isdigit(id[i])) {
                return false;
            }
        }
        
        return true;
    }
    
    // Display validation error message
    static void showError(const string& message) {
        cout << "Error: " << message << endl;
    }
    
    // Check if username is valid (between 5 and 20 characters, alphanumeric with underscore)
    static bool isValidUsername(const string& username) {
        // Check length
        if (!isValidStringLength(username, 5, 20)) {
            return false;
        }
        
        // Check characters (only allow letters, numbers, and underscore)
        for (char c : username) {
            if (!isalnum(c) && c != '_') {
                return false;
            }
        }
        
        return true;
    }
    
    // Check if password is valid (between 6 and 20 characters, with at least one number and one letter)
    static bool isValidPassword(const string& password) {
        // Check length
        if (!isValidStringLength(password, 6, 20)) {
            return false;
        }
        
        bool hasLetter = false;
        bool hasDigit = false;
        
        // Check for at least one letter and one number
        for (char c : password) {
            if (isalpha(c)) {
                hasLetter = true;
            } else if (isdigit(c)) {
                hasDigit = true;
            }
        }
        
        return hasLetter && hasDigit;
    }
    
    // Check if Staff/Admin ID is valid (S/A followed by 3 digits)
    static bool isValidStaffAdminID(const string& id, bool isAdmin) {
        // Check length
        if (id.length() != 4) {
            return false;
        }
        
        // Check first character (S for Staff, A for Admin)
        if (isAdmin && id[0] != 'A') {
            return false;
        } else if (!isAdmin && id[0] != 'S') {
            return false;
        }
        
        // Check remaining 3 characters are digits
        for (int i = 1; i < 4; i++) {
            if (!isdigit(id[i])) {
                return false;
            }
        }
        
        return true;
    }
};

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

// Friend function implementation
void displayRestaurantStats(const Restaurant& restaurant) {
    Restaurant::printHeader("Restaurant System Statistics");
    cout << "Total number of items: " << restaurant.itemCount << endl;
    Restaurant::printFooter();
}

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
    
    // Adds a new ingredient with specified quantity to the menu item
    // Dynamically resizes the ingredients array
    void addIngredient(const string& foodItemId, int quantity) {
        // Format: "foodItemId:quantity"
        string ingredient = foodItemId + ":" + to_string(quantity);
        
        // Create new array with increased size
        string* newIngredients = new string[ingredientCount + 1];
        
        // Copy existing ingredients to new array
        for (int i = 0; i < ingredientCount; i++) {
            newIngredients[i] = ingredients[i];
        }
        
        // Add new ingredient at the end
        newIngredients[ingredientCount] = ingredient;
        
        // Delete old array to prevent memory leaks
        if (ingredients != nullptr) {
            delete[] ingredients;
        }
        
        // Update member variables
        ingredients = newIngredients;
        ingredientCount++;
    }
    
    // Removes an ingredient from the menu item by its food item ID
    // Returns true if found and removed, false otherwise
    bool removeIngredient(const string& foodItemId) {
        if (ingredientCount == 0 || ingredients == nullptr) {
            return false;
        }
        
        int removeIndex = -1;
        
        // Find the ingredient to remove by matching food item ID
        for (int i = 0; i < ingredientCount; i++) {
            // Extract food item ID from "foodItemId:quantity"
            size_t colonPos = ingredients[i].find(":");
            if (colonPos != string::npos) {
                string id = ingredients[i].substr(0, colonPos);
                if (id == foodItemId) {
                    removeIndex = i;
                    break;
                }
            }
        }
        
        if (removeIndex == -1) {
            return false; // Ingredient not found
        }
        
        // Create new array with decreased size
        string* newIngredients = nullptr;
        if (ingredientCount > 1) {
            newIngredients = new string[ingredientCount - 1];
            
            // Copy elements before the removal index
            for (int i = 0; i < removeIndex; i++) {
                newIngredients[i] = ingredients[i];
            }
            
            // Copy elements after the removal index (skipping the removed element)
            for (int i = removeIndex + 1; i < ingredientCount; i++) {
                newIngredients[i - 1] = ingredients[i];
            }
        }
        
        // Delete old array to prevent memory leaks
        delete[] ingredients;
        
        // Update member variables
        ingredients = newIngredients;
        ingredientCount--;
        
        return true;
    }
    
    // Retrieves the required quantity of a specific ingredient
    // Returns 0 if the ingredient is not found
    int getIngredientQuantity(const string& foodItemId) const {
        for (int i = 0; i < ingredientCount; i++) {
            // Extract food item ID and quantity from "foodItemId:quantity"
            size_t colonPos = ingredients[i].find(":");
            if (colonPos != string::npos) {
                string id = ingredients[i].substr(0, colonPos);
                if (id == foodItemId) {
                    string quantityStr = ingredients[i].substr(colonPos + 1);
                    return stoi(quantityStr);
                }
            }
        }
        
        return 0; // Ingredient not found
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
    
    // Inserts a menu item at the specified position (0-based index)
    // Returns true if successful, false if position is invalid
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
    // Returns true if found and removed, false otherwise
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
    // Returns nullptr if no matching item exists
    // Caller is responsible for deleting the returned pointer
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
    // Returns nullptr if the list is empty
    // Caller is responsible for deleting the returned array
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

// Restaurant Inventory System with Hash Table
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
        
    // Merge function for Tim Sort
    // Parameters: array, left boundary, middle point, right boundary, and sort criteria
    static void merge(FoodItem arr[], int left, int mid, int right, bool byName) {
        int i, j, k;
        int n1 = mid - left + 1;
        int n2 = right - mid;
        
        // Create temporary arrays
        FoodItem* L = new FoodItem[n1];
        FoodItem* R = new FoodItem[n2];
        
        // Copy data to temp arrays L[] and R[]
        for (i = 0; i < n1; i++)
            L[i] = arr[left + i];
        for (j = 0; j < n2; j++)
            R[j] = arr[mid + 1 + j];
        
        // Merge the temp arrays back into arr[l..r]
        i = 0; // Initial index of first subarray
        j = 0; // Initial index of second subarray
        k = left; // Initial index of merged subarray
        
        while (i < n1 && j < n2) {
            if (byName) {
                // Sort by name (alphabetically)
                // Case-insensitive comparison of names
                string nameL = L[i].name;
                string nameR = R[j].name;
                
                // Convert to lowercase for comparison without using transform
                bool namesSame = true;
                int minLen = (nameL.length() < nameR.length()) ? nameL.length() : nameR.length();
                
                for (int idx = 0; idx < minLen; idx++) {
                    char cL = (nameL[idx] >= 'A' && nameL[idx] <= 'Z') ? nameL[idx] + 32 : nameL[idx];
                    char cR = (nameR[idx] >= 'A' && nameR[idx] <= 'Z') ? nameR[idx] + 32 : nameR[idx];
                    
                    if (cL < cR) {
                        // L comes before R
                        namesSame = false;
                        arr[k] = L[i];
                        i++;
                        break;
                    } else if (cL > cR) {
                        // R comes before L
                        namesSame = false;
                        arr[k] = R[j];
                        j++;
                        break;
                    }
                }
                
                // If all compared characters are the same, shorter string comes first
                if (namesSame) {
                    if (nameL.length() <= nameR.length()) {
                        arr[k] = L[i];
                        i++;
                    } else {
                        arr[k] = R[j];
                        j++;
                    }
                }
            } else {
                // Sort by quantity (ascending)
                if (L[i].quantity <= R[j].quantity) {
                    arr[k] = L[i];
                    i++;
                } else {
                    arr[k] = R[j];
                    j++;
                }
            }
            k++;
        }
        
        // Copy the remaining elements of L[], if there are any
        while (i < n1) {
            arr[k] = L[i];
            i++;
            k++;
        }
        
        // Copy the remaining elements of R[], if there are any
        while (j < n2) {
            arr[k] = R[j];
            j++;
            k++;
        }
        
        // Free the allocated memory for temporary arrays
        delete[] L;
        delete[] R;
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
                // Implement case-insensitive comparison
                while (j >= left) {
                    // Compare names case-insensitively
                    string nameTemp = temp.name;
                    string nameJ = arr[j].name;
                    bool jGreater = false;
                    
                    // Compare characters one by one
                    int minLen = (nameTemp.length() < nameJ.length()) ? nameTemp.length() : nameJ.length();
                    for (int k = 0; k < minLen; k++) {
                        char cTemp = (nameTemp[k] >= 'A' && nameTemp[k] <= 'Z') ? nameTemp[k] + 32 : nameTemp[k];
                        char cJ = (nameJ[k] >= 'A' && nameJ[k] <= 'Z') ? nameJ[k] + 32 : nameJ[k];
                        
                        if (cJ > cTemp) {
                            jGreater = true;
                            break;
                        } else if (cJ < cTemp) {
                            jGreater = false;
                            break;
                        }
                    }
                    
                    // If all compared characters are the same, longer string is "greater"
                    if (!jGreater && nameJ.length() > nameTemp.length() && minLen == nameTemp.length()) {
                        jGreater = true;
                    }
                    
                    if (jGreater) {
                        arr[j + 1] = arr[j];
                        j--;
                    } else {
                        break;
                    }
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
        
        // Variables for aggregating quantities of items with the same ID
        bool itemFound = false;
        FoodItem* result = nullptr;
        
        // Try all possible positions using quadratic probing
        while (attempt < TABLE_SIZE) {
            int probePos = quadraticProbing(position, attempt);
            
            // If the bucket at this position is not empty
            if (!hashTable[probePos].isEmpty()) {
                // Search in the queue at this bucket
                ADTLinkedQueue tempQueue = hashTable[probePos];
                FoodItem* items = tempQueue.toArray();
                int size = tempQueue.getSize();
                
                // Check all items in this bucket
                for (int i = 0; i < size; i++) {
                    if (items[i].id == id) {
                        if (!itemFound) {
                            // First time finding an item with this ID
                            // Create a new FoodItem as the result
                            result = new FoodItem(items[i]);
                            itemFound = true;
                        } else {
                            // If we already found an item with this ID, 
                            // update the quantity of the result
                            result->quantity += items[i].quantity;
                        }
                    }
                }
                
                // Clean up allocated memory
                delete[] items;
            }
            
            // Try next position using quadratic probing
            attempt++;
        }
        
        // Return the aggregated result or nullptr if not found
        return result;
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
    // Presents formatted information about all instances of the item if found
    void searchById(const string& id) {
        bool found = false;
        int totalCount = 0;
        int totalQuantity = 0;
        string itemName = "";
        string category = "";
        double price = 0.0;
        
        // Print table header
        printHeader("Search Result for ID: " + id);
        cout << left << setw(10) << "ID" 
             << setw(30) << "Name" 
             << setw(10) << "Price" 
             << setw(15) << "Category" 
             << setw(10) << "Quantity" 
             << setw(25) << "Receive Date" << endl;
        printFooter();
        
        // Iterate through all buckets to find all instances of this ID
        for (int i = 0; i < TABLE_SIZE; i++) {
            if (hashTable[i].isEmpty()) continue;
            
            // Get all items in this bucket
            ADTLinkedQueue tempQueue = hashTable[i];
            FoodItem* items = tempQueue.toArray();
            int size = tempQueue.getSize();
            
            // Check each item for matching ID
            for (int j = 0; j < size; j++) {
                if (items[j].id == id) {
                    // Found a match, display it
                    found = true;
                    totalCount++;
                    totalQuantity += items[j].quantity;
                    
                    // Store item details for summary
                    if (itemName.empty()) {
                        itemName = items[j].name;
                        category = items[j].category;
                        price = items[j].price;
                    }
                    
                    // Format and display the item details
                    cout << left << setw(10) << items[j].id 
                         << setw(30) << items[j].name 
                         << setw(10) << fixed << setprecision(2) << items[j].price
                         << setw(15) << items[j].category
                         << setw(10) << items[j].quantity 
                         << setw(25) << items[j].receiveDate << endl;
                }
            }
            
            // Free allocated memory
            delete[] items;
        }
        
        // Show summary or not found message
        if (found) {
            cout << "\n----- Summary -----" << endl;
            cout << "Found " << totalCount << " instance(s) of " << itemName << " (ID: " << id << ")" << endl;
            cout << "Total quantity: " << totalQuantity << endl;
        } else {
            cout << "Item with ID " << id << " not found." << endl;
        }
    }

    // Get current number of items in the system
    virtual int getItemCount() const override {
        return Restaurant::getItemCount();
    }

    // Display queue contents for a specific bucket
    // Parameters: bucketIndex - the index of the bucket to display
    void displayQueue(int bucketIndex) {
        // Validate bucket index
        if (bucketIndex < 0 || bucketIndex >= TABLE_SIZE) {
            cout << "Invalid bucket index." << endl;
            return;
        }
        
        // Get a copy of the queue at the specified bucket
        ADTLinkedQueue tempQueue = hashTable[bucketIndex];
        
        // Check if the bucket is empty
        if (tempQueue.isEmpty()) {
            cout << "Bucket " << bucketIndex << " is empty." << endl;
            return;
        }
        
        // Get all items in the bucket as an array
        FoodItem* items = tempQueue.toArray();
        int queueSize = tempQueue.getSize();
        
        // Print table header
        printHeader("Queue Contents for Bucket " + to_string(bucketIndex));
        cout << left << setw(10) << "ID" 
             << setw(30) << "Name" 
             << setw(10) << "Price" 
             << setw(15) << "Category" 
             << setw(10) << "Quantity" 
             << setw(25) << "Receive Date" << endl;
        printFooter();
        
        // Display all items in the bucket
        for (int i = 0; i < queueSize; i++) {
            cout << left << setw(10) << items[i].id 
                 << setw(30) << items[i].name 
                 << setw(10) << fixed << setprecision(2) << items[i].price
                 << setw(15) << items[i].category
                 << setw(10) << items[i].quantity 
                 << setw(25) << items[i].receiveDate << endl;
        }
        
        // Clean up allocated memory
        delete[] items;
    }        

    // Display all non-empty queues in the hash table
    // Shows the contents of each bucket that contains at least one item
    void displayAllQueues() {
        printHeader("All Queues Contents");
        
        // Track if any non-empty queues were found
        bool anyQueues = false;
        
        // Iterate through all buckets in the hash table
        for (int i = 0; i < TABLE_SIZE; i++) {
            // Display only non-empty buckets
            if (!hashTable[i].isEmpty()) {
                // Print bucket header with index and size information
                cout << "\nBucket " << i << " (Size: " << hashTable[i].getSize() << "):" << endl;
                cout << left << setw(10) << "ID" 
                     << setw(30) << "Name" 
                     << setw(10) << "Price" 
                     << setw(15) << "Category" 
                     << setw(10) << "Quantity" 
                     << setw(25) << "Receive Date" << endl;
                cout << string(100, '-') << endl;
                
                // Get items in the current bucket
                FoodItem* items = hashTable[i].toArray();
                int queueSize = hashTable[i].getSize();
                
                // Display each item in the bucket
                for (int j = 0; j < queueSize; j++) {
                    cout << left << setw(10) << items[j].id 
                         << setw(30) << items[j].name 
                         << setw(10) << fixed << setprecision(2) << items[j].price
                         << setw(15) << items[j].category
                         << setw(10) << items[j].quantity 
                         << setw(25) << items[j].receiveDate << endl;
                }
                
                // Clean up allocated memory
                delete[] items;
                anyQueues = true;
            }
        }
        
        // Display message if all buckets are empty
        if (!anyQueues) {
            cout << "No non-empty queues found." << endl;
        }
    }

    // Use (consume) a food item by ID - improved implementation for duplicate IDs
    // Parameters: id - the ID of the food item to consume
    //             amount - the quantity to consume (default: 1)
    // Returns: true if successfully consumed, false if item not found or not enough quantity
    bool useFoodItem(const string& id, int amount = 1) {
        try {
            // Ensure all inventory changes are saved before using items
            this->saveToFile("food_items.txt");
            
            // Search for the item info first to verify it exists and has enough quantity
            string itemName = "";
            int totalQuantity = 0;
            
            // First pass: Find the item and calculate total quantity across all buckets
            for (int i = 0; i < TABLE_SIZE; i++) {
                if (hashTable[i].isEmpty()) continue;
                
                // Create a copy of the queue to inspect without modifying the original
                ADTLinkedQueue tempQueue = hashTable[i];
                int size = tempQueue.getSize();
                FoodItem* items = tempQueue.toArray();
                
                // Check each item in the bucket
                for (int j = 0; j < size; j++) {
                    // If this is the item we're looking for, accumulate quantity info
                    if (items[j].id == id) {
                        totalQuantity += items[j].quantity;
                        if (itemName.empty()) {
                            itemName = items[j].name;
                        }
                    }
                }
                
                // Free allocated memory
                delete[] items;
            }
            
            // Check if item exists
            if (totalQuantity == 0) {
                cout << "Error: Food item with ID " << id << " not found." << endl;
                return false;
            }
            
            // Check if enough quantity is available
            if (totalQuantity < amount) {
                cout << "Error: Not enough quantity available. Only " << totalQuantity << " units of " << itemName << " in stock." << endl;
                return false;
            }
            
            // Second pass: Now we'll consume the items using FIFO order
            int remaining = amount;
            
            // Process each bucket until we've consumed the required amount
            for (int i = 0; i < TABLE_SIZE && remaining > 0; i++) {
                if (hashTable[i].isEmpty()) continue;
                
                // Use temporary queues to process the bucket
                ADTLinkedQueue oldQueue;
                ADTLinkedQueue newQueue;
                int size = hashTable[i].getSize();
                
                // Move all items to temporary queue to process them
                for (int j = 0; j < size; j++) {
                    oldQueue.enqueue(hashTable[i].dequeue());
                }
                
                // Process each item - either consume it or keep it
                size = oldQueue.getSize();
                for (int j = 0; j < size; j++) {
                    FoodItem item = oldQueue.dequeue();
                    
                    // If this is our target item and we still need to consume more
                    if (item.id == id && remaining > 0) {
                        if (item.quantity > remaining) {
                            // Partially consume this item (reduce quantity and keep in queue)
                            item.quantity -= remaining;
                            remaining = 0;
                            newQueue.enqueue(item);
                        } else {
                            // Fully consume this item (don't add back to queue)
                            remaining -= item.quantity;
                            // Skip re-adding it to the queue
                            // Only update item count if it's the last item with this ID
                            bool otherInstancesExist = false;
                            
                            // Check if other instances of this ID exist in any bucket
                            for (int k = 0; k < TABLE_SIZE; k++) {
                                if (k == i) continue; // Skip current bucket
                                
                                ADTLinkedQueue checkQueue = hashTable[k];
                                FoodItem* checkItems = checkQueue.toArray();
                                int checkSize = checkQueue.getSize();
                                
                                for (int l = 0; l < checkSize; l++) {
                                    if (checkItems[l].id == id) {
                                        otherInstancesExist = true;
                                        break;
                                    }
                                }
                                
                                delete[] checkItems;
                                if (otherInstancesExist) break;
                            }
                            
                            // Also check remaining items in the current bucket's new queue
                            if (!otherInstancesExist) {
                                FoodItem* newQueueItems = newQueue.toArray();
                                int newQueueSize = newQueue.getSize();
                                
                                for (int l = 0; l < newQueueSize; l++) {
                                    if (newQueueItems[l].id == id) {
                                        otherInstancesExist = true;
                                        break;
                                    }
                                }
                                
                                delete[] newQueueItems;
                            }
                            
                            // Only decrement item count if this was the last instance
                            if (!otherInstancesExist) {
                                this->itemCount--;
                            }
                        }
                    } else {
                        // Not our target item or nothing left to consume, keep it as is
                        newQueue.enqueue(item);
                    }
                }
                
                // Replace the bucket with the new queue containing updated items
                hashTable[i] = newQueue;
            }
            
            // Display success message with consumption details
            cout << "Successfully used " << amount << " units of " << itemName << ". Remaining: " << (totalQuantity - amount) << endl;
            return true;

        } catch (const std::bad_alloc& e) {
            // Handle memory allocation errors
            cout << "Memory allocation error in useFoodItem: " << e.what() << endl;
            return false;
        } catch (const std::exception& e) {
            // Handle standard exceptions
            cout << "Error in useFoodItem: " << e.what() << endl;
            return false;
        } catch (...) {
            // Handle any other unexpected errors
            cout << "Unknown error in useFoodItem" << endl;
            return false;
        }
    }
    
    // Log usage of food items to a history file
    // Parameters: id - the ID of the food item that was used
    //             amount - the quantity that was used
    //             purpose - description of why the item was used
    void logItemUsage(const string& id, int amount, const string& purpose) {
        try {
            // Retrieve the item details for complete logging
            FoodItem* item = findFoodItem(id);
            if (item == nullptr) {
                cout << "Warning: Could not find item details for logging." << endl;
                return;
            }
            
            // Open the history file in append mode to maintain all previous logs
            ofstream historyFile("usage_history.txt", ios::app);
            if (historyFile.is_open()) {
                // Get current time to timestamp the usage
                time_t now = time(0);
                string dateStr = ctime(&now);
                // Remove the newline character from ctime output
                if (!dateStr.empty() && dateStr[dateStr.length()-1] == '\n') {
                    dateStr.erase(dateStr.length()-1);
                }
                
                // Write entry in TXT format: timestamp,id,name,amount,purpose
                historyFile << dateStr << ","
                            << id << ","
                            << item->name << ","
                            << amount << ","
                            << purpose << endl;
                
                historyFile.close();
            } else {
                cout << "Warning: Could not open usage history file for writing." << endl;
            }
            
            // Clean up the dynamically allocated item
            delete item;
        } catch (const std::exception& e) {
            // Handle standard exceptions
            cout << "Error in logItemUsage: " << e.what() << endl;
        } catch (...) {
            // Handle any unexpected errors
            cout << "Unknown error in logItemUsage" << endl;
        }
    }
    
    // Display usage history from log file
    // Shows all recorded item usage in a formatted table
    void displayUsageHistory() {
        // Try to open the history file
        ifstream historyFile("usage_history.txt");
        if (!historyFile.is_open()) {
            cout << "No usage history found." << endl;
            return;
        }
        
        // Display the table header
        printHeader("Food Usage History");
        cout << left << setw(25) << "Date/Time" 
             << setw(10) << "ID" 
             << setw(30) << "Name" 
             << setw(10) << "Amount" 
             << setw(30) << "Purpose" << endl;
        printFooter();
        
        // Read and parse each line in the history file
        string line;
        while (getline(historyFile, line)) {
            string dateTime, id, name, amount, purpose;
            
            // Parse TXT line format: timestamp,id,name,amount,purpose
            size_t pos = 0;
            
            // Extract Date/Time
            pos = line.find(",");
            if (pos == string::npos) continue; // Skip malformed lines
            dateTime = line.substr(0, pos);
            line.erase(0, pos + 1);
            
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
            
            // Extract Amount
            pos = line.find(",");
            if (pos == string::npos) continue; // Skip malformed lines
            amount = line.substr(0, pos);
            line.erase(0, pos + 1);
            
            // The rest is purpose
            purpose = line;
            
            // Display the entry in a formatted row
            cout << left << setw(25) << dateTime 
                 << setw(10) << id 
                 << setw(30) << name 
                 << setw(10) << amount 
                 << setw(30) << purpose << endl;
        }
        
        // Close the file when done
        historyFile.close();
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


// Interpolation Search for MenuItem objects - O(log log n) average case for uniformly distributed data
// A searching algorithm that works on uniformly distributed sorted data
// Parameters: sorted array, array size, and ID to search for
int interpolationSearchMenuItems(MenuItem arr[], int n, const string& id) {
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

class RestaurantMenuSystem {
    private:
        ADTLinkedList menuList;  // Linked list to store menu items
        int itemCount;           // Total number of menu items

    public:
        // Constructor - initializes an empty menu system
        RestaurantMenuSystem() : itemCount(0) {}
        
        // Destructor - linked list handles its own memory cleanup
        ~RestaurantMenuSystem() {}

        // Returns the current number of menu items in the system
        virtual int getItemCount() const override {
            return Restaurant::getItemCount();
        }

        // Adds a new menu item to the system
        // Parameters: item - the menu item to add
        // Returns: true if successfully added, false if failed (e.g., duplicate ID)
        bool addMenuItem(const MenuItem& item) {
            // Validate menu item ID
            if (item.id.empty()) {
                cout << "Error: Menu item ID cannot be empty." << endl;
                return false;
            }
            
            // Check if item with this ID already exists (prevent duplicates)
            MenuItem* existingItem = findMenuItem(item.id);
            if (existingItem != nullptr) {
                cout << "Error: Menu item with ID " << item.id << " already exists." << endl;
                delete existingItem;
                return false;
            }
            
            // Add the item to the end of the linked list
            menuList.append(item);
            this->itemCount++;
            
            return true;
        }

        // Finds a menu item by its ID
        // Parameters: id - the ID to search for
        // Returns: pointer to a copy of the found item or nullptr if not found
        // Note: Caller is responsible for deleting the returned pointer
        MenuItem* findMenuItem(const string& id) const {
            return menuList.findById(id);
        }


        
};


/**
 * Base User class for authentication and user management
 * Provides common functionality for all user types
 * @time_complexity Varies by method
 * @space_complexity Varies by method
 */
class User {
protected:
    string username;
    string password;
    bool isLoggedIn;
    
    // Protected constructor to prevent direct instantiation
    User(const string& _username = "", const string& _password = "") 
        : username(_username), password(_password), isLoggedIn(false) {}
    
public:
    // Virtual destructor for proper cleanup in derived classes
    virtual ~User() {}
    
    // Getter for login status
    bool getLoginStatus() const {
        return isLoggedIn;
    }
    
    // Getter for username 
    string getUsername() const {
        return username;
    }
    
    // Virtual login function to be overridden by derived classes
    virtual bool login(const string& _username, const string& _password) {
        // Base implementation (will be overridden)
        if (username == _username && password == _password) {
            isLoggedIn = true;
            return true;
        }
        return false;
    }
    
    // Virtual logout function to be overridden by derived classes
    virtual void logout() {
        isLoggedIn = false;
        cout << "Logged out successfully." << endl;
    }
    
    // Virtual register account function to be overridden by derived classes
    virtual bool registerAccount(const string& _username, const string& _password) {
        // Base implementation (will be overridden)
        username = _username;
        password = _password;
        return true;
    }
    
    // Overloaded version of registerAccount that takes a role parameter
    virtual bool registerAccount(const string& _username, const string& _password, const string& role) {
        // Call the base version
        return registerAccount(_username, _password);
    }
    
    // Friend function declaration
    friend void displayUserInfo(const User& user);
};

/**
 * Staff class for regular staff users
 * Manages staff-specific functionality and authentication
 */
class Staff : public User {
private:
    string staffId;
    string position;
    
public:
    // Constructor
    Staff(const string& _username = "", const string& _password = "", 
          const string& _staffId = "", const string& _position = "")
        : User(_username, _password), staffId(_staffId), position(_position) {}
    
    // Getter for staff ID
    string getStaffId() const {
        return staffId;
    }
    
    // Getter for position
    string getPosition() const {
        return position;
    }
    
    // Setters
    void setStaffId(const string& _staffId) {
        staffId = _staffId;
    }
    
    void setPosition(const string& _position) {
        position = _position;
    }
    
    // Override login to check against staff.txt file
    virtual bool login(const string& _username, const string& _password) override {
        ifstream file("staff.txt");
        if (!file.is_open()) {
            cout << "Error: Could not open staff credentials file." << endl;
            return false;
        }
        
        string line;
        bool found = false;
        
        // Format: username,password,staffId,position
        while (getline(file, line)) {
            size_t pos = line.find(",");
            if (pos == string::npos) continue;
            
            string fileUsername = line.substr(0, pos);
            line.erase(0, pos + 1);
            
            pos = line.find(",");
            if (pos == string::npos) continue;
            
            string filePassword = line.substr(0, pos);
            line.erase(0, pos + 1);
            
            pos = line.find(",");
            if (pos == string::npos) continue;
            
            string fileStaffId = line.substr(0, pos);
            string filePosition = line.substr(pos + 1);
            
            if (fileUsername == _username && filePassword == _password) {
                username = fileUsername;
                password = filePassword;
                staffId = fileStaffId;
                position = filePosition;
                isLoggedIn = true;
                found = true;
                break;
            }
        }
        
        file.close();
        
        if (!found) {
            cout << "Invalid username or password for staff login." << endl;
        } else {
            cout << "Staff login successful. Welcome " << username << "!" << endl;
        }
        
        return found;
    }
    
    // Override logout with staff-specific message
    virtual void logout() override {
        isLoggedIn = false;
        cout << "Staff member " << username << " logged out successfully." << endl;
    }
    
    // Override register to save to staff.txt file
    virtual bool registerAccount(const string& _username, const string& _password) override {
        return registerAccount(_username, _password, "", "Default");
    }
    
    // Override overloaded registerAccount with staff-specific parameters
    virtual bool registerAccount(const string& _username, const string& _password, 
                                const string& _staffId, const string& _position) {
        // Check if username already exists
        ifstream checkFile("staff.txt");
        if (checkFile.is_open()) {
            string line;
            while (getline(checkFile, line)) {
                size_t pos = line.find(",");
                if (pos != string::npos) {
                    string existingUsername = line.substr(0, pos);
                    if (existingUsername == _username) {
                        cout << "Error: Username already exists." << endl;
                        checkFile.close();
                        return false;
                    }
                }
            }
            checkFile.close();
        }
        
        // Save to staff.txt
        ofstream file("staff.txt", ios::app);
        if (!file.is_open()) {
            cout << "Error: Could not open staff credentials file for writing." << endl;
            return false;
        }
        
        // Update member variables
        username = _username;
        password = _password;
        staffId = _staffId;
        position = _position;
        
        // Write to file
        file << username << "," << password << "," << staffId << "," << position << endl;
        file.close();
        
        cout << "Staff account registered successfully." << endl;
        return true;
    }
    
    // Overload the original virtual registerAccount with role parameter
    virtual bool registerAccount(const string& _username, const string& _password, const string& role) override {
        // For staff, we ignore the role parameter and use default position
        return registerAccount(_username, _password, "", "Default");
    }
};

// Friend function implementation
void displayUserInfo(const User& user) {
    cout << "Username: " << user.username << endl;
    cout << "Login Status: " << (user.isLoggedIn ? "Logged In" : "Logged Out") << endl;
}


/**
 * Authentication Manager class to handle user login and registration
 * Manages both Staff and Admin users in a unified interface
 */
class AuthManager {
private:
    Staff* currentStaff;
    Admin* currentAdmin;
    bool isAdminMode;
    
public:
    // Constructor
    AuthManager() : currentStaff(nullptr), currentAdmin(nullptr), isAdminMode(false) {}
    
    // Destructor - clean up dynamically allocated objects
    ~AuthManager() {
        if (currentStaff != nullptr) {
            delete currentStaff;
        }
        if (currentAdmin != nullptr) {
            delete currentAdmin;
        }
    }
    
    // Check if any user is logged in
    bool isLoggedIn() const {
        return (currentStaff != nullptr && currentStaff->getLoginStatus()) || 
               (currentAdmin != nullptr && currentAdmin->getLoginStatus());
    }
    
    // Check if current user is an admin
    bool isAdminLoggedIn() const {
        return isAdminMode && currentAdmin != nullptr && currentAdmin->getLoginStatus();
    }
    
    // Get current user username
    string getCurrentUsername() const {
        if (isAdminMode && currentAdmin != nullptr) {
            return currentAdmin->getUsername();
        } else if (!isAdminMode && currentStaff != nullptr) {
            return currentStaff->getUsername();
        }
        return "";
    }
    
    // Handle login process
    bool login(const string& username, const string& password, bool adminLogin) {
        if (adminLogin) {
            // Admin login
            if (currentAdmin == nullptr) {
                currentAdmin = new Admin();
            }
            if (currentAdmin->login(username, password)) {
                isAdminMode = true;
                return true;
            }
        } else {
            // Staff login
            if (currentStaff == nullptr) {
                currentStaff = new Staff();
            }
            if (currentStaff->login(username, password)) {
                isAdminMode = false;
                return true;
            }
        }
        return false;
    }
    
    // Handle logout process
    void logout() {
        if (isAdminMode && currentAdmin != nullptr) {
            currentAdmin->logout();
        } else if (!isAdminMode && currentStaff != nullptr) {
            currentStaff->logout();
        } else {
            cout << "No user is currently logged in." << endl;
        }
    }
    
    // Handle staff registration
    bool registerStaff(const string& username, const string& password, 
                      const string& staffId, const string& position) {
        Staff staff;
        return staff.registerAccount(username, password, staffId, position);
    }
    
    // Handle admin registration
    bool registerAdmin(const string& username, const string& password, 
                       const string& adminId, const string& accessLevel) {
        Admin admin;
        return admin.registerAccount(username, password, adminId, accessLevel);
    }
    
    // Overloaded function to register either staff or admin based on role
    bool registerUser(const string& username, const string& password, const string& role) {
        if (role == "admin") {
            Admin admin;
            return admin.registerAccount(username, password, role);
        } else {
            Staff staff;
            return staff.registerAccount(username, password, role);
        }
    }
};

int main()
{   
    AuthManager authManager;
    bool isAuthenticated = false;
    RestaurantInventorySystem inventory;
    RestaurantMenuSystem menuSystem; 
    int choice;
    // First check for authentication
    do {
        // Clear screen for better UI experience
        RestaurantInventorySystem::clearScreen();
        
        // Check if user is already authenticated
        if (!isAuthenticated) {
            // Display authentication menu
            cout << "\n==== Restaurant Management System - Authentication ====" << endl;
            cout << "1. Login as Staff" << endl;
            cout << "2. Login as Admin" << endl;
            cout << "3. Register Staff Account" << endl;
            cout << "4. Register Admin Account" << endl;
            cout << "0. Exit" << endl;
            cout << "Enter your choice: ";
            cin >> choice;
        }
    } while (choice != 0);

    return 0;
}

