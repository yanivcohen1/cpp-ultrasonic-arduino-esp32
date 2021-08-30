#include "Arduino.h"
// #include <iostream> // std::cout
#include <string>         // std::string

#include <algorithm> // std::shuffle
#include <list>
#include <numeric>
#include <random> // std::random
#include <vector>

void tester_list()
{
    // lists
    std::list<int> l(5);
    std::iota(l.begin(), l.end(), 1);// l= 1 2 3 4 5 

    std::list<int> l2(2);
    std::iota(l2.begin(), l2.end(), -1);// l2 = -1 0

    // std::copy(l.begin(), l.end(), l2.begin()); // append l to l2 now l2 = -1 0 1 2 3 4 5
    std::copy(l.begin(), l.end(), std::back_inserter(l2));

    std::vector<std::list<int>::iterator> v(l2.size()); // vector of lists of size l2
    std::iota(v.begin(), v.end(), l2.begin());// v = -1 0 1 2 3 4 5

    std::shuffle(v.begin(), v.end(), std::mt19937{ std::random_device{}() });// suffle v

    // std::cout << "Contents of the list: ";
    Serial.println("Contents of the list: ");
    for (auto n : l2) Serial.print(n + String(" "));//std::cout << n << ' '; // print l2 = -1 0 1 2 3 4 5
    Serial.println();

    l2.push_back(-4);
    l2.push_back(7);
    l2.sort();
    // l2.reverse();
    Serial.println("Contents of the sort list: ");
    for (auto n : l2) Serial.print(n + String(" "));//std::cout << n << ' '; // print l2 = -4 -1 0 1 2 3 4 5 7
    Serial.println();
    //std::cout << '\n';

    //std::cout << "Contents of the list, shuffled: ";
    Serial.println("Contents of the list, shuffled: ");
    for (auto i : v) {
        Serial.print(*i);
        Serial.print(" ");//std::cout << *i << ' '; // print v content(*i), v = 5 -1 1 3 2 4 0
    }
    Serial.println();
    //std::cout << '\n';

}

void print(std::list<std::string> const &list)
{
    for (auto const &i: list) {
        Serial.println(i.c_str());
        //std::cout << i << std::endl;
    }
}
 
int tester_str_list()
{
    std::list<std::string>::iterator it;
    std::list<std::string> list = { "blue", "red", "green" };
    list.insert(list.begin(), "yellow");// insert at begin
    list.push_back("black"); // insert at end
    list.remove("black"); // remove black from list
    it = list.begin(); // iterator to begin of list
    Serial.println(String("size: ") + String(list.size()));
    Serial.println(String("first: ") + String((*it).c_str()));
    Serial.println(String("secend: ") + String((*++it).c_str()));
    print(list); // print all in loop
 
    return 0;
}
//useing it
//#include "all_testers.h"
//
//int main()
//{
//    tester_list();
//    return 0;
//}

// printing:
// Contents of the list : -1 0 1 2 3 4 5
// Contents of the list, shuffled : 2 3 - 1 0 5 4 1