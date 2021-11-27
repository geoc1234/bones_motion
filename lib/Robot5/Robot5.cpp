#include "Robot5.h"

template <class T>
MyBuffer<T>::MyBuffer(int N)
{
	num = 0;  
	size = (N > 0 && N < 10 ? N : 10);
	buffPtr = new T[size];
	this->clear();
}

 // push an element onto the Buffer 
template <class T>
void MyBuffer<T>::push_back(const T& elemnt)
{
	num = (num < (size - 1)) ? num : (size - 1);
	
	for(int i=++num; i>0; i--)
	{
		buffPtr[i] = buffPtr[i-1];
	}	
	buffPtr[0] = elemnt;
}

template <class T>
void MyBuffer<T>::clear() 
{
	for(int i=0; i<size; i++)
	{
		buffPtr[i] = 0;
	}
}

template <class T>
T MyBuffer<T>::mean() 
{
	T container = buffPtr[0];
	for(int i=1; i<num; i++)
	{
		container += buffPtr[i];
	}
	return container / num;
}

Wheel::Wheel(uint8_t motorID, float diameter, Sabertooth * st, Encoder * enc)
{
    _diameter = diameter;
    enc_ = enc;
	st_  =  st;
	_motorID = motorID;

	float EncoderPPR = 20;			//encoder pulses per rotatiom
	float GearboxRR = 84;			//gear box reduction ratio
	_counts_per_unitdist = (EncoderPPR * GearboxRR) / (3.14 * _diameter); //counts per mm travel ~ 3
	_dist_per_count = (1/ _counts_per_unitdist); // mm travel per counts
	_distTol = 100; //mm
	_timeLastSpeed = millis();

    speedBuffer = floatBuffer(3);

}

void Wheel::setPower(int8_t p)
{
	st_->motor(_motorID,p);
	if(p == 0)
	{
		speedBuffer.clear();
	}
}

float Wheel::get_distance()
{
	_distanceLast = _distance;
	_distance = (_dist_per_count * enc_->read());
	return _distance;
}
float Wheel::get_speed()
{
	uint32_t time = millis();

	// check that clock hasn't rolled over. if it has, return last speed
	if(time < _timeLastSpeed)
	{
		return _speedLast;
		_timeLastSpeed = millis();
	}

	uint32_t delTime = (time - _timeLastSpeed);
	float delDist = get_distance() - _distanceLastSpeed;
	float spd = (delDist * 1000) / (float) delTime;
	speedBuffer.push_back(spd);
	_timeLastSpeed = time;
	_speedLast = _speed;
	_distanceLastSpeed = _distance;
	_speed = speedBuffer.mean();
	return  _speed;
}

void Wheel::drive(float spdTarget)
{// Rotates wheel at target speed 
	static int counter;

	if(spdTarget != 0)
	{
	    float spdError =  (spdTarget - get_speed());
		int8_t pwrchange = mysgn<float>(spdError);
		
		//check for banging
		if(mysgn<float>(spdError) != mysgn<float>(_speedErrorLast))
		{
			pwrchange = 0;
			counter = 0;
		}
		else if(mysgn<float>(spdError) == mysgn<float>(_speedErrorLast))
		{
			counter++;
			if(counter > 4)
			{
				counter = 0;
				_power += pwrchange;
			}
		}

		int8_t appliedpower = _power + (_power > 0 ? _deadband : (_power < 0 ? -_deadband : 0));
		setPower(appliedpower);
		_speedErrorLast = spdError;
	}
	else
	{
		_power = 0;
		setPower(_power);
		speedBuffer.clear();
	}

	_powerLast = _power;
}

void Wheel::findDeadBand()
{
	// remove the backlash
	for(int i = 0; i < 40; i++)
	{
		setPower(i);
		delay(100 - 2*i);
	}
	setPower(0);
	_power = 0;
	
	// ramp power unit counts change by two units
	int32_t current_counts = enc_->read();
	while(enc_->read() - current_counts < 2 && _power < 127)
	{
		_power++;
		setPower(_power);
		delay(30);
	}
	_deadband = _power;
	_power=0;
	setPower(_power);
}
		
void Wheel::removeBacklash(int dir)
{
//remove backklash from the motor gearbox	
	//read the encoder positions and save them
	int32_t re = enc_->read();

	//then position the motors for the next motion command
	for(int i = 0; i < 10; i++)
	{
		setPower((_deadband+i)*dir);
		delay(100 - 10*i);
	}
	//once positioned, restore the encoder values 
	enc_->write(re);
}

void Wheel::stop()
{
	_power = 0;
	setPower(_power);
	speedBuffer.clear();
}

void Wheel::resetEnc(float dist)
{
float counts = _counts_per_unitdist * dist;
	enc_->write(counts);
}

Robot::Robot(float wheelBase, Wheel * lwheel, Wheel * rwheel)
{
	_wheelBase = wheelBase;
	_rwheel = rwheel;
	_lwheel = lwheel;
	_heading = 0.0;
	_timeLastSpeed = millis();
}

void Robot::getOdometer(){ // gets the current odometer reading 

	uint32_t time = millis();
	uint32_t delTime = time - _timeLastSpeed;
	_timeLastSpeed = time;

	float rwdist = _rwheel->get_distance();
	float lwdist = _lwheel->get_distance();

	float delta_dist = (rwdist - lwdist);
	_distance = (rwdist + lwdist)/2;
	_heading = delta_dist /_wheelBase;
	_speed = (_distance - _distanceLast) * 1000 / (float) delTime;
	_angularSpeed = (_heading - _headingLast) * 1000 / (float) delTime;
	_distanceLast = _distance;
	_headingLast = _heading;
}

void Robot::resetEnc(float dist, float heading)
{
	float offset = (_wheelBase * heading/2);
	_lwheel->resetEnc(dist - offset);
	_rwheel->resetEnc(dist + offset);
	_distanceLast = dist;
}

void Robot::stop()
{
	_lwheel->stop();
	_rwheel->stop();
	_speedTarget = 0;
	_angularSpeedTarget = 0;
}

void Robot::twist(){//drive until timeout or new command
	if(_speedTarget != 0.0 || _angularSpeedTarget != 0.0){
		float rwSpeedTarget = _speedTarget + ((_angularSpeedTarget * _wheelBase / 2.0));
		float lwSpeedTarget = _speedTarget - ((_angularSpeedTarget * _wheelBase / 2.0));
		_rwheel->drive(rwSpeedTarget);
		_lwheel->drive(lwSpeedTarget);	
	}
	else
	{
		stop();
		_rwheel->resetEnc(0);
		_lwheel->resetEnc(0);
		_distanceLast = 0.0;
		_headingLast = 0.0;
	}
}
