#include <iostream>
#include <exception>
using namespace std;

class null_ptr_exception : public std::exception{
public:
    null_ptr_exception() : std::exception() {}

    virtual const char* what() const noexcept{
        return "nullptr exception";
    }
};

template <typename T>
class smart_ptr {

private:
    T* ptr_;               // pointer to the referred object
    int* ref_;             // pointer to a reference count
    
    //auxilary function used to check if there is room for allocation
    bool check(int* &reference) {
        int* temp_num;
        try {
            temp_num = new int(1);
        }
        catch(bad_alloc) {
            reference = nullptr;
            return false;
        }
        delete temp_num;
        return true;
    }
    
    //auxilary function used to delete a smart pointer
    void remove() {
        if (ref_ != nullptr) {
            if (*ref_ == 1) {
                delete ptr_;
                delete ref_;
            }
            else {
                *ref_ = *ref_ - 1;
            }
            ptr_ = nullptr;
            ref_ = nullptr;
        }
    }
    
public:
    
    //tries to create a new pointer that is set to nullptr, if cant -> throw exception
    smart_ptr() noexcept {
        this->ptr_ = nullptr;
        this->ref_ = nullptr;
    }
      // Create a smart_ptr that is initialized to nullptr. The reference count
      // should be initialized to nullptr.
    
    //tries to create a new pointer with the value given and tries to set count to 1
    //if cant -> throw exception
    explicit smart_ptr(T* &raw_ptr){
        if (check(ref_) == false) {
            throw bad_alloc();
        }
        this->ptr_ = raw_ptr;
        this->ref_ = new int(1);
    }
    //exception leaves func f
      // Create a smart_ptr that is initialized to raw_ptr. The reference count
      // should be one. No change is made to raw_ptr.
    
    //copy constructor for rvalue
    explicit smart_ptr(T* &&raw_ptr){
        if (check(ref_) == false) {
            delete raw_ptr;
            throw bad_alloc();
        }
        this->ptr_ = raw_ptr;
        ref_ = new int(1);
    }
    //want sp to delete but exception to leave func
      // Create a smart_ptr that is initialized to raw_ptr. The reference count
      // should be one. If the constructor fails raw_ptr is deleted.

    //copy constructor
    smart_ptr(const smart_ptr& rhs) noexcept{
        this->ptr_ = rhs.ptr_;
        this->ref_ = rhs.ref_;
        if (ref_ != nullptr) {
            *ref_ = *ref_ + 1;
        }
    }
    
      // Copy construct a pointer from rhs. The reference count should be
      // incremented by one.

    //move constructor
    smart_ptr(smart_ptr&& rhs) noexcept{
        this->ptr_ = rhs.ptr_;
        this->ref_ = rhs.ref_;
        rhs.ptr_ = nullptr;
        rhs.ref_ = nullptr;
    }
      // Move construct a pointer from rhs.

    smart_ptr& operator=(const smart_ptr& rhs) noexcept{
        if (this != &rhs) {
            remove();
            ptr_ = rhs.ptr_;
            ref_ = rhs.ref_;
            if (ref_ != nullptr) {
                *ref_ = *ref_ + 1;
            }
        }
        return *this;
    }
      // This assignment should make a shallow copy of the right-hand side's
      // pointer data. The reference count should be incremented as appropriate.

    smart_ptr& operator=(smart_ptr&& rhs) noexcept{
        this->ptr_ = rhs.ptr_;
        this->ref_ = rhs.ref_;
        rhs.ptr_ = nullptr;
        rhs.ref_ = nullptr;
        return *this;
    }
      // This move assignment should steal the right-hand side's pointer data.
      
    bool clone() {
        if (ptr_ == nullptr || *ref_ == 1){
            return false;
        }
        
        T* temp = new T(*ptr_);
        //decrease the objects current ref count
        remove();
        //check for bad_alloc
        if (check(ref_) == false) {
            throw bad_alloc();
        }
        
        ptr_ = temp;
        ref_ = new int(1);
        return true;
    } //exception leaves function
      // If the smart_ptr is either nullptr or has a reference count of one, this
      // function will do nothing and return false. Otherwise, the referred to
      // object's reference count will be decreased and a new deep copy of the
      // object will be created. This new copy will be the object that this
      // smart_ptr points and its reference count will be one.
      
    int ref_count() const noexcept{
        if (ref_ == nullptr) {
            return 0;
        }
        return *ref_;
    }
      // Returns the reference count of the pointed to data.

    T& operator*(){
        if (ptr_ != nullptr){
            return *ptr_;
        }
        throw null_ptr_exception();
    }
      // The dereference operator shall return a reference to the referred object.
      // Throws null_ptr_exception on invalid access.

    T* operator->(){
        if (ptr_) {
            return ptr_;
        }
        throw null_ptr_exception();
    }
      // The arrow operator shall return the pointer ptr_. Throws null_ptr_exception
      // on invalid access.

    ~smart_ptr() noexcept{
        remove();
    }
    // deallocate all dynamic memory
};
