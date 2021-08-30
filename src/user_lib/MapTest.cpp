#include "Arduino.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <map>
 
template<typename Map>
void print_map(Map& m)
{
   Serial.println('{');
    for(auto& p: m)
        Serial.println(String((p.first).c_str()) + 
                String(" : ") + String(p.second));
            // std::cout << p.first << ':' << p.second << ' ';
    Serial.println("}");
}
 
struct Point { double x, y; };
struct PointCmp {
    bool operator()(const Point& lhs, const Point& rhs) const { 
        return lhs.x < rhs.x; // NB. intentionally ignores y
    }
};

int tester_map()
{
    std::map<std::string, int> mapOfWords;
    // Inserting data in std::map
    mapOfWords.insert(std::make_pair("earth", 1));
    mapOfWords.insert(std::make_pair("moon", 2));
    // add sun
    mapOfWords["sun"] = 3;
    // update earth Will replace the value of already added key i.e. earth
    mapOfWords["earth"] = 4;
    // remove moon
    mapOfWords.erase("moon");
    // Iterate through all elements in std::map
    std::map<std::string, int>::iterator it = mapOfWords.begin();
    while(it != mapOfWords.end())
    {
        Serial.println(String((it->first).c_str() + // earth : 4 ; sun : 3
            String(" : ") + String(it->second)));
        it++;
    }
    // Check if insertion is successful or not
    if(mapOfWords.insert(std::make_pair("earth", 1)).second == false)
    {
        Serial.println("Element with key 'earth' not inserted because already existed"); // print not inserted
    }
    // Searching element in std::map by key.
    if(mapOfWords.find("sun") != mapOfWords.end())
        // std::cout<<"word 'sun' found"<<std::endl;
        Serial.println("word 'sun' found");
        auto sun = mapOfWords.find("sun"); // iterator
        Serial.println(String((sun->first).c_str()) + // sun : 3
            String(" : ") + String(sun->second));
    if(mapOfWords.find("mars") == mapOfWords.end())
        Serial.println("word 'mars' not found"); // not find
        //std::cout<<"word 'mars' not found"<<std::endl;
    const std::map<std::string, int> init {
        {"this", 100},
        {"can", 100},
        {"be", 100},
        {"const", 100},
    };
    print_map(init); // print arry

  /* // (1) Default constructor
  std::map<std::string, int> map1;
  map1["something"] = 69;
  map1["anything"] = 199;
  map1["that thing"] = 50;
  std::cout << "map1 = "; print_map(map1);
 
  // (2) Range constructor
  std::map<std::string, int> iter(map1.find("anything"), map1.end());
  std::cout << "\niter = "; print_map(iter);
  std::cout << "map1 = "; print_map(map1);
 
  // (3) Copy constructor
  std::map<std::string, int> copied(map1);
  std::cout << "\ncopied = "; print_map(copied);
  std::cout << "map1 = "; print_map(map1);
 
  // (4) Move constructor
  std::map<std::string, int> moved(std::move(map1));
  std::cout << "\nmoved = "; print_map(moved);
  std::cout << "map1 = "; print_map(map1);
 
  // (5) Initializer list constructor
  const std::map<std::string, int> init {
    {"this", 100},
    {"can", 100},
    {"be", 100},
    {"const", 100},
  };
  std::cout << "\ninit = "; print_map(init);
 
 
  // Custom Key class option 1:
  // Use a comparison struct
  std::map<Point, double, PointCmp> mag = {
      { {5, -12}, 13 },
      { {3, 4},   5 },
      { {-8, -15}, 17 }
  };
 
  for(auto p : mag)
      std::cout << "The magnitude of (" << p.first.x
                << ", " << p.first.y << ") is "
                << p.second << '\n';
 
  // Custom Key class option 2:    
  // Use a comparison lambda
  // This lambda sorts points according to their magnitudes, where note that
  //  these magnitudes are taken from the local variable mag
  auto cmpLambda = [&mag](const Point &lhs, const Point &rhs) { return mag[lhs] < mag[rhs]; };
  //You could also use a lambda that is not dependent on local variables, like this:
  //auto cmpLambda = [](const Point &lhs, const Point &rhs) { return lhs.y < rhs.y; };
  std::map<Point, double, decltype(cmpLambda)> magy(cmpLambda);
 
  //Various ways of inserting elements:
  magy.insert(std::pair<Point, double>({5, -12}, 13));
  magy.insert({ {3, 4}, 5});
  magy.insert({Point{-8.0, -15.0}, 17});
 
  std::cout << '\n';
  for(auto p : magy)
      std::cout << "The magnitude of (" << p.first.x
                << ", " << p.first.y << ") is "
                << p.second << '\n'; */
}