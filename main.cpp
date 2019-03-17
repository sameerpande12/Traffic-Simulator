/*
Axes:- Right -> Positive,
       Down -> Positive

(i,j) for roads-> indexing pattern
----------------------
0,0     0,1       0,2
1,0     1,1       1,2
-----------------------
references- https://www.geeksforgeeks.org/stdany-class-in-c/
https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
*/
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
using namespace std;

std::random_device random_gen;
std::mt19937 gen(random_gen());
std::uniform_real_distribution<>dis(0.0,1.0);

int present_time;
int vehicle_id;
int default_max_xspeed=3;
int default_max_acceleration = 1;
int default_lanechange_vertical_speed = 1;
int default_lanechange_horizontal_speed = 1;
int signal_pos = 8;
int default_xpos = 0;
int default_ypos = 0;
int default_vertical_velocity  = 0;
int default_horizontal_velocity = 0;
int road_len = 20;
int road_wid = 5;
float default_lane_change_freq = 0.4;
float getRandom(){
  return (float) dis(gen);
};//returns a float between 0 to 1 in uniform distribution
class Vehicle;
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
class template_vehicle
{
public:
  string type; string color ; int length; int width;  char symbol; int lane_no; int column_no; int max_speed; int max_acceleration; int lanechange_vertical_speed; int lanechange_horizontal_speed;
  float lane_change_freq;
  template_vehicle()
  {
    type="";
    length=2;
    width=2;
    symbol='\0';
    max_speed=default_max_xspeed;
    max_acceleration=default_max_acceleration;
    lanechange_vertical_speed=default_lanechange_vertical_speed;
    lanechange_horizontal_speed=default_lanechange_horizontal_speed;

  }


};
class template_road
{
public:
  int id, length, width, signal;


};
void printRoad(Road* rd);
class Vehicle{
public:

 //existential parameters
  int id;//ids should be unique for all the vehicles
  string type;
  string color;
  int length;
  int width;
  char symbol;
  Road on_road;//road on which vehicle is moving

  //movement parameters
  int max_xspeed = 3;
  int max_acceleration = 1;
  float lane_change_freq = 0.5;
  int lanechange_horizontal_speed = 1;
  int lanechange_vertical_speed = 1;
  //overtaking done only from right side of vehicle to be overtaken
  //bool changing_lane = false;


  /*probability of overtaking given overtaking is possible*/
  /*we define overtaking is possible when a smaller vehicle can shift to a lane(s) on which no trace of the vehicle to be overtaken is found*/
  /*for time being assume instantaneous breaking is possible:- Scenario is almost true for indian drivers xD*/

  int velocity[2]={0,0};//velocity[0] is x_velocity and velocity[1] is y_velocity
  //int lane_no;//represents lane on which top right corner of vehicle is

  int pos[2] = {0,0};

  //bool** road_matrix = on_road.road_matrix;

