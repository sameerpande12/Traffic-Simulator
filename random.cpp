#include <any>
#include <cstring>
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <random>
#include <queue>
#include <fstream>
#include <sstream>
#include <chrono>
#include "random.h"
#include <thread>
using namespace std;
std::random_device random_gen;
std::mt19937 gen(random_gen());
std::uniform_real_distribution<>dis(0.0,1.0);

float getRandom(){
  //cout<<&dis<<endl; to check the address is same or not
  return (float) dis(gen);
};//returns a float between 0 to 1 in uniform distribution
