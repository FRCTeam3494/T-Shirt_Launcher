#include "WPILib.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 

class RobotDemo : public SimpleRobot
{
	RobotDrive* myRobot; // robot drive system
	Relay* relay;
	Joystick* GamePad;
	
	Relay* Spike2;
	AnalogChannel* Pressure;
	int IntPressure;
	float voltPressure;
	

public:
	RobotDemo(void)
{
		myRobot = new RobotDrive(1,2,3,4);	// these must be initialized in the same order
		
		GamePad = new Joystick(1);
		
		relay = new Relay(5);
		Spike2 = new Relay(1);
		Pressure = new AnalogChannel(2);

		
		myRobot->SetExpiration(0.1);
		myRobot->SetInvertedMotor(myRobot->kRearLeftMotor,true);
		
		
		
}
	
	//{
		//myRobot->SetExpiration(0.1);
		
	//}

	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	void Autonomous()
	{
		myRobot->SetSafetyEnabled(false);
		myRobot->Drive(-0.5, 0.0); 	// drive forwards half speed
		Wait(2.0); 				//    for 2 seconds
		myRobot->Drive(0.0, 0.0); 	// stop robot
	}
	
	void communications()
	{
		
		
	}

	/**
	 * Runs the motors with arcade steering. 
	 */
	void OperatorControl()
	{
		
		int timer  = 0;
		myRobot->SetSafetyEnabled(true);
		while (IsOperatorControl())
		{
			IntPressure = Pressure->GetValue();
			voltPressure = Pressure->GetVoltage();
			
			timer++;
			if (timer == 100){
				printf("%f", voltPressure);
				printf("\n");
				printf("%d", IntPressure);
				printf("\n");
				timer = 0;
			}
		
			myRobot->TankDrive(GamePad->GetRawAxis
					(4),GamePad->GetRawAxis(2)); // drive with tank style (use right stick)
			Wait(0.005);				// wait for a motor update time
			//Gets air compressor
			if(GamePad->GetRawButton(8) == true)
			{
				relay->Set(relay->kForward);
			}
			else
			{
				relay->Set(relay->kOff);
			}
			
			if(GamePad->GetRawButton(1) == true)
			{
				Spike2->Set(Spike2->kForward);
			}
			else
			{
				Spike2->Set(Spike2->kOff);
			}
			
			if (GamePad->GetRawButton(4) == true)
			{
					relay->Set(relay->kForward);
			}
				else
				{
					relay->Set(relay->kOff);
				}
				
			}
		}
	
	
	/**
	 * Runs during test mode
	 */
	void Test() {

	}

};
START_ROBOT_CLASS(RobotDemo);

