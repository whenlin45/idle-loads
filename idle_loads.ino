
int Vo;
int analogPin= 0;
int raw= 0;
int Vin= 5;
float Vout= 0;
float R1= 1000;
float R2= 0;
float buffer= 0;

float outputCurrent;
float inputResistance;
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //Start serial communication

  pinMode(7, OUTPUT); // pin 6 as pwm output for photoresistor
}

void loop() {
  // put your main code here, to run repeatedly:

  raw= analogRead(analogPin);
  if(raw) 
  { 
  buffer= raw * Vin;
  Vout= (buffer)/1024.0;
  buffer= (Vin/Vout) -1;
  R2= R1 * buffer;
  Serial.print("Vout: ");
  Serial.println(Vout);
  Serial.print("R2: ");
  Serial.println(R2);

  if(R2 < 60000){
    digitalWrite(7, LOW);
    Serial.print("Setting current to low");
  } else {
    digitalWrite(7, HIGH);
    Serial.print("Setting current to high");
  }
  
  delay(1000);
//
  }
}
