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
const int max = 255;

int distance, distanceLeft, distanceRight, distanceBack, obstacleinfront, obstacleinback, left, right, movementype;
// For movementype, 0 = move forwards, 1 = back left, 2 = back right, 3 = move backwards, 4 = stop, 5 = move left, 6 = move right

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
    case 0: digitalWrite(leftdirection,HIGH); digitalWrite(rightdirection,HIGH); left=max; right=max; break;
    case 1: digitalWrite(leftdirection,LOW); digitalWrite(rightdirection,HIGH); left=max/2; right=0; break;
    case 2: digitalWrite(leftdirection,HIGH); digitalWrite(rightdirection,LOW); left=0; right=max/2; break;
    case 3: digitalWrite(leftdirection,HIGH); digitalWrite(rightdirection,HIGH); left=max/2; right=max/2; break;
    case 4: digitalWrite(leftdirection,HIGH); digitalWrite(rightdirection,HIGH); left=0; right=0; break;
  }
}

void checkObstacle() {
  if (distanceBack > 5) {
    obstacleinback=0;
    } else {
    obstacleinback=1;
    }
  if (distanceLeft > 5 && distanceRight > 5) {
    obstacleinfront=0;
    } else {
      obstacleinfront=1;
    }
  if (obstacleinfront==1) {
    if (obstacleinback=0) {
      if (distanceLeft <= 5) {
        movementype = 1;
      } else if (distanceRight <= 5) {
        movementype = 2;
      } else {
        movementype = 3;
      }
    } else {
      movementype = 4;
    } 
  } else {
    movementype = 0;
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
  digitalWrite(leftbreak,LOW);
  digitalWrite(rightbreak,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorUpdate(trigBack,echoBack);
  distanceBack = distance;
  sensorUpdate(trigLeft,echoLeft);
  distanceLeft = distance;
  sensorUpdate(trigRight,echoRight);
  distanceRight = distance;
  Serial.print("\nLeft: ");
  Serial.println(distanceLeft);
  Serial.print("Right: ");
  Serial.print(distanceRight);
  Serial.print("\nBack: ");
  Serial.print(distanceBack);
  checkObstacle();
  moveChasis();
  // This is where we put our line following
  analogWrite(leftspeed,left);
  analogWrite(rightspeed,right);
}