  void changeVelocity(){/*lane changing and overtaking not yet coded */
  //  cout<<"entering changeVelocity"<<velocity[1]<<" "<<velocity[0]<<endl;
//  cout<<"Initial Velocity of "<<type<<" "<<id<<" "<<velocity[1]<<endl;

     float p = getRandom();
     //cout<<"Changing velocity for "<<type<<" and id "<<id<<endl;
     if( *(on_road.signal_color) == 'R'){

        p = 1;// to ensure no overtaking takes place when signal is red
     }
     int max_xvel = velocity[1]+max_acceleration;

     if(max_xvel > max_xspeed)max_xvel = max_xspeed;

     for(int i = 0; i<width ;i++){//ensures that driver takes maximum velocity possible so as to avoid collision
         for(int j = 1; j <= max_xvel ; j++){
          if(j+pos[1]< on_road.length){/*i+pos[0]< on_road.width -> this part must be true*/

              if(j+pos[1]>= on_road.signal_pos && *(on_road.signal_color) == 'R' && pos[1]<on_road.signal_pos){
                max_xvel = j-1;
              //  cout<<"making "<<type<<" "<<id<<" velocity "<<max_xvel<<endl;
        //        cout<<"WAITING "<<type<<" "<<endl;

                continue;
              }

              if(i+pos[0]>=on_road.width)continue;
              if(on_road.road_matrix[i+pos[0]][j+pos[1]] != ' '){
                max_xvel = j-1;
                continue;
              }
         }
       }
     }


     if(p > lane_change_freq && max_xvel!=0){// no lane_changing when max_xel!=0 and p says not to change
        velocity[1]= max_xvel;
         velocity[0]= 0;
       //cout<<type<<" velocity[0]:"<<velocity[0]<<" velocity[1]:"<<velocity[1]<<endl;
     }
    else{//assuming for now, overtaking can take place at given speeds only. We need to account later on for variable overtaking speeds
       //vehicle will plan to overtake if at current time there is no vehicle that could with max_xspeed  cause a collision

        bool turn_right = true;

        if( pos[0]+lanechange_vertical_speed+ width <=on_road.width && pos[0]+lanechange_vertical_speed>=0 ){
            for(int i = pos[0]+lanechange_vertical_speed ; i< pos[0]+lanechange_vertical_speed+width;i++){
              for(int j = pos[1]-length+1+lanechange_horizontal_speed;j <= pos[1]+lanechange_horizontal_speed;j++){
                  if(j>=0 && j<on_road.length){
                    if(i>=on_road.width)continue;
                    if(on_road.road_matrix[i][j]!=' ' && on_road.road_matrix[i][j]!=symbol){
                      turn_right = false;

                      break;
                    }
                  }
              }
              if(turn_right==false)break;
            }

             if(turn_right){//checks if any other vehicle could crash into it assuming the max_xspeed of current vehicle

                 // pos[0]+lanechange_vertical_speed   to pos[0]+lanechange_vertical_speed + width-1
                /* for(int i = pos[0]+lanechange_vertical_speed; i< pos[0]+lanechange_vertical_speed+width;i++){
                   for(int j = 1;j<=max_xspeed;j++){ OVERTAKING RULE BY ASSUMING THAT the j-limit is determined only by max_speed of overtaking vehicle and not vehicles behind them
                     if(pos[1]-length+1+lanechange_horizontal_speed-max_xspeed>=0 && pos[1]-length+1+lanechange_horizontal_speed - max_xspeed < on_road.length){
                        if(on_road.road_matrix[i][pos[1]-length+1+lanechange_horizontal_speed - max_xspeed]!=' '){
                          to_overtake = false;
                          break;
                        }
                      }  //O(n) in total
                   }


                   if(to_overtake==false)break;
                 }*/

                map<char,Vehicle*>::iterator sym_iter = on_road.symbol_maps.begin();
                int x_left = pos[1]+lanechange_horizontal_speed -length + 1;
                int x_right = pos[1]+lanechange_horizontal_speed;

                int y_top = pos[0]+lanechange_vertical_speed;
                int y_bottom = y_top + width -1;


                for(sym_iter = on_road.symbol_maps.begin();sym_iter != on_road.symbol_maps.end();sym_iter++){
                  // if we reach here we can be sure that there is no overlap between sym_iter->second vehicle and (this) vehicle.
                   Vehicle* temp_veh = sym_iter->second;
                   int vx_left = temp_veh -> pos[1] - length + 1;
                   int vx_right = vx_left + temp_veh->velocity[1]+ temp_veh->max_acceleration;// we need to cover the entire rectangle possibilities: if temp_veh moves max fast or it halts: hence vx_left will be current position of left width
                   int vx_top = temp_veh -> pos[0];
                   int vx_bottom = vx_top + temp_veh -> width -1;

                   //cout<<"LEFT:"<<x_left<<" "<<x_right<<" "<<y_top<<" "<<y_bottom<<endl;
                   //cout<<vx_left<<" "<<vx_right<<" "<<vx_top<<" "<<vx_bottom<<endl;
                   if( (vx_left - x_left)*(vx_left - x_right)<=0 || (vx_right - x_left)*(vx_right - x_right)<=0 ){
                        if( (vx_top - y_top)*(vx_top - y_bottom) <=0 || (vx_bottom - y_top)*(vx_bottom-y_bottom)<=0){
                          turn_right = false;

                          break;
                        }
                   }
                }

             }

         }
        else turn_right = false;



        bool turn_left = true;
        if( pos[0] - lanechange_vertical_speed + width<=on_road.width && pos[0]-lanechange_vertical_speed>=0){
            for(int i = pos[0]-lanechange_vertical_speed;i<pos[0]-lanechange_vertical_speed+width;i++){
              for(int j = pos[1]-length+1+lanechange_horizontal_speed;j <= pos[1]+lanechange_horizontal_speed;j++){
                  if(j>=0 && j<on_road.length){
                    if(i>=on_road.width)continue;
                    if(on_road.road_matrix[i][j]!=' ' && on_road.road_matrix[i][j]!=symbol){
                      turn_left = false;

                      break;
                    }
                  }
              }
              if(turn_left==false)break;
            }

           if(turn_left){
                map<char,Vehicle*>::iterator sym_iter = on_road.symbol_maps.begin();
                int x_left = pos[1]+lanechange_horizontal_speed -length + 1;
                int x_right = pos[1]+lanechange_horizontal_speed;

                int y_top = pos[0]-lanechange_vertical_speed;
                int y_bottom = y_top + width -1;


                for(sym_iter = on_road.symbol_maps.begin();sym_iter != on_road.symbol_maps.end();sym_iter++){
                  // if we reach here we can be sure that there is no overlap between sym_iter->second vehicle and (this) vehicle.
                   Vehicle* temp_veh = sym_iter->second;
                   int vx_left = temp_veh -> pos[1] - length + 1;
                   int vx_right = vx_left + temp_veh->velocity[1]+ temp_veh->max_acceleration;// we need to cover the entire rectangle possibilities: if temp_veh moves max fast or it halts: hence vx_left will be current position of left width
                   int vx_top = temp_veh -> pos[0];
                   int vx_bottom = vx_top + temp_veh -> width -1;

                   //cout<<"LEFT:"<<x_left<<" "<<x_right<<" "<<y_top<<" "<<y_bottom<<endl;
                   //cout<<vx_left<<" "<<vx_right<<" "<<vx_top<<" "<<vx_bottom<<endl;
                   if( (vx_left - x_left)*(vx_left - x_right)<=0 || (vx_right - x_left)*(vx_right - x_right)<=0 ){
                        if( (vx_top - y_top)*(vx_top - y_bottom) <=0 || (vx_bottom - y_top)*(vx_bottom-y_bottom)<=0){
                          turn_left = false;

                          break;
                        }
                   }
                }

             }

        }
        else turn_left = false;


          float direction_prob = getRandom();
          if(direction_prob > 0.5){
            if(turn_right){turn_left = false;}
          }
          else{
            if(turn_left){turn_right = false;}
          }

        if(turn_right && !(pos[1]< on_road.signal_pos  && pos[1]+lanechange_horizontal_speed>=on_road.signal_pos ) ) {
          velocity[1]=lanechange_horizontal_speed;
          velocity[0]=lanechange_vertical_speed;
        }
        else if(turn_left && !(pos[1]< on_road.signal_pos  && pos[1]+lanechange_horizontal_speed>=on_road.signal_pos )){
          velocity[1]=lanechange_horizontal_speed;
          velocity[0]=-lanechange_vertical_speed;
        }
        else{
          velocity[1] = max_xvel;
          velocity[0]=0;
        }
      //  if(to_overtake)cout<<"Overtaking by "<<type<<endl;
      //    else cout<<"NO overtaking "<<type<<endl;

      //  cout<<type<<" velocity[0]:"<<velocity[0]<<" velocity[1]:"<<velocity[1]<<endl;
    }
//  cout<<"Changed Velocity of "<<type<<" "<<id<<" "<<velocity[1]<<endl;
  // cout<<"leaving changeVelocity: "<<velocity[1]<<" "<<velocity[0]<<endl;
  }

