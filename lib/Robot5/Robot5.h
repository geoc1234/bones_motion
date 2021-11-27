#ifndef Robot5_H
#define Robot5_H

#include <Arduino.h>
#include <Sabertooth.h>
#include <Encoder.h>

//define data structures
struct pose_t
{ // components of the base pose returned to the pi
	float x;	// loc x in odom frame (mm)
	float y; //  loc y in odom frame (mm)
	float w; // azimuth about z (theta) in odom frame (mrads)
	float ds; // speed in base frame (speed in the forward direction) (mm/sec)
	float ws; // angular in base frame (about the z-axis) (mrads/sec)
};

// instantiate functions
extern Sabertooth ST;
extern Encoder lwEnc;
extern Encoder rwEnc;

template <typename T> T mysgn(T num)
{ // return the sign of a variable.  works on arduino 
	return (num > 0 ? 1 : -1);
}

template <class T>
class MyBuffer
{ // provides stack-like structure for the arduino
public:
	MyBuffer(int N = 10); 
	void push_back(const T&); 
	T mean();
	void clear();

private:
	int size ;  // Max Number of elements on buffer
	int num ;   // number of elements added to the buffer
	T* buffPtr;  // the actual buffer
};

typedef MyBuffer<float> floatBuffer;

class Wheel
{
	public:

		Wheel(uint8_t motorID, float diameter, Sabertooth * st, Encoder * enc);
		void resetEnc(float dist);
		void drive(float spdTarget);
		void drive(float spdTarget, float trim);
		void findDeadBand();
		void removeBacklash(int dir);
		void stop();
		float get_speed();
		float get_distance();
		void set_distTol(float d);
		void set_deadband(int8_t db);
		void setPower(int8_t p);

		Encoder * enc_;
		Sabertooth * st_;
		
		int8_t _deadband;
		int8_t _power;
		int8_t _powerLast;

	private:

		floatBuffer speedBuffer;
		float _speed;
		float _speedLast;
		float _speedAverage[10];
		int		_spdAveIndx;
		float _speedErrorLast;
		float _diameter;
		float _counts_per_unitdist; 
		float _dist_per_count;
		float _distance;
		float _distanceLast;
		float _distanceLastSpeed;
		float _distTol;
		int8_t _motorID;
		uint32_t _timeLastSpeed;
		float _countsLastSpeed;
		float pwrSmooth;
};

class Robot
{
public:	
	//list the Constructors
	Robot(float wheelBase, Wheel * lwheel, Wheel * rwheel);

	//list prototypes of the methods
	void getOdometer();
	void getHeadingTarget();
	float getHeadingError();
	void go();
	void stop();
	void resetEnc(float dist, float heading);
	void twist();
	bool safetyCheck(bool OK2run);	

// position variables
	float _angularSpeed;		//rads per second
	float _speed;				//cm per second
	float _heading; 			//pointer to the heading in the Loc_t structure
	float _headingLast;
	float _distance;	    //distance travelled (in) since last encoder reset
	float _distanceLast;    //distance travelled (in) since last Odometer reading
	float _speedTarget;
	float _angularSpeedTarget;
	
private: 	//make things private to keep them accessible only to members of the class

	Wheel * _lwheel;
	Wheel * _rwheel;

	float _wheelBase;
	float _default_turn_speed;

	// targeting variables
	float _distanceTarget ;		 //distance target to travel at current heading
	uint32_t _timeLastSpeed;
};

#endif