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
#include<any>
#include<cstring>
#include<string>
#include <iostream>
#include<map>
#include<vector>
#include<random>
using namespace std;

std::random_device random_gen;
std::mt19937 gen(random_gen());
std::uniform_real_distribution<>dis(0.0,1.0);


float getRandom(){
  return (float) dis(gen);
};//returns a float between 0 to 1 in uniform distribution
class Vehicle;
class Road{
public:
  int id;//
  int length;//
  int width;// make them of type final
  char** road_matrix;
  map<int,Vehicle*> vehicles;
  int sam=100;
};
void printRoad(Road rd);
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
  float lane_change_freq = 0.3;
  float overtake_freq = 0.4;
  int overtake_horizontal_speed = 1;
  int overtake_vertical_speed = 1;
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
     float p = getRandom();

     int max_xvel = velocity[1]+max_acceleration;

     if(max_xvel > max_xspeed)max_xvel = max_xspeed;

     for(int i = 0; i<width ;i++){//ensures that driver takes maximum velocity possible so as to avoid collision
         for(int j = 1; j <= max_xvel ; j++){

          if( /*i+pos[0]< on_road.width -> this part must be true*/  j+pos[1]< on_road.length){
              if(on_road.road_matrix[i+pos[0]][j+pos[1]] != ' '){
                max_xvel = j-1;
                continue;
              }
         }
       }
     }


     if(p > overtake_freq){ // no overtaking
       velocity[1]= max_xvel;
       velocity[0]= 0;
       cout<<type<<" velocity[0]:"<<velocity[0]<<" velocity[1]:"<<velocity[1]<<endl;
     }
    else{//assuming for now, overtaking can take place at given speeds only. We need to account later on for variable overtaking speeds
     //vehicle will plan to overtake if at current time there is no vehicle that could with max_xspeed  cause a collision
      bool to_overtake = true;
      if(pos[0]+overtake_vertical_speed+length<=on_road.width && pos[0]+overtake_vertical_speed>=0){
         for(int i = 1;i<=overtake_horizontal_speed;i++){
            for(int j =1; j<=overtake_vertical_speed;j++){
               if(pos[1]+i<on_road.length){
                  if(on_road.road_matrix[j+pos[0]][pos[1]+i]!=' '){
                    to_overtake = false;
                    break;
                  }

               }
            }

            if(to_overtake==false)break;
         }

         if(to_overtake){//checks if any other vehicle could crash into it assuming the max_xspeed of current vehicle

             // pos[0]+overtake_vertical_speed   to pos[0]+overtake_vertical_speed + width-1
             for(int i = pos[0]+overtake_vertical_speed; i< pos[0]+overtake_vertical_speed+width;i++){
               for(int j = 1;j<=max_xspeed;j++){
                 if(pos[1]-length+1+overtake_horizontal_speed-max_xspeed>=0 && pos[1]-length+1+overtake_horizontal_speed - max_xspeed < on_road.length){
                    if(on_road.road_matrix[i][pos[1]-length+1+overtake_horizontal_speed - max_xspeed]!=' '){
                      to_overtake = false;
                      break;
                    }
                  }
               }
               if(to_overtake==false)break;
             }

         }
      }
      else to_overtake = false;

      if(to_overtake){
        velocity[1]=overtake_horizontal_speed;
        velocity[0]=overtake_vertical_speed;
      }
      else{
        velocity[1] = max_xvel;
        velocity[0]=0;
      }
      if(to_overtake)cout<<"Overtaking by "<<type<<endl;
      else cout<<"NO overtaking "<<type<<endl;

      cout<<type<<" velocity[0]:"<<velocity[0]<<" velocity[1]:"<<velocity[1]<<endl;
    }

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
    (iter->second)->changeVelocity();
  //  cout<<"after-Changing"<<iter->second.velocity[1]<<endl;;
  }



  std::vector<int> ids_to_remove;
  for(iter = rd->vehicles.begin();iter!= rd->vehicles.end();iter++)(iter->second)->changePosition();
  iter = rd->vehicles.begin();


  for(int i = 0;i<rd->width;i++)
   for(int j = 0;j<rd->length;j++)rd->road_matrix[i][j]=' ';


  for(iter = rd->vehicles.begin();iter!=rd->vehicles.end();iter++)
  {

    for(int i = 0;i<(iter->second)->width;i++){
      for(int j = 0;j<(iter->second)->length;j++){

        if((iter->second)->pos[1]>=j && (iter->second)->pos[1]-j<rd->length){
          rd->road_matrix[i+(iter->second)->pos[0]][(iter->second)->pos[1]-j]=iter->second->symbol;
        }

      }
    }
    if((iter->second)->pos[1]-(iter->second)->length+1>=rd->length){
         ids_to_remove.push_back(iter->first);
    }
  }



  vector<int>::iterator vec_iter = ids_to_remove.begin();
  for(vec_iter = ids_to_remove.begin();vec_iter<ids_to_remove.end();vec_iter++){
    rd->vehicles.erase(*vec_iter);
    cout<<"REMOVING:"<<*vec_iter<<endl;
  }