  void changePosition(){// changing position in unit times
  //cout<<"entering changePosition"<<velocity[1]<<" "<<velocity[0]<<endl;
   pos[0] = pos[0]  + velocity[0];
   pos[1] = pos[1]  + velocity[1];
   //cout<<"leaving changePosition: "<<endl;
  }
};


void updateRoad(Road* rd){//function to update road matrix
  //cout<<"entering updateRoad"<<endl;
  map<int,Vehicle*>::iterator iter = rd->vehicles.begin();
  for(iter = rd->vehicles.begin();iter!= rd->vehicles.end();iter++){
  //  cout<<"before-Changing"<<iter->second.velocity[1]<<endl;;
//  cout<<"On road: "<<iter->second->type<<" and id "<<iter->second->id<<endl;
    (iter->second)->changeVelocity();
  //  cout<<"after-Changing"<<iter->second.velocity[1]<<endl;;
  }



  std::vector<int> ids_to_remove;
  std::vector<char> syms_to_remove;
  for(iter = rd->vehicles.begin();iter!= rd->vehicles.end();iter++)(iter->second)->changePosition();
  iter = rd->vehicles.begin();


  for(int i = 0;i<rd->width;i++)
   for(int j = 0;j<rd->length;j++)rd->road_matrix[i][j]=' ';


  for(iter = rd->vehicles.begin();iter!=rd->vehicles.end();iter++)
  {

    for(int i = 0;i<(iter->second)->width;i++){
      for(int j = 0;j<(iter->second)->length;j++){

        if((iter->second)->pos[1]>=j && (iter->second)->pos[1]-j<rd->length){
          if(i+(iter->second)->pos[0]>=rd->width)continue;
          rd->road_matrix[i+(iter->second)->pos[0]][(iter->second)->pos[1]-j]=iter->second->symbol;
        }


      }
    }
    if((iter->second)->pos[1]-(iter->second)->length+1>=rd->length){
         ids_to_remove.push_back(iter->first);
         syms_to_remove.push_back(iter->second->symbol);
    }
  }



  vector<int>::iterator vec_iter = ids_to_remove.begin();
  for(vec_iter = ids_to_remove.begin();vec_iter<ids_to_remove.end();vec_iter++){
    rd->vehicles.erase(*vec_iter);

    //cout<<"REMOVING:"<<*vec_iter<<endl;
  }
  vector<char>::iterator char_iter = syms_to_remove.begin();
  for(char_iter = syms_to_remove.begin();char_iter<syms_to_remove.end();char_iter++){
    rd->vehicles.erase(*char_iter);

    //cout<<"REMOVING:"<<*char_iter<<endl;
  }
//  cout<<"leaving UpdateRoad"<<endl;
  //updates the road by one unit time
  present_time++;

  /*if(present_time %8 < 4){
    *(rd->signal_color) = 'R';
  }
  else if(present_time%8==5){
    *(rd->signal_color) = 'Y';
  }
  else{
    *(rd->signal_color) = 'G';
  }*/
}

