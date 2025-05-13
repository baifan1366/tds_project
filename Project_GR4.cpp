#include <iostream>

using namespace std;

int main()
{
    return 0;
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