//  cout<<"leaving UpdateRoad"<<endl;
  //updates the road by one unit time
}

void updatePositionsOnRoad(Road* rd){
  for(int i = 0;i<rd->width;i++)
   for(int j = 0;j<rd->length;j++)rd->road_matrix[i][j]=' ';

   map<int,Vehicle*> vehicles = rd->vehicles;
   map<int,Vehicle*>::iterator iter = vehicles.begin();

  for(iter = vehicles.begin();iter!=vehicles.end();iter++)
  {

    for(int i = 0;i<(iter->second)->width;i++){
      for(int j = 0;j<(iter->second)->length;j++){
        if((iter->second)->pos[1]>=j && (iter->second)->pos[1]-j<=rd->length){
          rd->road_matrix[i+(iter->second)->pos[0]][(iter->second)->pos[1]-j]=iter->second->symbol;
        }
      }
    }
  }
}
void printRoad(Road* rd){
  for(int i = 0;i<2*rd->length;i++)cout<<"-";
  cout<<endl;
  for(int i= 0;i<rd->width;i++){
    cout<<"|";
    for(int j =0;j<rd->length;j++)cout<<rd->road_matrix[i][j]<<"|";
    cout<<endl;
    for(int j =0;j<2*rd->length;j++)cout<<"-";
    cout<<endl;
  }
  cout<<endl;
}
void updateRoad(Road* road, int t){
  if(t <=0 )cout<<"ERROR:Please enter valid time input"<<endl;
  else{
    for(int i=0;i<t;i++){
      updateRoad(road);
      printRoad(road);
      cout<<endl;
    }
  }
}

int main(int argc, char** argv){
  Road road;
  int rlen = 15;
  int rwid = 5;
  road.length = rlen;
  road.width = rwid;
  road.id = 1;
  char** road_matrix = new char*[rwid];
  for(int i = 0;i<rwid;i++){
    road_matrix[i]=new char[rlen];
   for(int j = 0;j<rlen;j++)road_matrix[i][j]=' ';
  }

  road.road_matrix = road_matrix;

  Vehicle mycar;
  mycar.id = 1;
  mycar.type = "car";
  mycar.color = "red";
  mycar.length = 2;
  mycar.width = 2;
  mycar.on_road = road;
  mycar.symbol='c';
  road.vehicles.insert(std::pair<int,Vehicle*>(mycar.id,&mycar));

  Vehicle bike;
  bike.id = 2;
  bike.type = "bike";
  bike.color = "red";
  bike.length = 3;
  bike.width = 1;
  bike.pos[0]=0;
  bike.pos[1]=3;
  bike.on_road = road;
  bike.symbol = 'b';
  road.vehicles.insert(std::pair<int,Vehicle*>(bike.id,&bike));


updatePositionsOnRoad(&road);
printRoad(&road);
cout<<endl;//initial condition upon definition

updateRoad(&road,5);


}
