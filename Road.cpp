
#include "Vehicle.cpp"
class Road{
public:
  int id;//
  int length=1;//
  int width=1;// make them of type final
  char** road_matrix;
  int signal_pos;
  char* signal_color = new char[0];
  map<int,Vehicle*> vehicles;
  map<char,Vehicle*> symbol_maps;

};