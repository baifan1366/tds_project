# 🍽️ C++ Restaurant Management System

This project is a comprehensive **Restaurant Management System** implemented in **C++**, featuring both inventory and menu management capabilities. The system utilizes advanced data structures and algorithms to provide efficient operation for restaurant management tasks.

## ✨ Core Features

### 1. Inventory Management
- Track food ingredients with unique IDs, names, prices, categories, and quantities
- Automatic timestamp tracking for each inventory batch
- Multiple search options (by ID, name, price range)
- Sorted views by name or quantity
- FIFO (First-In-First-Out) consumption of ingredients for freshness management

### 2. Menu Management
- Create and manage menu items with ingredients linked to inventory
- Multiple sorting options (by name, price, category)
- Efficient search algorithms for quick item retrieval
- Ingredient availability checking before preparation
- Detailed menu item information with real-time ingredient status

### 3. Preparation System
- Prepare menu items by automatically consuming required ingredients
- Individual food item usage with purpose tracking
- Usage history logging for audit and analysis
- Ingredient freshness management based on receive date

### 4. Data Persistence
- All data is stored in text files for easy backup and recovery
- Automatic data loading/saving to maintain system state
- Sample data generation for first-time setup

## 🧠 Advanced Algorithms

The system implements several optimized algorithms:

| Algorithm | Implementation | Benefits |
|-----------|----------------|----------|
| **Tim Sort** | Hybrid algorithm combining merge sort and insertion sort | O(n log n) average time complexity, better than bubble sort's O(n²) |
| **Interpolation Search** | Improved search for uniformly distributed data | O(log log n) average time complexity, better than linear search's O(n) |
| **Universal Hashing** | Hash function with collision resolution | Reduces clustering and provides better hash distribution |
| **ADT Implementations** | Linked List and Queue | Efficient data management with dynamic memory allocation |

## 📊 Data Structures Used

| Feature | Data Structure | Purpose |
|---------|----------------|---------|
| Inventory Storage | Hash Table with Linked Queues | Efficient item lookup with FIFO batch management |
| Menu Items | Linked List | Flexible menu organization and updates |
| Sorting | Tim Sort Algorithm | Efficient sorting for different display options |
| Search | Interpolation Search | Fast item retrieval by ID |

## 🔄 System Workflow

1. **Data Initialization**: System loads existing data or creates sample data
2. **Main Menu**: Navigate between Inventory and Menu Management
3. **Inventory Operations**: Add, search, update, and use food items
4. **Menu Operations**: Create recipes, check availability, prepare dishes
5. **Data Persistence**: Changes are saved to files for future sessions

## 💻 Usage

The system provides an interactive console interface with clear menu options:

1. Start the application by running the executable
2. Navigate through the main menu using numeric options
3. Follow on-screen prompts to perform specific operations
4. All data changes are automatically saved to text files

## 📋 Implementation Details

- Written in C++ with standard libraries
- Memory-safe implementation with proper resource management
- Extensive error handling to prevent crashes
- Well-documented code with descriptive comments

## Project todo

- add registration and login&logout module for staff and admin (staff.txt,admin.txt)
- change two system to staff and admin module
- admin module add manage admin and staff
- add more search and sort (by date and by category)
- add overloaded/overridden functions (need 4)
- change 2 base classes (res,user)
- change 3 derived classes (res_inventory,res_menu,staff,admin)
- add 4 friend functions
- The system must include proper error handling using try and catch blocks in C++ to manage exceptions effectively. Error handling mechanisms should be implemented in both the Customer and Staff/Admin modules.