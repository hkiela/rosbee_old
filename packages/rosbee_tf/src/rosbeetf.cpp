#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Quaternion.h>
#include <rosbee_control/encoders.h>
#include <ros/console.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <tf/transform_listener.h>
#include <sensor_msgs/LaserScan.h>

#define WHEELBASE 			       0.41
#define FULLCIRCLEPULSE		     36.0
#define CIRCUMFERENCEWHEEL     (2.0* M_PI*(0.153/2.0))
#define DISTANCEBASETOSCANNER  0.155,0,0.155
#define DISTANCEBASETOLASER    0.155,0,0.105
#define DISTANCEBASETORWHEEL   0,-0.205,0
#define DISTANCEBASETOLWHEEL   0,0.205,0
#define MARGIN								 0.005


double prevEncR,prevEncL,encR,encL;
geometry_msgs::PoseStamped prevpose;

tf::TransformListener * listener;
ros::Publisher * odom_pub;

float CosPrev =  1;    // previous value for Cos(Theta)
float SinPrev =  0;    // previous value for Sin(Theta)

void publishOdomMessage(geometry_msgs::PoseStamped pose){

	double vx,vy,vth;
	vx=vy=vth=0;

	nav_msgs::Odometry odom;
	odom.header.stamp = ros::Time::now();
	odom.header.frame_id = "odom";
	odom.pose.pose = pose.pose;

	//calculate deltas
	double delx =   pose.pose.position.x -prevpose.pose.position.x ;
	double dely  =  pose.pose.position.y-  prevpose.pose.position.y;
	double delth =  tf::getYaw(pose.pose.orientation) - tf::getYaw(prevpose.pose.orientation);
	ros::Duration delt = ros::Time::now() - prevpose.header.stamp;
	ROS_DEBUG_NAMED("Odometry","delx= %f,dely=%f,delth=%f,delt.nsec=%i,delt.sec=%i",delx,dely,delth,delt.nsec,delt.sec);

	//calculate velocity
	vx = (delx/1000000000.0) * ((float)delt.nsec);
	vy = (dely/1000000000.0) * ((float)delt.nsec);
	vth= (delth/1000000000.0)* ((float)delt.nsec);

	//set the velocity
	odom.child_frame_id = "base_link";
	odom.twist.twist.linear.x = vx;
	odom.twist.twist.linear.y = vy;
	odom.twist.twist.angular.z = vth;


	ROS_DEBUG_NAMED("Odometry","X-velocity= %f Y-velocity= %f TH-velocity= %f",vx,vy,vth);
	odom_pub->publish(odom);
}


void publishTf(const double encL, const double encR, const geometry_msgs::PoseStamped base_pose)
{
	static tf::TransformBroadcaster br;
	tf::Transform transform;

	transform.setOrigin(tf::Vector3(base_pose.pose.position.x,base_pose.pose.position.y,base_pose.pose.position.z));
	transform.setRotation(tf::Quaternion(base_pose.pose.orientation.x ,base_pose.pose.orientation.y,base_pose.pose.orientation.z,base_pose.pose.orientation.w));
	br.sendTransform(tf::StampedTransform(transform, ros::Time::now(),"odom", "base_link"));

	transform.setOrigin( tf::Vector3(DISTANCEBASETOSCANNER));
	transform.setRotation(tf::createQuaternionFromRPY(0,0,0));
	br.sendTransform(tf::StampedTransform(transform, ros::Time::now(),"base_link", "openni_camera"));

	transform.setOrigin( tf::Vector3(DISTANCEBASETOLASER));
	transform.setRotation(tf::createQuaternionFromRPY(0,0,0));
	br.sendTransform(tf::StampedTransform(transform, ros::Time::now(),"base_link", "laser"));

	transform.setOrigin( tf::Vector3(DISTANCEBASETOLWHEEL));
	transform.setRotation(tf::createQuaternionFromRPY(0,encL,0));
	br.sendTransform(tf::StampedTransform(transform, ros::Time::now(),"base_link", "leftWheel"));

	transform.setOrigin( tf::Vector3(DISTANCEBASETORWHEEL));
	transform.setRotation(tf::createQuaternionFromRPY(0,encR,0));
	br.sendTransform(tf::StampedTransform(transform, ros::Time::now(),"base_link", "rightWheel"));

	ROS_DEBUG_NAMED("TF","TF Sended");
}


geometry_msgs::PoseStamped calculatePlatformPose(double l, double r) {
	
	double theta =0;
	double posx=0,posy=0;
	//double CosNow=0,SinNow=0;

	if (fabs(r-l) <= MARGIN) {
		posy = ((l+r)/2) * SinPrev;
		posx =((l+r)/2) * CosPrev;	
		ROS_DEBUG_NAMED("Odometry","===Straight Line===");	
	}
	else {
		double dtheta = (r-l)/WHEELBASE;
		theta =  fmodf((theta+dtheta),(2*M_PI));
		CosPrev=cosf(theta); // for the next cycle
    SinPrev=sinf(theta);
		ROS_DEBUG_NAMED("Odometry","===Pivoting===");	
	}
	/*else{
		double dtheta = (r-l)/WHEELBASE;
		theta =  fmodf((theta+dtheta),(2*M_PI));
		CosNow=cosf(theta);
    SinNow=sinf(theta);
	ROS_DEBUG_NAMED("Odometry","===Cornering===");	

   double Radius=(WHEELBASE/2)*((r+l)/(r-l));
    posx=Radius*(SinNow-SinPrev);
    posy=Radius*(CosPrev-CosNow);
		} */
			//make posestamped message
		geometry_msgs::PoseStamped posestamp;
		posestamp.header.frame_id ="base_link";
		posestamp.header.stamp = ros::Time(0);
		posestamp.pose.position.y = posy;
		posestamp.pose.position.x = posx;
		posestamp.pose.position.z = 0;
		posestamp.pose.orientation = tf::createQuaternionMsgFromYaw(theta); 	
    ROS_DEBUG_NAMED("Odometry"," new Platform pose generated! pose= x:%f y:%f YAW(deg):%f",posx,posy,theta *(180/M_PI));

		return posestamp;
}

