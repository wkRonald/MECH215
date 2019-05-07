//Name: Wai-Kwok Ronald See 
//Stu_ID: 40094605

// ------ARDUINO PROJECT 01 A.R.H.M.-------
// --------------------------------------
#include <Servo.h>

Servo servoX; 
Servo servoY1;
//Servo servoY2;

int Magnet = 3;

int trig = 5;
int echo = 6;
float distance = 0.0;
float duration = 0.0;

int thetaX = 5;

float Use_USSensor();
void Object_found();
void Put_Trash();
void GO_Back(int &thetaX);

void setup() {
  Serial.begin(9600);

  // Attaching servos to pins
  servoX.attach(10); // arbitrary digital pin between 0 to 13 
  servoY1.attach(11);  // servo RHS
  //servoY2.attach(12); // servo LHS

  // Setting the Arm to its initial position
  servoX.write(5);
  delay(1000); // needed delay for motor (at least 0.1 sec)
  
  servoY1.write(100);
  //servoY2.write(100);
  delay(1000);

  //ULTRASONIC SENSOR PIN SET-UP
  
  pinMode(trig,OUTPUT); // dig pin 5
  pinMode(echo,INPUT); // dig pin 6

  //Magnet Set-up 
  pinMode(Magnet,OUTPUT); //dig pin 3
  
  //---Set-Up Complete----
}

void loop() {
//----------------ServoX control-----------------
  int kill =0; // variable to keep track of the number of objects found
  for (thetaX = 5; thetaX <=180; thetaX+= 20) {
    servoX.write(thetaX);
    delay(1000);
     
//-------------Ultra-sound sensor--------
   
    distance = Use_USSensor();  
    
    if(thetaX >=165 && kill ==0) {
      thetaX = 5 ;
      servoX.write(thetaX);
      delay(1000);
      exit(1);
      delay(1000);
      while(1); // infinite loop that does nothing if exit doesn't work (alternative to exit) 
    }
    
    if(distance < 6.0) { //Object found therefore Activates the movement sequence
      Object_found(); 
      delay(2000);
      Put_Trash();// Put the object in the "trash can"
      delay(3000);
      GO_Back(thetaX); //Brings the ARHM to former position
      delay(3000);
      kill++;
      Serial.print("Killed = ");
      Serial.println(kill);
    } 
  }
}


//-----------FUNction List -----------

float Use_USSensor() { // Function to use the US sensor
  digitalWrite(trig,LOW);
  delay(5);
  digitalWrite(trig,HIGH);
  delay(10);
  digitalWrite(trig,LOW);
  duration = pulseIn(echo,HIGH);
  delay(500);
  distance = (duration/2)* 0.0343 ; //distance is in cm
  Serial.println(duration);
  Serial.print("The distance is ");
  Serial.print(distance);
  Serial.println(" cm");

  return distance;
}

void Object_found() {  // function when an object is in range
  servoY1.write(30); //bend the arm down
  delay(500);
  digitalWrite(Magnet,LOW);// grab the object
  delay(100);
  digitalWrite(Magnet,HIGH);
  delay(2000);
}

void Put_Trash() { ///Function sequence to bring object in trash
  servoY1.write(70); //put some angle not 0
  delay(1000);
  servoX.write(175); //Angle where the center of the trash can is
  delay(2000);
  digitalWrite(Magnet, LOW); //releases the object into the trash
  delay(1000);
}

void GO_Back(int &thetaX) {  //Function to reset the Arm back to the former angle 
    servoY1.write(100);
    //servoY2.write(90);
    delay(300); 
    servoX.write(thetaX);
    delay(500);
}
