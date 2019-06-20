#Traffic-Simulator
Road_Id = ... <br />
Road_Length = ...<br />
Raod_Width = ...<br />
Road_Signal = ...<br />
<br />
<br />
Default_MaxSpeed = ..<br />
Default_Acceleration= ...<br />
Default_Lane_Change_Probability= ...<br />
<br />
<br />
begin new Vehicle<br />
Vehicle_Type<br />
<br />
...<br />
...<br />

end new Vehicle<br />
<br />
<br />
...<br />
<br />

#Definitions over<br />
<br />
#Send vechicles with given types with given types on road ( num_vps per second)<br />
#Try to send as many vehicles possible per second preferring vehicles who occur first<br />
#in configuration file i.e Car GREEN preferred over bike BLUE<br />


Car GREEN<br />
bike BLUE<br />
Truck GREEN<br />
Bus BLUE<br />

Pass 15<br />

set_num_vps 3<br />

Car RED 3<br />
#the parameter 3 is used to denote where the top right edge of rectangle should be<br />
#If the vehicle can enter at that point it will enter but if not it will see the best fit<br />

#If some vehicles are unable to enter since non-availability of roads, add them to queue<br />

<br />
...
...<br />
...<br />
<br />
