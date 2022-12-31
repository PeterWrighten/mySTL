#include <iostream>
#include "shared_ptr.h"
#include "weak_ptr.h"
using namespace std;
using namespace SmartPtr;

int main(int argc, char *argv[]) {

    int *pi = new int(2);

    SharedPtr<int> ptr1(pi);
    SharedPtr<int> ptr2(ptr1);
    SharedPtr<int> ptr3(new int(3));
    cout << "ptr1 count: " << ptr1.use_count() << endl;  
    {   
       WeakPtr<int> wp = ptr1;
       cout << "wp count: " << wp.use_count() << ", wp: " << *wp << endl;
    }

    cout << "ptr1 count: " << ptr1.use_count() << ", ptr2: " << ptr2.use_count() << ", ptr3: " << ptr3.use_count() << endl;

    ptr3 = ptr2;
    
    cout << "ptr3 count: " << ptr3.use_count() << endl;

    cout << "*ptr1: " << *ptr1 << endl;
    *ptr1 = 20;

    ptr3.reset();
    ptr3 = ptr1;
    cout << "*ptr3: " << *ptr3 << endl;

    cout << "*ptr2: " << *ptr2 << endl;

    return 0;

}
