#include "WPILib.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */

class RobotDemo: public SimpleRobot {
	RobotDrive* myRobot; // robot drive system
	//Spikes to compress and fire
	Relay* compressorSpike1;
	Relay* compressorSpike2;
	Relay* fireSpike;
	Joystick* GamePad;
	AnalogChannel* PressureSensor;
	KinectStick* Rarm;
	KinectStick* Larm;

	Jaguar* jaguar;
	int intPressure;
	float voltPressure;
	float RightArmX;
	float RightArmY;
	float motorRotation;

public:
	RobotDemo(void) {

		myRobot = new RobotDrive(1, 2, 3, 4); // these must be initialized in the same order

		GamePad = new Joystick(1);

		//Spike that controls pressure compressors
		compressorSpike1 = new Relay(5);
		compressorSpike2 = new Relay(4);
		
		//Spike that controls firing
		fireSpike = new Relay(1);
		PressureSensor = new AnalogChannel(2);

		Rarm = new KinectStick(1);
		Larm = new KinectStick(2);

		jaguar = new Jaguar(2);

		motorRotation = 0;

		myRobot->SetExpiration(0.1);
		myRobot->SetInvertedMotor(myRobot->kRearLeftMotor, true);

	}

	//{
	//myRobot->SetExpiration(0.1);

	//}

	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	void Autonomous() {
		myRobot->SetSafetyEnabled(false);
		myRobot->Drive(-0.5, 0.0); // drive forwards half speed
		Wait(2.0); //    for 2 seconds
		myRobot->Drive(0.0, 0.0); // stop robot
	}

	void communications() {

	}

	/**
	 * Runs the motors with arcade steering. 
	 */
	void OperatorControl() {
		bool printed = false;
		int timer = 0;
		int checkTimer = 0;
		myRobot->SetSafetyEnabled(true);
		//Pressures to flag at
		int checkPressures[] = {10,20,30,40,50,60}; 
		int length = 6;
		bool flagged[] = {false, false, false, false, false, false};
		while (IsOperatorControl()) {
			RightArmX = Rarm->GetX(Rarm->kRightHand);
			RightArmY = Rarm->GetY(Rarm->kRightHand);

			/**
			 if(RightArmX != 0){
			 printf("%s%f", "Right Arm X: ", RightArmX);
			 printf("\n");
			 }
			 
			 if(RightArmY != 0){
			 printf("%s%f", "Right Arm Y: ", RightArmY);
			 printf("\n");
			 }
			 **/

			myRobot->TankDrive(GamePad->GetRawAxis(4), GamePad->GetRawAxis(2));
			//drive with tank style (use right stick)

			/**KINECT**
			 myRobot->TankDrive(Rarm,Larm);
			 
			 **/
			
			if (checkTimer >= 100){
				for (int i=0; i<length; i++){
					if (getPressure() == checkPressures[i] && !flagged[i]){
						printf("FLAG: Pressure at %i! PSI\n", checkPressures[i]);
						checkTimer = 0;
						flagged[i] = true;
						if (i != 0){
							flagged[i-1] = false;
						}
					}
				}
			}
			checkTimer++;
			
			Wait(0.005); // wait for a motor update time
			
			/**
			//Gets air compressor
			if (GamePad->GetRawButton(?) == true) {
				relay->Set(relay->kForward);
			} else {
				relay->Set(relay->kOff);
			}
			**/

			//Only fires if there is more than 20PSI in the tank.
			if (GamePad->GetRawButton(8) == true) {
				if (getPressure()>=20){
					//FIRE!
					if(!printed){
						printf("%s", "FIRE!\n");
						printed = true;
					}
					fireSpike->Set(fireSpike->kForward);
				} else {
					if(!printed){
						printf("%s", "Not enough pressure.\n");
						printed = true;
					}
				}
			} else {
				fireSpike->Set(fireSpike->kOff);
				printed = false;
			}

			//Pump!
			if (GamePad->GetRawButton(7) == true) {
				compressorSpike1->Set(compressorSpike1->kForward);
				compressorSpike2->Set(compressorSpike2->kForward);
			} else {
				compressorSpike1->Set(compressorSpike1->kOff);
				compressorSpike2->Set(compressorSpike2->kOff);
			}
			
			if (GamePad->GetRawButton(6) == true) {
				motorRotation += 0.01f;
				jaguar->Set(motorRotation);
			}
			if (GamePad->GetRawButton(5) == true && timer >= 100) {
				printf("%s%i%s", "Pressure: ", getPressure(), " (PSI)\n");
				timer = 0;
			} else {
				jaguar->Set(0.0f);
			}
			timer++;
		}
	}

	/**
	 * Runs during test mode
	 */
	void Test() {

	}

	int getPressure() {
		intPressure = PressureSensor->GetValue();
		voltPressure = PressureSensor->GetVoltage();
		int pressure = int((0.1706*intPressure-14.984+33.058*voltPressure-15.406)/2);
		if (pressure == 1 || pressure < 0) {
			pressure = 0;
		}
		return pressure;
	}
};
START_ROBOT_CLASS(RobotDemo)
;