void printRoad(Road* rd){
  cout<<"Signal Color:"<<rd->signal_color<<endl;
  for(int i = 0;i<2*rd->length;i++)cout<<"-";
  cout<<endl;
  for(int i= 0;i<rd->width;i++){

    for(int j =0;j<rd->length;j++){
      cout<<rd->road_matrix[i][j];
      if(j+1==rd->signal_pos)cout<<"|";else cout<<" ";
    }
    cout<<endl;
    for(int j =0;j<2*rd->length;j++)cout<<"-";
    cout<<endl;
  }
  cout<<endl;
}

void updatePositionsOnRoad(Road* rd,bool print=false){

/*  if(present_time %8 < 4){
    *(rd->signal_color) = 'R';
  }
  else if(present_time%8==5){
    *(rd->signal_color) = 'Y';
  }
  else{
    *(rd->signal_color) = 'G';
  }
*/
  for(int i = 0;i<rd->width;i++)
   for(int j = 0;j<rd->length;j++)rd->road_matrix[i][j]=' ';

   map<int,Vehicle*> vehicles = rd->vehicles;
   map<int,Vehicle*>::iterator iter = vehicles.begin();

  for(iter = vehicles.begin();iter!=vehicles.end();iter++)
  {

    for(int i = 0;i<(iter->second)->width;i++){
      for(int j = 0;j<(iter->second)->length;j++){
        if((iter->second)->pos[1]>=j && (iter->second)->pos[1]-j<=rd->length){
          if(i+(iter->second)->pos[0]>=rd->width)continue;
          rd->road_matrix[i+(iter->second)->pos[0]][(iter->second)->pos[1]-j]=iter->second->symbol;
        }
      }
    }
  }

  if(print)printRoad(rd);
 // no change in time. Just to initialize the roadMatrix.
}