geometry_msgs::PoseStamped transformPose(geometry_msgs::PoseStamped pose){

	geometry_msgs::PoseStamped odom_point;
	odom_point.pose = geometry_msgs::Pose();
	odom_point.pose.orientation =tf::createQuaternionMsgFromYaw(2);

	try{
		listener->transformPose("odom", pose, odom_point);
		ROS_DEBUG_NAMED("Odometry","new pos for base  x=%f y=%f th=%f" ,odom_point.pose.position.z, odom_point.pose.position.y,tf::getYaw(odom_point.pose.orientation));

	}
	catch(tf::TransformException& ex){
		ROS_DEBUG_NAMED("Odometry","Received an exception trying to transform a point: %s", ex.what());
	return odom_point;
	}
	
	//save prevpose


	return odom_point;
}

void pubOnce()
{
	static tf::TransformBroadcaster br;
	tf::Transform transform;
	transform.setOrigin(tf::Vector3(0,0,0));
	transform.setRotation(tf::createQuaternionFromYaw(90.0 * (M_PI/180.0)));
	br.sendTransform(tf::StampedTransform(transform, ros::Time::now(),"odom", "base_link"));

}

void enc(const rosbee_control::encoders::ConstPtr& msg)
{
	//set the prevEnc values only the first time, because there is no initial value when
	//entering this method for the first time. assuming the values will never be 0.
	if(prevEncL == 0 && prevEncR == 0)
	{
		prevEncR = msg->rightEncoder;
		prevEncL =  msg->leftEncoder;
    return;
	}

	ROS_DEBUG_NAMED("Odometry","prevEnc r:%f ,PrevEnc l:%f NewEnc l:%i, NewEnc R:%i",prevEncR,prevEncL, msg->leftEncoder,msg->rightEncoder);
	//get the difference between last and current position
	double delr = msg->rightEncoder - prevEncR;
	double dell = msg->leftEncoder -  prevEncL;
	ROS_DEBUG_NAMED("Odometry","delta r:%f pulses,delta l:%f pulses",delr,dell);

	if(delr < -60000 || delr > 60000 || dell < -60000 || dell > 60000)
	{
	
		prevEncR = msg->rightEncoder;
		prevEncL = msg->leftEncoder;
			return;  
	}

	//calculate the distance the wheels have traveled, compared to last measurement.
	double right = (CIRCUMFERENCEWHEEL*delr)/FULLCIRCLEPULSE;
	double left = (CIRCUMFERENCEWHEEL*dell)/FULLCIRCLEPULSE;
	ROS_DEBUG_NAMED("Odometry","New distance: left wheel:%lfm right wheel:%lfm",left,right);

	//calculate the wheelangle from both encoders for tf in radian.(can be optimized).
	encR += ((360.0*delr)/FULLCIRCLEPULSE) *(M_PI/180.0);
	encL += ((360.0*dell)/FULLCIRCLEPULSE) *(M_PI/180.0);
	ROS_DEBUG_NAMED("Odometry","New Wheel Angle:R=%f L=%f (degrees)",(encR *(180/M_PI)),(encL*(180/M_PI)));

	//get new pose platform.
	geometry_msgs::PoseStamped pose = calculatePlatformPose(left,right);

	// translate the platform pose to a pose in the odom frame
	pose = transformPose(pose);

	//publish Transforms
	publishTf(encL,encR,pose);

	//publish Odom message
	publishOdomMessage(pose); //TODO

	//save the encoder values for next call
	prevEncR = msg->rightEncoder;
	prevEncL = msg->leftEncoder;

	prevpose = pose;
 ROS_DEBUG_NAMED("Odometry"," ");
}


	void publishOdom()
	{
	static tf::TransformBroadcaster br;
		geometry_msgs::TransformStamped odom_trans;
    odom_trans.header.stamp = ros::Time::now();
    odom_trans.header.frame_id = "odom";
    odom_trans.child_frame_id = "base_link";

    //send the transform
   br.sendTransform(odom_trans);
	}

int main(int argc, char **argv) {

	//ros initialisation
	ros::init(argc, argv, "tfBroadcaster");
	ros::NodeHandle n;

	publishOdom();

	//init variables
	encR=encL=prevEncL=prevEncR=0;
  prevpose = geometry_msgs::PoseStamped();
	prevpose.pose.position.x = 0;
	prevpose.pose.position.y = 0;
	prevpose.pose.position.z = 0;
	prevpose.pose.orientation = tf::createQuaternionMsgFromYaw(2);

	//subscribe to encoders. every time an encoder value is published over ROS the enc method is called.
	//we need this values to calculate the position/speed of the platform.
	ros::Subscriber subx = n.subscribe("/enc",1,enc);

	//ros Publisher.will be used later when odomerty information is calculated.
	//you need to initiaze this publisher in the main or else it wont work.
	odom_pub = new ros::Publisher();
	*odom_pub = n.advertise<nav_msgs::Odometry>("odom", 1);

	//transform listener. will be used later
	listener = new tf::TransformListener(n);

	//run node.
	ros::spin();

	//delete pointers
	delete listener;
	delete odom_pub;

	return 0;
}
