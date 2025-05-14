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