void updateRoad(Road* road, int t,bool print){
    if(t <=0 )cout<<"ERROR:Please enter valid time input"<<endl;
    else{
      for(int i=0;i<t;i++){

        updateRoad(road);
      if(print)printRoad(road);
        cout<<endl;
      }

    }

}

Vehicle* createVehicleOnRoad( string type, string color , int length, int width, Road* rd, char symbol,int lane_no,int column_no, int max_speed, int max_acceleration, int lanechange_vertical_speed, int lanechange_horizontal_speed, int ini_vertical_velocity, int ini_horizontal_velocity,float lane_change_freq){
  Vehicle* newVehicle =  new Vehicle;
  newVehicle->id = vehicle_id;
  vehicle_id++;//update the vehicle id
  newVehicle->type = type;
  newVehicle->color = color;
  newVehicle->length = length;
  newVehicle->symbol = symbol;
  newVehicle->width = width;
  newVehicle->on_road = *(rd);
  newVehicle->pos[0] = lane_no;
  newVehicle->pos[1] = column_no;
  newVehicle->max_xspeed = max_speed;
  newVehicle->max_acceleration = max_acceleration;
  newVehicle->lanechange_vertical_speed = lanechange_vertical_speed;
  newVehicle->lanechange_horizontal_speed = lanechange_horizontal_speed;
  newVehicle->velocity[0] = ini_vertical_velocity;
  newVehicle->velocity[1] = ini_horizontal_velocity;
  newVehicle->lane_change_freq = lane_change_freq;
  (*rd).vehicles.insert(std::pair<int,Vehicle*>(newVehicle->id,newVehicle));

  (*rd).symbol_maps.insert(std::pair<char,Vehicle*>(newVehicle->symbol,newVehicle));
  return newVehicle;
}

void addVehicleOnRoad(Vehicle* newVehicle , Road * rd){
  newVehicle->on_road = *(rd);
   //cout<<"Previous Size "<<(*rd).vehicles.size();
  // cout<<"type "<< newVehicle->type << " and id "<<newVehicle->id<<" for newVehicle"<<endl;
  (*rd).vehicles.insert(std::pair<int,Vehicle*>(newVehicle->id,newVehicle));
  (*rd).symbol_maps.insert(std::pair<char,Vehicle*>(newVehicle->symbol,newVehicle));
  //cout<<" Later Size "<<(*rd).vehicles.size()<<endl;
}

