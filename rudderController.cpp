/* Rudder Control 
Description: The rudder controller is responsible for adjusting the boat orientation
with respect to the defined trajectory. The directions of the compass and the desired path 
are compared 

Inputs: desiredPath - angle that the bsailboat needs to travel to reach the waypoint
		heading - actual direction (angle) found via compass

Outputs: angle_rudder - angle of the rudder (type float)


Jordan Leung 5/16/18
*/


#include <StandardCplusplus.h>
#include <stdio.h>
#include <stdlib.h>
#include <Servo.h>
#include <math.h>

Servo servoR; 


// define rudder negative and positive limits 
float rudder_pos = 45;
float rudder_neg = -45; 

//initialize PID variables

float errorActual = 0; // error = desired angle - actual angle 
float Iaccum = 0;    // accummulator of integrative error.
float Kp = 2;           // proportional gain - starting value suggested by Davi
float Ki = 1;       // integral gain - starting value suggsted by Davi
float angle_rudder = 0;
float desiredPath = 0;
float heading = 0; 
//approximate time of code execution
float sample_time = 0.2; // sample time of the system (in seconds)




// define max and min limits for the boat's orientation (angle)
float saturator(float value) {
	if (value > 180) { 
		value = value - 360;
	}
	if (value < -180) {
		value = value + 360;
	}
	return value;
}

// define the max and min limits for the rudder angles 
float saturator_rudder(float servoRudderAngle) {
	if (servoRudderAngle > rudder_pos) {
		servoRudderAngle = rudder_pos;

	}
	if (servoRudderAngle < rudder_neg) {
		servoRudderAngle = rudder_neg;
	}
	return servoRudderAngle;
}



float rudder_controller(float desiredPath, float heading) {
	// desiredPath - angle that the sailboat needs to travel to reach waypoint
	desiredPath = saturator(desiredPath);
	// error between the actual and desired path
	// heading - actual angle found via compass
	errorActual = desiredPath - heading; 
	errorActual = saturator(errorActual);

	// apply PI theory 
	control_act = P() + I();
	// turning the boat in the clockwise direction 
	if (errorActual > 0) {
		angle_rudder = rudder_pos/2 + (rudder_pos - rudder_pos/2)*(control_act/180);
	}
	// turning the boat in the counterclockwise direction
	else {
		angle_rudder = rudder_pos/2 + (rudder_neg - rudder_pos/2)*(control_act/180);
	}
	// his laws 
	//angle_rudder = 90 * (errorActual/180);
	//error = desired angle - actual angle
	//errorActual = angle_rudder;

	angle_rudder = saturator_rudder(angle_rudder);
	return angle_rudder;
}

float P() 
{
	return Kp * errorActual;
}

//error is reduced at each iteration 
float I() 
{
    // I = I + Ki* Er* Tp                                    
	Iaccum = Iaccum + Ki * errorActual * sample_time;
    return Iaccum;
}
