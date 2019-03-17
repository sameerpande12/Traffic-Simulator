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
    symbol=' ';
    max_speed=default_max_xspeed;
    max_acceleration=default_max_acceleration;
    lanechange_vertical_speed=default_lanechange_vertical_speed;
    lanechange_horizontal_speed=default_lanechange_horizontal_speed;

  }


};