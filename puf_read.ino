const byte modePin=2;
const byte requestPin=4;
const byte getReadyPin=3;
const byte getInterruptPin=5;
const byte dataPin[8]={6,7,8,9,10,11,12,13};
int val,val2,val3;
int HEXval1=0,HEXval2=0,times,timeshake;
int data[8];
int num;


// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  pinMode(getReadyPin, INPUT);
  pinMode(getInterruptPin, INPUT);
  for(int x=0;x<8;x++){
      pinMode(dataPin[x], INPUT);
  }
  pinMode(requestPin,OUTPUT);
  pinMode(modePin,OUTPUT);
  digitalWrite(modePin, HIGH);
  attachInterrupt(digitalPinToInterrupt(getInterruptPin),getdata,RISING);
  //Scheduler.startLoop(loop1);
}

// the loop function runs over and over again forever
void loop() {
  val = digitalRead(getReadyPin);
  val3 = digitalRead(modePin);
  if(val3==1){
    num=12500001;//125000
  }
  else{
    num=512;
  }
  if(val==1&&timeshake<num){
    digitalWrite(requestPin, HIGH);
    delay(8);
    digitalWrite(requestPin, LOW);
    timeshake++;
  }
  else if(val==0){
  }

}
void getdata(){
  for (int x = 0; x < 8; x++) {
    data[x]=digitalRead(dataPin[x]);
  }
  HEXchange1();
  HEXchange2();
  if(timeshake<num){
  Serial.print(HEXval2,HEX);
  Serial.print(HEXval1,HEX);
  }
  HEXval1=0;
  HEXval2=0;
  times++;
  if(times==4&&timeshake<num){
    Serial.println();
    times=0;
  }
}
void HEXchange1(){
  for (int x = 0; x < 4; x++) {
    HEXval1=HEXval1+data[x]*pow(2,x);
  } 
}
void HEXchange2(){
  for (int x = 0; x < 4; x++) {
    HEXval2=HEXval2+data[x+4]*pow(2,x);
  } 
}
