#include <Robot5.h>
#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/TwistStamped.h>

geometry_msgs::Twist cmd_vel_msg;
geometry_msgs::TwistStamped act_vel_msg;

Sabertooth ST(128,Serial2);
Encoder rwEnc(18,19);
Encoder lwEnc(2,3);

Wheel rwheel(2, 175, &ST, &rwEnc);
Wheel lwheel(1, 175, &ST, &lwEnc);
Robot Bones(419, &lwheel, &rwheel);

bool commandTimeout = false;

void twist_cb(const geometry_msgs::Twist &msg);
void error_blink(int d1, int d2); // triggers LED error indicators
void test();

ros::NodeHandle nh;
uint32_t lastfeedback;
uint32_t lastCommandReceived;

ros::Publisher act_vel_pub("act_vel",&act_vel_msg);
ros::Subscriber<geometry_msgs::Twist> cmd_vel_sub("cmd_vel",twist_cb);

bool OK2run()
{
	return (!commandTimeout);
}

void setup()
{
	pinMode(11,OUTPUT);	// error status led
	pinMode(12,OUTPUT);	// error status led

	digitalWrite(11,HIGH);
	digitalWrite(12, LOW);

	nh.initNode();
	nh.advertise(act_vel_pub);
	nh.subscribe(cmd_vel_sub);

	Serial2.begin(9600); //Serial2 communication to Sabertooth
	rwheel.stop();
	lwheel.stop();

  	//initialize parameters
	rwheel.findDeadBand();
	int d1 = rwheel._deadband/10;
	int d2 = rwheel._deadband % 10;
	error_blink(d1,d2);

	lwheel.findDeadBand();
	d1 = lwheel._deadband/10;
	d2 = lwheel._deadband % 10;	
	error_blink(d1,d2);

	Bones.stop();

	lastfeedback = millis();
	lastCommandReceived = millis();

	rwheel.resetEnc(0);
	lwheel.resetEnc(0);

	nh.spinOnce();
}

void loop()
{
	nh.spinOnce();

	if(millis() - lastfeedback > 200)
	{
		lastfeedback = millis();
		Bones.getOdometer();
		act_vel_msg.header.stamp = nh.now();
		act_vel_msg.twist.linear.x = Bones._speed/1000;
		act_vel_msg.twist.angular.z = Bones._angularSpeed;
		act_vel_pub.publish(&act_vel_msg);		
		nh.spinOnce();
		//take advantage that we just checked millis() to check interval since last command
		if(lastfeedback - lastCommandReceived > 25000) commandTimeout = true;
	}

	if(OK2run())
	{
		digitalWrite(11,LOW);
		digitalWrite(12,HIGH);
		Bones.twist();
	}
	else
	{	// if not OK2run
		Bones.stop();
		digitalWrite(11,HIGH);
		digitalWrite(12,LOW);
		error_blink(1,1);
	}
}


void error_blink(int d1, int d2)
{
	for (int i = 0; i < 2; i++)
	{
		digitalWrite(11, LOW);
		for(int j = 0; j < d1; j++)
		{
			digitalWrite(12,HIGH);
			delay(100);
			digitalWrite(12,LOW);
			delay(100);
		}
		digitalWrite(12, LOW);
		for(int j = 0; j < d2; j++)
		{
			digitalWrite(11,HIGH);
			delay(100);
			digitalWrite(11,LOW);
			delay(100);
		}		
	}
}

void twist_cb(const geometry_msgs::Twist & msg)
{
	Bones._speedTarget = msg.linear.x * 1000;
	Bones._angularSpeedTarget = msg.angular.z;
	lastCommandReceived = millis();
	commandTimeout = false;
}