Vehicle* createVehicle( string type, string color , int length, int width, char symbol,int lane_no,int column_no, int max_speed, int max_acceleration, int lanechange_vertical_speed, int lanechange_horizontal_speed, int ini_vertical_velocity, int ini_horizontal_velocity,float lane_change_freq){
  Vehicle* newVehicle = new Vehicle;
  newVehicle->id = vehicle_id;
  vehicle_id++;//update the vehicle id
  newVehicle->type = type;
  newVehicle->color = color;
  newVehicle->length = length;
  newVehicle->symbol = symbol;
  newVehicle->width = width;

  newVehicle->pos[0] = lane_no;
  newVehicle->pos[1] = column_no;
  newVehicle->max_xspeed = max_speed;
  newVehicle->max_acceleration = max_acceleration;
  newVehicle->lanechange_vertical_speed = lanechange_vertical_speed;
  newVehicle->lanechange_horizontal_speed = lanechange_horizontal_speed;
  newVehicle->velocity[0] = ini_vertical_velocity;
  newVehicle->velocity[1] = ini_horizontal_velocity;
  newVehicle->lane_change_freq = lane_change_freq;
  return newVehicle;
}
template_vehicle find_in(string name, vector<template_vehicle> vec)
{
  template_vehicle temp;
  for(auto k=vec.begin();k!=vec.end();k++)
  {
    if(k->type.compare(name)==0)
      return *k;
  }
  return temp;
}
int main(int argc, char** argv)
{
  present_time = 1;//initiating the time
  vehicle_id = 1;// initiating vehicle_id: each vehicle has its own id

  Road road;

  road.length = road_len;
  road.width = road_wid;
  road.id = 1;
  int vehicle_no=0;
  road.signal_pos = signal_pos;
//  std::vector< Vehicle> vec_vehicle;
  char** road_matrix = new char*[road_wid];
  for(int i = 0;i<road_wid;i++){
    road_matrix[i]=new char[road_len];
   for(int j = 0;j<road_len;j++)road_matrix[i][j]=' ';
  }
  road.road_matrix = road_matrix;
  if(argc<2)
  {
    cout<<"Error: no config file passed"<<endl;
    return -1;
  }
  ifstream file(argv[1]);
  std::stringstream buffer;
  if ( file )
  {
        buffer << file.rdbuf();
        file.close();
  }

        std::vector<std::string> result;
        //std::stringstream stream(string);
        std::string word;
        while (std::getline(buffer, word, '\n'))
        {
            size_t found = word.find("#");
            if(word.length()==0)
              {
                result.push_back(word);
                continue;
              }
            if (found != string::npos)
                word=word.substr(0,found);
            if(word.length()>0)
              result.push_back(word);
        }
      result.push_back("");
     auto k=result.begin();
     for(;k!=result.end();k++)
     {
      cout<<*k<<endl;
     }
  std::vector< std::vector<std::string> > tokens;  //every element of vector stores a vector storing one section line by line
  auto it=result.begin();
  std::vector<std::string> temp;
  for(;it!=result.end();it++)
  {

      if(it->length()==0)
        {if(temp.size()>0)
          {tokens.push_back(temp);}
        temp.clear();}
      else
        temp.push_back(*it);
  }
  /*for(auto it1=tokens.begin();it1!=tokens.end();it1++)
  {
    for(auto it2=it1->begin();it2!=it1->end();it2++)
    {
      std::cout<<*it2<<std::endl;
    }
    std::cout<<endl<<"--------------------------------------------------"<<std::endl;
  }*/
  std::vector< template_vehicle > all_vehicles;
  bool mode=false;
  for(auto it1=tokens.begin();it1!=tokens.end();it1++)
  {
    //std::vector<std::string> instr=*it;
    int i=0;
    std::string type, ins=*(it1->begin());
    if(ins.length()==0)
      continue;
    if(ins.substr(0,5).compare("START")==0)
      {
        mode=true;
        continue;
      }
    if(mode==false)
     {
      size_t findc=ins.find("_");
      if (findc != string::npos)
        {
          type=ins.substr(0,findc);
        }
      else
      {
        cout<<"Error while parsing, no underscore used "<<endl;
        return -1;
      }
      if(type.compare("Vehicle")==0)
        {
          template_vehicle new_vehic;
          try{
          for(auto it2=it1->begin();it2!=it1->end();it2++)
          {
            std::string property, grabage, value;
            istringstream ss(*it2);
            ss>>property;
            ss>>grabage;
            ss>>value;
            if(property.compare("Vehicle_Type")==0)
            {
                new_vehic.type=value;
            }
            else if(property.compare("Vehicle_Length")==0)
            {
                new_vehic.length=stoi(value);
            }
            else if(property.compare("Vehicle_Width")==0)
            {
                new_vehic.width=stoi(value);
            }
            else if(property.compare("Vehicle_MaxSpeed")==0)
            {
                new_vehic.max_speed=stoi(value);
            }
            else if(property.compare("Vehicle_MaxAcceleration")==0)
            {
                new_vehic.max_acceleration=stoi(value);
            }
            else if(property.compare("Vehicle_LChange_v_speed")==0)
            {
                new_vehic.lanechange_vertical_speed=stoi(value);
            }
            else if(property.compare("Vehicle_LChange_h_speed")==0)
            {
                new_vehic.lanechange_horizontal_speed=stoi(value);
            }


          }
        }
        catch(std::invalid_argument& e){
          cout<<"error in conversion of vehicle"<<endl;}
          all_vehicles.push_back(new_vehic);
        }
      else if(type.compare("Default")==0)
        {
          try{template_vehicle new_vehic;
          for(auto it2=it1->begin();it2!=it1->end();it2++)
          {
            std::string property, grabage, value;
            istringstream ss(*it2);
            ss>>property;
            ss>>grabage;
            ss>>value;
            if(property.compare("Default_MaxSpeed")==0)
            {
                default_max_xspeed=stoi(value);
            }
            else if(property.compare("Default_Acceleration")==0)
            {
                default_max_acceleration=stoi(value);
            }
            else if(property.compare("Default_LChange_v_speed")==0)
            {
                default_lanechange_vertical_speed=stoi(value);
            }
            else if(property.compare("Default_LChange_h_speed")==0)
            {
                default_lanechange_horizontal_speed=stoi(value);
            }
          }
         }
         catch(std::invalid_argument& e){
          cout<<"error in conversion of default"<<endl;}
         }
        else if(type.compare("Road")==0)
        {
          try{
          for(auto it2=it1->begin();it2!=it1->end();it2++)
          {
            std::string property, grabage, value;
            istringstream ss(*it2);
            ss>>property;
            ss>>grabage;
            ss>>value;
            if(property.compare("Road_Length")==0)
            {
                road.length=stoi(value);
            }
            else if(property.compare("Road_Width")==0)
            {
                road.width=stoi(value);
            }
            else if(property.compare("Road_Id")==0)
            {
              road.id=stoi(value);
            }
            else if(property.compare("Road_Signal")==0)
            {
              road.signal_pos=stoi(value);
            }
            *(road.signal_color)='R';
          }
        }
         catch(std::invalid_argument& e){
          cout<<"error in conversion of road"<<endl;}
        }
      }
    else
    {

      string name, color="red";
      int length=2, width=1, lane_no=0, col_no=0, max_speed=default_max_xspeed, max_acceleration=default_max_acceleration, lnchangeh=default_lanechange_horizontal_speed, lnchangev=default_lanechange_vertical_speed, v_vel=0, h_vel=0;float lchang_f=0;
      for(auto it2=it1->begin();it2!=it1->end();it2++)
          {
            std::vector<std::string> attributes;
            cout<<"Input: "<<*it2<<endl;
            istringstream ss(*it2);
            while(ss)
            {
              string temp;
              ss>>temp;

              if(temp.compare(" ")!=0)
              {

                attributes.push_back(temp);
              }
            }

            if(attributes[0].compare("Pass")==0)
            {
              try{updateRoad(&road,stoi(attributes[1]),true);}
              catch(std::invalid_argument& e){
              cout<<"error in conversion of time in PASS"<<endl;}

              continue;
            }
            if(attributes[0].compare("Signal")==0)
            {

              *(road.signal_color)=toupper(attributes[1].at(0));

              continue;
            }
            template_vehicle to_load=find_in(attributes[0], all_vehicles);
            if(to_load.type.length()==0)      //no vehicle found
              {
                cout<<attributes[0]<<" was not defined"<<endl;
                continue;
              }
            else
            {
              name=to_load.type;
              length=to_load.length;
              width=to_load.width;
              max_speed=to_load.max_speed;
              max_acceleration=to_load.max_acceleration;
              lnchangeh=to_load.lanechange_horizontal_speed;
              lnchangev=to_load.lanechange_vertical_speed;
              color=attributes[1];
              try{
              if(attributes.size()>=4)
               {
                lane_no=stoi(attributes[2]);
                if(attributes.size()>=5)
                      {
                        col_no=stoi(attributes[3]);
                        if(attributes.size()>=6)
                        {
                          v_vel=stoi(attributes[4]);
                          if(attributes.size()>=7)
                          {
                            h_vel=stoi(attributes[5]);
                            if(attributes.size()>=8)
                              lchang_f=stof(attributes[6]);

                          }
                        }
                      }
                }
                }
                catch(std::invalid_argument& e){
              cout<<"error in conversion of non existent "<<endl; return -1;}
              map<int,Vehicle*>::iterator iter = road.vehicles.begin();


            Vehicle *temp_vehicle=((createVehicle(name,color,length,width,name.at(0),lane_no,col_no,max_speed,max_acceleration,lnchangev,lnchangeh,v_vel, h_vel, lchang_f)));
            while(true) //incomplete check for clash but it works assuming contagious vehicles
            {
<<<<<<< HEAD
             updateRoad(&road,1,true);
=======
              bool isEmpty = true;
               for(int i = lane_no;i<lane_no+width;i++){
                for(int j =col_no;j<col_no+length;j++){
                  if( i < road.width && j<road.length){
                    if(road.road_matrix[i][j]!=' '){
                      isEmpty = false;
                      break;
                    }
                  }
                }
                if(isEmpty == false)break;
              }
             if(isEmpty)break;
             else updateRoad(&road,1,true);
>>>>>>> efe7d6c936d4ee2e0b7de4e388584604938c6924
            }

            addVehicleOnRoad(temp_vehicle, &road);

            vehicle_no++;
<<<<<<< HEAD
            cout<<"why"<<endl;
            printRoad(&road);
            updatePositionsOnRoad(&road,true);
            updateRoad(&road,1,true);
=======

            updatePositionsOnRoad(&road,true);
            //updateRoad(&road,1,true);
>>>>>>> efe7d6c936d4ee2e0b7de4e388584604938c6924
            }

          }

    }

  }



  return 0;
}

