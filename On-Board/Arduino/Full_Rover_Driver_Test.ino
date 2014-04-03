#define FL_ENABLE 30
#define ML_ENABLE 26
#define RL_ENABLE 22
#define FR_ENABLE 38
#define MR_ENABLE 42
#define RR_ENABLE 34

#define FL_PWM 4
#define ML_PWM 3
#define RL_PWM 2
#define FR_PWM 6
#define MR_PWM 7
#define RR_PWM 5

#define FL_DIG 31
#define ML_DIG 27
#define RL_DIG 23
#define FR_DIG 39
#define MR_DIG 43
#define RR_DIG 35

char inp;

void setup() {
  Serial.begin(9600);
  
  pinMode(FL_ENABLE, OUTPUT);
  pinMode(ML_ENABLE, OUTPUT);
  pinMode(RL_ENABLE, OUTPUT);
  pinMode(FR_ENABLE, OUTPUT);
  pinMode(MR_ENABLE, OUTPUT);
  pinMode(RR_ENABLE, OUTPUT);
  
  pinMode(FL_PWM, OUTPUT);
  pinMode(ML_PWM, OUTPUT);
  pinMode(RL_PWM, OUTPUT);
  pinMode(FR_PWM, OUTPUT);
  pinMode(MR_PWM, OUTPUT);
  pinMode(RR_PWM, OUTPUT);
  
  pinMode(FL_DIG, OUTPUT);
  pinMode(ML_DIG, OUTPUT);
  pinMode(RL_DIG, OUTPUT);
  pinMode(FR_DIG, OUTPUT);
  pinMode(MR_DIG, OUTPUT);
  pinMode(RR_DIG, OUTPUT);
  
  digitalWrite(FL_ENABLE, HIGH);
  digitalWrite(ML_ENABLE, HIGH);
  digitalWrite(RL_ENABLE, HIGH);
  digitalWrite(FR_ENABLE, HIGH);
  digitalWrite(MR_ENABLE, HIGH);
  digitalWrite(RR_ENABLE, HIGH);
}

void loop() {
  if(Serial.available()) {
    inp=Serial.read();
    if(inp=='f') { // Forward
      digitalWrite(FL_ENABLE, HIGH);
      digitalWrite(ML_ENABLE, HIGH);
      digitalWrite(RL_ENABLE, HIGH);
      digitalWrite(FR_ENABLE, HIGH);
      digitalWrite(MR_ENABLE, HIGH);
      digitalWrite(RR_ENABLE, HIGH);
      
      digitalWrite(FL_DIG, HIGH);
      analogWrite(FL_PWM, 0);
      
      digitalWrite(ML_DIG, HIGH);
      analogWrite(ML_PWM, 0);
      
      digitalWrite(RL_DIG, HIGH);
      analogWrite(RL_PWM, 0);
      
      digitalWrite(FR_DIG, HIGH);
      analogWrite(FR_PWM, 0);
      
      digitalWrite(MR_DIG, HIGH);
      analogWrite(MR_PWM, 0);
      
      digitalWrite(RR_DIG, HIGH);
      analogWrite(RR_PWM, 0);
    }
    else if(inp=='b') { // Backward
      digitalWrite(FL_ENABLE, HIGH);
      digitalWrite(ML_ENABLE, HIGH);
      digitalWrite(RL_ENABLE, HIGH);
      digitalWrite(FR_ENABLE, HIGH);
      digitalWrite(MR_ENABLE, HIGH);
      digitalWrite(RR_ENABLE, HIGH);
      
      digitalWrite(FL_DIG, LOW);
      analogWrite(FL_PWM, 255);
      
      digitalWrite(ML_DIG, LOW);
      analogWrite(ML_PWM, 255);
      
      digitalWrite(RL_DIG, LOW);
      analogWrite(RL_PWM, 255);
      
      digitalWrite(FR_DIG, LOW);
      analogWrite(FL_PWM, 255);
      
      digitalWrite(MR_DIG, LOW);
      analogWrite(MR_PWM, 255);
      
      digitalWrite(RR_DIG, LOW);
      analogWrite(RR_PWM, 255);
    }
    else if(inp=='s') { // Stop the rover
      // PWM and direction pins
      digitalWrite(FL_ENABLE, LOW);
      digitalWrite(ML_ENABLE, LOW);
      digitalWrite(RL_ENABLE, LOW);
      digitalWrite(FR_ENABLE, LOW);
      digitalWrite(MR_ENABLE, LOW);
      digitalWrite(RR_ENABLE, LOW);
    }
    else { // Randomness
    }
  }
}
