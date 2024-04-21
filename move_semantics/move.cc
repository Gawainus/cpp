#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Person
{
public:
    Person() {
        cout << "Default ctor called" << endl;
    }

    Person(const string& name)
        : name_(name) {
        cout << "Simple ctor called" << endl;
    }

    Person(string&& name)
        : name_(std::move(name)) {
        cout << "Simple MOVE ctor called" << endl;
    }
    
    Person(const Person& rhs) {
        cout << "COPY ctor called" << endl;
        
    }

    Person(Person&& rhs) {
        cout << "MOVE ctor called" << endl;
    }

    Person& operator=(const Person& rhs) {
        cout << "COPY assignment called" << endl;
        name_ = rhs.name_;
        return *this;
    }

    Person& operator=(Person&& rhs) {
        cout << "MOVE assignment called" << endl;
        name_ = std::move(rhs.name_);
        return *this;
    }

private:
    string name_;
};

class Staff
{
public:
    Staff() = default;

    template<typename T>
    void push_log(T&& person) {
        auto ts = chrono::high_resolution_clock::now();
        c_.push_back(person);

        auto ts2 = chrono::high_resolution_clock::now();
        cout << (ts2 - ts).count() << ": " << c_.size() << endl;
    }

    template<typename T>
    void emplace_log(T&& person) {
        auto ts = chrono::high_resolution_clock::now();
        c_.emplace_back(std::move(person));

        auto ts2 = chrono::high_resolution_clock::now();
        cout << (ts2 - ts).count() << ": " << c_.size() << endl;
    }

private:

    vector<Person> c_;
};

int main() {
    Staff s;
    // cout << "\nMoves:" << endl;
    // s.add_log(Person("Alex"));

    cout << "\nCopy and move:" << endl;
    string alex_name("Alex");

    Person serg{alex_name};
    s.emplace_log(std::move(serg));

    Person alex{alex_name};
    s.push_log(alex);

    Person mikha{alex_name};
    s.push_log(mikha);
    
    Person dmitri{alex_name};
    s.push_log(dmitri);
    

}