/*int main(int argc, char** argv){
  present_time = 1;//initiating the time
  vehicle_id = 1;// initiating vehicle_id: each vehicle has its own id




  Road road;
  road.length = road_len;
  road.width = road_wid;
  road.id = 1;
  road.signal_pos = signal_pos;
  char** road_matrix = new char*[road_wid];
  for(int i = 0;i<road_wid;i++){
    road_matrix[i]=new char[road_len];
   for(int j = 0;j<road_len;j++)road_matrix[i][j]=' ';
  }
  road.road_matrix = road_matrix;

  queue<Vehicle> entry_queue;


  Vehicle mycar = createVehicle("car","red",2,2,'c',0,0,default_max_xspeed,default_max_acceleration,default_lanechange_vertical_speed,default_lanechange_horizontal_speed,default_vertical_velocity, default_horizontal_velocity, default_lane_change_freq);
  Vehicle bike = createVehicle("bike","red",3,1,'b',0,3,default_max_xspeed,default_max_acceleration,default_lanechange_vertical_speed,default_lanechange_horizontal_speed,default_vertical_velocity, default_horizontal_velocity, default_lane_change_freq);

  addVehicleOnRoad(&mycar, &road);
  addVehicleOnRoad(&bike, &road);


  updatePositionsOnRoad(&road,true);


  updateRoad(&road,4,true);

}*/
