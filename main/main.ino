// Channel A is left, Channel B is right
const int leftdirection = 12;
const int rightdirection = 13;
const int leftspeed = 3;
const int rightspeed = 11;
const int leftbreak = 9;
const int rightbreak = 8;
const int currentleft = A0;
const int currentright = A1;
const int trigLeft = 4;
const int trigRight = 10;
const int trigBack = 2;
const int echoLeft = 5;
const int echoRight = 6;
const int echoBack = 7;
const int LDRLeft = A2;
const int LDRRight = A3;
const int PotentioSwitcher = A4;
const int max = 100;

int distance, distanceLeft, distanceRight, distanceBack, obstacleinfront, obstacleinback, left, right, movementype,last_state,lefthigh,righthigh,potentioswap=0;
unsigned long currenttime, starttime, backingtimestart, backingtimeend;

void sensorUpdate(int trigPin,int echoPin) {

  int duration;
  digitalWrite(trigPin, LOW);        // Reset sensor
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);       // Turn on sensor       
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);        // Turn off sensor  

  duration = pulseIn(echoPin, HIGH); // Take in the reading from my echo pin

  distance = (duration/2) / 29.1;    // Calculate my distances

}

void moveChasis() {
  switch (movementype) {
    case 0: digitalWrite(leftdirection,LOW); digitalWrite(rightdirection,HIGH); last_state = 0; left=max; right=max; break;  // Forwards
    case 1: digitalWrite(leftdirection,HIGH); digitalWrite(rightdirection,HIGH); left=max; right=0; break;   // Back Right
    case 2: digitalWrite(leftdirection,LOW); digitalWrite(rightdirection,LOW); left=0; right=max; break;     // Back Left
    case 3: digitalWrite(leftdirection,HIGH); digitalWrite(rightdirection,LOW); left=max; right=max; break;  // Back Forwards
    case 4: digitalWrite(leftdirection,LOW); digitalWrite(rightdirection,HIGH); left=0; right=0; break;      // Stop
    case 5: digitalWrite(leftdirection,LOW); digitalWrite(rightdirection,HIGH); last_state = 1; left=max; right=0; break;    // Turn Left
    case 6: digitalWrite(leftdirection,LOW); digitalWrite(rightdirection,HIGH); last_state = 2; left=0; right=max; break;    // Turn Right
  }
}

void IR_sensor_movement() {
  int Left_Side,Right_Side;/*
  Serial.print("Left:");
  Serial.println(analogRead(LDRLeft));
  Serial.print("Right:");
  Serial.println(analogRead(LDRRight)); */
  if (analogRead(LDRLeft) > 0.85*lefthigh) {Left_Side = 0;} else {Left_Side = 1;}
  if (analogRead(LDRRight) > 0.85*righthigh) {Right_Side = 0;} else {Right_Side = 1;}
  Serial.print("Right:");
  Serial.print(Right_Side);
  Serial.print("\n");
  Serial.print("Left: ");
  Serial.print(Left_Side);
  Serial.print("\n");
  if((Right_Side == 0)&&(Left_Side == 0)){//if both white move forward
    movementype = 0;
  }
  if((Right_Side == 1)&&(Left_Side == 0)){//if right is black and left is white turn right
    movementype = 6;
  }
  if((Right_Side == 0)&&(Left_Side == 1)){// if right is white and left is black turn left
    movementype = 5;
  } 
  if((Right_Side == 1)&&(Left_Side == 1)){// if both are black then stop
    movementype = 4;
  }
  Serial.println(movementype);
}

void checkObstacle() {
  int dist = 15;
  if (distanceBack > dist) {
    obstacleinback=0;
    } else {
    obstacleinback=1;
    }
  if (distanceLeft < dist || distanceRight < dist) {
    obstacleinfront=1;
    } else {
      obstacleinfront=0;
    }
  if (obstacleinfront==1) {
    if (obstacleinback==0) {
      if (distanceLeft <= dist && distanceRight <= dist) {
        movementype = 3;
      }
      else if (distanceRight <= dist) {
        movementype = 1;
      } 
      else {
        movementype = 2;
      }
    } 
    else {
      movementype = 4;
    } 
  } 
  else {
    IR_sensor_movement();
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(leftdirection,OUTPUT);
  pinMode(rightdirection,OUTPUT);
  pinMode(leftspeed,OUTPUT);
  pinMode(rightspeed,OUTPUT);
  pinMode(leftbreak,OUTPUT);
  pinMode(rightbreak,OUTPUT);
  pinMode(trigLeft,OUTPUT);
  pinMode(trigRight,OUTPUT);
  pinMode(trigBack,OUTPUT);
  pinMode(echoLeft,INPUT);
  pinMode(echoRight,INPUT);
  pinMode(echoBack,INPUT);
  analogRead(LDRLeft);
  analogRead(LDRRight);
  analogRead(PotentioSwitcher);
  digitalWrite(leftbreak,LOW);
  digitalWrite(rightbreak,LOW);
  lefthigh = analogRead(LDRLeft);
  righthigh = analogRead(LDRRight);
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorUpdate(trigBack,echoBack);
  distanceBack = distance;
  sensorUpdate(trigLeft,echoLeft);
  distanceLeft = distance;
  sensorUpdate(trigRight,echoRight);
  distanceRight = distance;
  if (analogRead(PotentioSwitcher)>500) {
    if (potentioswap != 0) {
      starttime = millis();
      potentioswap = 1;
    }
  }
  currenttime=millis();
  if (analogRead(PotentioSwitcher)>500 && (currenttime-starttime)<174000) { //(2.9 Minute Set Time)
    /*
    Serial.print("\nLeft: ");
    Serial.println(distanceLeft);
    Serial.print("Right: ");
    Serial.print(distanceRight);
    Serial.print("\nBack: ");
    Serial.print(distanceBack);
    */
    checkObstacle();
    moveChasis();

    //IR_sensor_movement();
    // This is where we put our line following
    analogWrite(leftspeed,left);
    analogWrite(rightspeed,right);
    delay(1); //Stability delay
  } 
  else {
    movementype = 4;
  }
}
