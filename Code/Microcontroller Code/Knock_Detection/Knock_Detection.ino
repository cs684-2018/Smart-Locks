#include <Wire.h>
#define DFF 2000


int16_t ary[100], knock = 0, difference;
int16_t i=0,i_prev=0,j;
int16_t time_out, knock_array[10], knock_ref_array[10], interval=0,knock_start=0,knock_index=0, unlock = 1,err;

//Setup function will do following tasks:
//1. Setup controller configuration for serial connectivity with PC terminal
//2. Setup controller configuration for I2C Communication with Accelerometer
//3. Setup knocking pattern (2,1,1,2)
void setup()
{  
  //Setup serial link with PC Terminal
  Serial.begin(115200);
  while (!Serial);             // Leonardo: wait for serial monitor
  Serial.println("\n\nIoT Based Smart Lock Interface\n");
  
  //To setup I2C connectivity with accelerometer
  scanPorts();
  
  //Configure LED Pin & D3 pin as an output ports and write High on them
  pinMode(0, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(2, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  digitalWrite(0, HIGH);
  digitalWrite(2, HIGH);
  
  //Define Knocking pattern
  knock_ref_array[0] = 2000;
  knock_ref_array[1] = 1000;
  knock_ref_array[2] = 1000;
  knock_ref_array[3] = 2000;
  knock_ref_array[4] = 0;
  knock_ref_array[5] = 0;
  knock_ref_array[6] = 0;
  knock_ref_array[7] = 0;
  knock_ref_array[8] = 0;
  knock_ref_array[9] = 0;
}

void scanPorts() 
{ 
	//Use port D1 as SCL & D2 and SDA
    Wire.begin(4, 5);
	
	//Send address of accelerometer which is 0x68
    Wire.beginTransmission(0x68);
    Wire.write(0x6B);  // PWR_MGMT_1 register
    Wire.write(0);     // set to zero (wakes up the MPU-6050)
    Wire.endTransmission(true);
    delay(500);
}

int16_t read_data(void)
{
  int16_t Acc;
  
  //Send address of accelerometer which is 0x68
  Wire.beginTransmission(0x68);
  
  //Address of Z axis register
  Wire.write(0x3F);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(0x68,2,true);  // request a total of 2 registers
  
  //Read Two bytes from I2C
  Acc=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  return Acc;
}

//find_diff functioin will return absolute value of A-B
int16_t find_diff(int16_t A, int16_t B)
{
  if(A > B)
    return A-B;
  else if(B > A)
    return B-A;
  else
    return 0;
}

//inc_i function will increase value of i and round it off after 99
void inc_i(void)
{
  //Store i_prev to compare previous sample with current sample for filter implementation
  i_prev = i;
  i++;
  if(i>99)
    i = 0;  
}

int16_t compare_array(void)
{
  int16_t ok=1;
  err = 500; //Define margin of allowed error in knocking. At present error margin is 500 milliseconds
  
  for(j=0;j<10;j++)
  {
	//Check if knock is within expected window of +/- 500ms
    if((knock_array[j] < (knock_ref_array[j] + err)) && (knock_array[j] > (knock_ref_array[j] - err)))
      continue;
    else
    {
      ok = 0;
      Serial.print("Mismatch"); Serial.print(j); //Display poisition of mismatched knock
    }
  }
  return ok;
}

//unlock_door by turning down digital Output Pin
void unlock_door()
{
  digitalWrite(0, LOW);  
}

//Lock_door by turning up digital Output Pin
void lock_door()
{
  digitalWrite(0, HIGH);  
}


void loop() 
{
  //Delay of 10ms between every consecutive accelerometer data sample
  delay(10);
  
  if(knock_start == 1)
  {
    interval += 10; //Add delay to interval between two knocks if knocking event is detected
    time_out  += 10; //Add delay to interval between two knocks if knocking event is detected
  }  
  
  //Buffer to read accelerometer data
  ary[i] = read_data();
  difference = find_diff(ary[i], ary[i_prev]);
  
  //If difference is beyond certain threshold then only start knock detection procedure
  while(difference > DFF)
  {
    delay(50); //Time delayed sample reading to ensure LPF functionality and remove noise
    if(knock_start == 1)
    {
      interval += 50; //Add delay to interval between two knocks if knocking event is detected
      time_out  += 50;//Add delay to interval between two knocks if knocking event is detected
    }
    inc_i();
    ary[i] = read_data();
    difference = find_diff(ary[i], ary[i_prev]);
    if(difference > DFF)
    {
      knock = 1; //Knock is detected
      digitalWrite(2, LOW); //Glow on board LED on every knocking
    }
  }
  if(knock == 1)
  {
    knock = 0;
    Serial.print("Knock = 1"); Serial.print("\n");      //Print on serial debug interface for knocking detection     
    
	//Reset all variables in case knocking is not detected 
	if(knock_start == 0)
    {
      knock_start = 1;
      interval = 0;
      time_out = 0;
      knock_index=0;
    }
    else
    {
      knock_array[knock_index] = interval; //Store interval between successive knocks
      knock_index++;
      interval = 0;
    }
    digitalWrite(2, HIGH);
  }
  if(difference < DFF)
  {
    knock = 0;    
  }

  if(time_out > 10000) //Detect time out and reset all the variables to default values
  {
    time_out = 0;
    interval = 0;
    knock_index = 0;
    knock_start = 0;
    Serial.print("Timeout\n");
    Serial.print("Pattern\n");
    for(j=0;j<10;j++)
    {
      Serial.print(knock_array[j]);Serial.print(" "); //Print pattern on serial debug interface
    }
    Serial.print("\n");
    unlock = compare_array();
    if(unlock == 1)
    {
      Serial.print("\nUnlocked\n");
      unlock_door();
    }
    else
    {
      Serial.print("\nFailed To Unlock\n");
      lock_door();
    }
    for(j=0;j<10;j++)
      knock_array[j] = 0; //Flush array
  }
  
  inc_i();  
}