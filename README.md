#Traffic-Simulator
Road_Id = ...
Road_Length = ...
Raod_Width = ...
Road_Signal = ...


Default_MaxSpeed = ..
Default_Acceleration= ...
Default_Lane_Change_Probability= ...


begin new Vehicle
Vehicle_Type

...
...

end new Vehicle


...


#Definitions over

#Send vechicles with given types with given types on road ( num_vps per second)
#Try to send as many vehicles possible per second preferring vehicles who occur first
#in configuration file i.e Car GREEN preferred over bike BLUE


Car GREEN
bike BLUE
Truck GREEN
Bus BLUE

Pass 15

set_num_vps 3

Car RED 3
# the parameter 3 is used to denote where the top right edge of rectangle should be
#If the vehicle can enter at that point it will enter but if not it will see the best fit

#If some vehicles are unable to enter since non-availability of roads, add them to queue


...
...
...
