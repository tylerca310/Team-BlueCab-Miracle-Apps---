#define build_id "new5_STR_240929"

#include <ArduinoBLE.h>

BLEDevice Blunami;
BLEDevice Blucab_Blunami[6];
String Blunami_UUID = "f688fd00-da9a-2e8f-d003-ebcdcc259af9";

int max_scans = 100;
int in=0;
int max_Blunamis = 2; // max devices
int nBlunamis = 0;
int k;  // Lap Counter

struct Blunami_matrix
{ // open struct
int Blunami_index;
  String Blunami_address;  // peripheral device address
  String Blunami_device_name;
  String Blunami_local_name;
} ; // close struct

Blunami_matrix Blunamis[4]; // Blunamis[] is a struc array (matrix) of int Blunami_index, 
                            //  String Blunami_address, String Blunami_local-name and String Blunami_local_name

// define millis() parameters ---
unsigned long command_delay = 30;
unsigned long connect_duration_delay = 1000;
unsigned long poll_delay=320;
unsigned long key_delay=25;
unsigned long max_delay=180000;   //   3 min
unsigned long startmillis;
unsigned long Blucab_Blunami1_connect_millis;
unsigned long Blucab_Blunami2_connect_millis;
unsigned long Blucab_Blunami1_disconnect_millis;
unsigned long Blucab_Blunami2_disconnect_millis;
unsigned long Blucab_Blunami1_connect_duration_millis;
unsigned long Blucab_Blunami2_connect_duration_millis;

// Billy's variables --------------------------------------------------------------------------------------------------------------------------------------------------------
int attribute_delay = 30;       //  used during identifying attributes (start 20)
bool Blunami_all_attributes;
bool Blunami_all_services;
BLECharacteristic BlunamiCommand; //  defines main commands
BLECharacteristic motorvoltage;   //  defines motor voltage

//****************************************Top setup()***********************************************
void setup()
{ // open setup void

// Start Serial/UART
Serial.begin(115200);
while (!Serial);

// Landmark "Build" on Serial Monitor-------------------
Serial.println(build_id);

// start BLE
if (!BLE.begin()) { // open if !BLE
Serial.println("starting Bluetooth® Low Energy module failed!");
while (1);
} // Close if !BLE

BLE.poll(poll_delay);   //  not sure what this does but seems to connect faster

// Pause for latency
startmillis = millis(); while (millis() <= (startmillis + command_delay)) {}  //  latency pause

Serial.println();
Serial.println("\t\tSurveying...");
Blunami_survey();   //  line 250    <-------------search for blunamis

//Serial.println();
//Serial.println("\t\tSurvey Done");

// Pause for latency
startmillis = millis(); while (millis() <= (startmillis + command_delay)) {}  //  latency pause

//Serial.println();
//Serial.println("Memorializing Blunamis[] (a struc array (matrix) of int Blunami_index, String Blunami_address, String Blunami_local-name and String Blunami_local_name");
Blunami_matrix_serial_output(); //  <--------------display blnami search line 302

//Serial.println();
//Serial.println("Completed Blunami_matrix_serial_output()");

// Pause for latency
startmillis = millis(); while (millis() <= (startmillis + command_delay)) {}  //  latency pause

Serial.println();
Serial.println("\t\tConnecting");
Blucab_Blunami_connect();   //  <---------------CONNECT LINE 188

//Serial.println();
//Serial.println("Completed Blucab_Blunami_connect()");

// Pause for latency
startmillis = millis(); while (millis() <= (startmillis + command_delay)) {}  //  latency pause

Serial.println();
Serial.println("\t\tDiscovering attributes");
mrblue_attributes();    //  <-------------print out attributes, set up characteristics       (line 321)

//Serial.println("Completed mrblue_attributes()");

// Pause for latency
startmillis = millis(); while (millis() <= (startmillis + command_delay)) {}  //  latency pause

Serial.println();
Serial.println("Testing (and tickeling) connections");
setup_connect_status_label1: connected_status("setup_connect_status_label1:");

// Pause for latency
startmillis = millis(); while (millis() <= (startmillis + command_delay)) {}  //  latency pause

Serial.println();
Serial.println("Testing connections durations");
Blucab_Blunami1_connect_millis = millis();
Blucab_Blunami2_connect_millis = millis();

Serial.println();
Serial.println("exiting setup");
Serial.println();

}  //close setup void
//***********************************Bottom setup()*******************************************************

//***********************************Top loop()*****************************************************
void loop()
{ // open loop void

if (Blucab_Blunami[1].connected())
{ // open if (Blucab_Blunami[1].connected())
Blucab_Blunami1_disconnect_millis = millis();
} // close if (Blucab_Blunami[1].connected())

if (Blucab_Blunami[2].connected())
{ // open if (Blucab_Blunami[2].connected())
Blucab_Blunami2_disconnect_millis = millis();
} // close if (Blucab_Blunami[2].connected())

k++;
Serial.print("Lap = ");
Serial.println(k);
Serial.print("\t");
connected_status("@146");

// Pause for latency
startmillis = millis(); while (millis() <= (startmillis + connect_duration_delay)) {}  //  connect_duration_delay pause

if ((!Blucab_Blunami[1].connected()) && (!Blucab_Blunami[2].connected()))
{ // open if ((!Blucab_Blunami[1].connected()) && (!Blucab_Blunami[2].connected()))

connect_history();

} // close if ((!Blucab_Blunami[1].connected()) && (!Blucab_Blunami[2].connected()))


} // close loop void
//***************************************Bottom loop()****************************************************


//*************************** TOP BLUNAMI SURVEY****************************************************
void Blunami_survey()
{ // open Blunami_survey

Serial.println();
Serial.println("Entering Blunami_survey()");
Serial.println();

int j;
int i;
int in;

// max scan for Blunamis
for (j = 1; j <= max_scans; j++)
{// open for j<= max_scans
  in = 0;
//  Serial.print("  Blunami_survey() lap ");
//  Serial.println(j);

  BLE.scanForUuid(Blunami_UUID);

  Blunami = BLE.available();

  if(Blunami.address() != "00:00:00:00:00:00") // if good address
  { // open if good address

    for (i=0; i<=(nBlunamis); i++)
    {  //open for i
      if (Blunami.address() == Blunamis[i-1].Blunami_address)
      {  // open if address
        in = 1;  // already in matrix
      //  Serial.print(Blunamis[i-1].Blunami_address);
      //  Serial.print(" Is already in the Blunami matrix");
      }  // close if address
    }  //close for i

    if (in==0)
    {  //open if in == 0 --> not in
    //  Serial.print(Blunami.address());
    //  Serial.print(" is not in the Blunami matrix");

      nBlunamis = nBlunamis + 1; // increment the Blunami index
      // Add into the matrix
      Blunamis[nBlunamis - 1].Blunami_index = nBlunamis;
      Blunamis[nBlunamis - 1].Blunami_address = Blunami.address();
      Blunamis[nBlunamis - 1].Blunami_device_name = Blunami.deviceName();
      Blunamis[nBlunamis - 1].Blunami_local_name = Blunami.localName();
    } // close if in
  } // close if good address
} // for j<= max_scans
while(j < max_scans); // repeat max-scans times

Serial.println();
Serial.println("Exiting Blunami_survey()");
Serial.println();

} // close Blunami_survey
//************************************BOTTOM BLUNAMI SURVEY*********************************************************


//******************************************top connect_history()**********************************
void connect_history()
{ // open void connect_history

Serial.println();
Serial.print("Entering connect_history()");
Serial.println();

Serial.println();
Serial.print("Blucab_Blunami1_disconnect_millis= ");
Serial.print(Blucab_Blunami1_disconnect_millis);
Serial.print(" Blucab_Blunami1_connect_millis= ");
Serial.println(Blucab_Blunami1_connect_millis);
Serial.print(" Blucab_Blunami2_disconnect_millis= ");
Serial.print(Blucab_Blunami2_disconnect_millis);
Serial.print(" Blucab_Blunami2_connect_millis= ");
Serial.println(Blucab_Blunami2_connect_millis);
Serial.print("Blucab_Blunami[1] stayed connected for ");
Blucab_Blunami1_connect_duration_millis = Blucab_Blunami1_disconnect_millis - Blucab_Blunami1_connect_millis;
float Blucab_Blunami1_connect_duration_millis_float = float(Blucab_Blunami1_connect_duration_millis);
float Blucab_Blunami1_connect_duration_minutes = Blucab_Blunami1_connect_duration_millis_float/60000;
Serial.print(Blucab_Blunami1_connect_duration_minutes);
Serial.print(" Minutes");

Serial.println();
Serial.print("Blucab_Blunami[2] stayed connected for ");
Blucab_Blunami2_connect_duration_millis = Blucab_Blunami2_disconnect_millis - Blucab_Blunami2_connect_millis;
float Blucab_Blunami2_connect_duration_millis_float = (float(Blucab_Blunami2_connect_duration_millis));
float Blucab_Blunami2_connect_duration_minutes = Blucab_Blunami2_connect_duration_millis_float/60000;
Serial.print(Blucab_Blunami2_connect_duration_minutes);
Serial.print(" Minutes");
Serial.println();

// Pause for latency
startmillis = millis(); while (millis() <= (startmillis + connect_duration_delay)) {}  //  connect_duration_delay pause

Serial.println();
Serial.print("Redirecting to Blucab_Blunami_reconnect() in about 1 minute...");
Serial.println();

// Pause for redireection
startmillis = millis(); while (millis() <= (startmillis + (60*connect_duration_delay))) {}  //  connect_duration_delay pause

Blucab_Blunami_reconnect();

} // close void connect_history
//*********************************************Bottom connect_history()***********************************


//**************************************Top Blucab_Blunami_reconnect()***************************************************
void Blucab_Blunami_reconnect()
{  // open Blucab_Blunami_reconnect()
  
Serial.println();
Serial.print("Entering Blucab_Blunami_reconnect()...");  
Serial.println();
Serial.println();

  int j;
 
  Serial.println("\t\tNo.\tAddress\t\t\tName\t\t\tConnected"); //  print header

  for (j = 1; j <= nBlunamis; j++)
    {  // Open for (j = 1; j <= nBlunamis; j++)

    while(!Blucab_Blunami[j].connected())
      { // open while(!Blucab_Blunami[j].connected())
      Blucab_Blunami[j].connect();
      } // close while (!Blucab_Blunami[j].connected())
      
     //Memorialize connection
     // Serial.print("         Blucab_Blunami[");
      Serial.print("\t\t");     //  set tab for blunami index number
      Serial.print(j);
    //  Serial.print("] address= ");
      Serial.print("\t");
      Serial.print(Blunamis[j - 1].Blunami_address);    //  print address
    //  Serial.print(" Device name= ");
      Serial.print("\t");
      Serial.print(Blunamis[j - 1].Blunami_local_name); //  print name
    //  Serial.print(" Availabilty= ");
    //  Serial.print(Blucab_Blunami[j]);
    //  Serial.print(" lap ");
    //  Serial.print(i);
    //  Serial.print(" Connected= ");
        Serial.print("\t\t");
        Serial.print(Blucab_Blunami[j].connected());
        Serial.println();

        startmillis = millis(); while (millis() <= (startmillis + 150)) {}  //  latency pause give it time before it connects to next one 

  } // Close for (j = 1; j <= nBlunamis; j++)

Blucab_Blunami1_connect_millis = millis();
Blucab_Blunami2_connect_millis = millis(); 
k=0;

Serial.println();
Serial.println("Testing reconnections durations");
Serial.println("redirecting to loop");
Serial.println();

loop();

} // close Blucab_Blunami_reconnect()
//***************************Bottom Blucab_Blunami_reconnect()**************************************************************


//**************************************Top Blucab_Blunami_disconnect()***************************************************
void Blucab_Blunami_disconnect()
{  // open Blucab_Blunami_disconnect()
  
Serial.println();
Serial.print("Entering Blucab_Blunami_disconnect()...");  
Serial.println();
  
  int j;
 
 
  for (j = 1; j <= nBlunamis; j++)
    {  // Open for (j = 1; j <= nBlunamis; j++)
     
      while (Blucab_Blunami[j].connected())
      { // open while (Blucab_Blunami[j].connected())
        
        Blucab_Blunami[j].disconnect();

      } // close while (!Blucab_Blunami[j].connected())
     
    } // close for(j = 1; j <= nBlunamis; j++)

Serial.println();
Serial.println("\t\tSurveying...");
Blunami_survey();   //  line 250    <-------------search for blunamis

// Blucab_Blunami_disconnect();

} // close Blucab_Blunami_disconnect()
//***************************Bottom Blucab_Blunami_disconnect()**************************************************************


//**************************************Top Blucab_Blunami_connect()***************************************************
void Blucab_Blunami_connect()
{  // open Blucab_Blunami_connect()
  
Serial.println();
Serial.print("Entering Blucab_Blunami_connect()...");  
Serial.println();
  
  int j;
  unsigned long Blucab_Blunami_connect_abort_millis;

  Serial.println("\t\tNo.\tAddress\t\t\tName\t\t\tConnected"); //  print header

Blucab_Blunami_connect_abort_millis = millis()+5000;

  for (j = 1; j <= nBlunamis; j++)
    {  // Open for (j = 1; j <= nBlunamis; j++)

    while(!Blucab_Blunami[j])
      { // open while((!Blucab_Blunami[j])

      BLE.scanForAddress(Blunamis[j - 1].Blunami_address);

      Blucab_Blunami[j] = BLE.available();

      } // close while(!Blucab_Blunami[j])
      
      while (!Blucab_Blunami[j].connected())
      { // open while (!Blucab_Blunami[j].connected())
        
        Blucab_Blunami[j].connect();
        startmillis = millis(); while (millis() <= (startmillis + command_delay)) {}  //  latency pause

        if (millis() >= Blucab_Blunami_connect_abort_millis)
        { //open if (millis >= Blucab_Blunami_connect_abort_millis)
        Serial.println();
        Serial.print("Aborting Blucab_Blunami_connect()...");  
        Serial.println("Recursively redirecting to Blucab_Blunami_disconnect()...");
        Serial.println();
        Blucab_Blunami_disconnect();
        } //close if (millis >= Blucab_Blunami_connect_abort_millis)

      } // close while (!Blucab_Blunami[j].connected())

    if(Blucab_Blunami[j].connected())
    { //  only print if connected
      //Memorialize connection
     // Serial.print("         Blucab_Blunami[");
      
      Serial.print("\t\t");     //  set tab for blunami index number
      Serial.print(j);
    //  Serial.print("] address= ");
      Serial.print("\t");
      Serial.print(Blunamis[j - 1].Blunami_address);    //  print address
    //  Serial.print(" Device name= ");
      Serial.print("\t");
      Serial.print(Blunamis[j - 1].Blunami_local_name); //  print name
    //  Serial.print(" Availabilty= ");
    //  Serial.print(Blucab_Blunami[j]);
    //  Serial.print(" lap ");
    //  Serial.print(i);
    //  Serial.print(" Connected= ");
        Serial.print("\t\t");
        Serial.print(Blucab_Blunami[j].connected());
        Serial.println();
      } //  close only print if connected      
        startmillis = millis(); while (millis() <= (startmillis + 150)) {}  //  latency pause give it time before it connects to next one 
     
    } // close for(j = 1; j <= nBlunamis; j++) 

// Blucab_Blunami_connect();

} // close Blucab_Blunami_connect()
//***************************Bottom Blucab_Blunami_connect()**************************************************************





//********************************TOP Blunami_matrix_serial_output*********************
void Blunami_matrix_serial_output()
{ // open Blunami_matrix_serial_output()

int i;

Serial.println();
Serial.println("\t\tNo.\tAddress\t\t\tName");

for (i = 0; i <= (nBlunamis - 1); i++)
{ // open for i
Serial.print("\t\t");
Serial.print(Blunamis[i].Blunami_index);

Serial.print("\t");
Serial.print(Blunamis[i].Blunami_address);
Serial.print("\t");
//Serial.print(Blunamis[i].Blunami_device_name);
//Serial.print("\t");
Serial.print(Blunamis[i].Blunami_local_name); //  THIS IS ALL WE CARE ABOUT
Serial.println();
} // close for i

} // close Blunami_matrix_serial_output()
//******************************************************************************************

//**************************Void Function mrblue_attributes()**********************
//once connected mrblue_attributes() prints out the characteristics that we use
//  plus it defines the 2 main characteristics, DCC & motorvoltage
void mrblue_attributes()
{  // open void mrblue_attributes()

int i;

// Landmark entry
Serial.println("\t\t\tinside of attributes function");

// memorialize Blunamis
Serial.print("\t\t\tnumber of Blunamis (@333)=  ");
Serial.println(nBlunamis);
Serial.println();                 

startmillis = millis(); while (millis() <= (startmillis + attribute_delay)) {} //  delay command (20)

Serial.println("\t\tNo.\tServices\tAttributes");
      //   Discover all Blunami attributes
for (i = 1; i <= nBlunamis; i++)
{ // open for (i = 1; i <= nBlunamis; i++)
          Serial.print("\t\t");Serial.print(i);
      //  Serial.print("\t");
     //     Serial.print(Blunamis[i].Blunami_address);
     //     Serial.print("\t");
      //    Serial.print(Blunamis[i].Blunami_local_name);          
          startmillis = millis(); while (millis() <= (startmillis + attribute_delay)) {} //  delay command (20) 
          Blunami_all_services = Blucab_Blunami[i].discoverService("f688fd00-da9a-2e8f-d003-ebcdcc259af9");
          startmillis = millis(); while (millis() <= (startmillis + attribute_delay)) {} //  delay command (20)   
         // startmillis = millis(); while (millis() <= (startmillis + attribute_delay)) {} //  delay command (20)
         // startmillis = millis(); while (millis() <= (startmillis + 100)) {} //  delay command (20)
          Serial.print("\t");
          Serial.print(Blunami_all_services);
          
          startmillis = millis(); while (millis() <= (startmillis + attribute_delay)) {} //  delay command (20)  
          Blunami_all_attributes =Blucab_Blunami[i].discoverAttributes();
         
        // startmillis = millis(); while (millis() <= (startmillis + attribute_delay)) {} //  delay command (20)
        //  startmillis = millis(); while (millis() <= (startmillis + 100)) {} //  delay command (20)
          Serial.print("\t\t");
          Serial.println(Blunami_all_attributes);
          startmillis = millis(); while (millis() <= (startmillis + attribute_delay)) {} //  delay command (20)

          
             
      //    if(!Blunami_all_attributes){mrblue_connect();}  //  if no attributes found, reconnect

  //      Obtain Blunami's advertised service Uuid    9/2/24 not necessary since we know what it is
      //  Blunami_ServiceUuid = Blunami.advertisedServiceUuid();
        //  startMillis = millis(); while (millis() <= (startMillis + setup_delay)) {} //  delay command 
        //  Serial.print("\tBlunami_ServiceUuid = ");
        //  Serial.println(Blunami_ServiceUuid);
        
          
  //      Query the number of Blunami characteristics
        //  Blunami_Num_Characteristics = Blunami.characteristicCount();  //9/12/24 not necessary
        //  Serial.print("\tBlunami characteristic count= ");
        //  Serial.println(Blunami_Num_Characteristics);

        //  startMillis = millis(); while (millis() <= (startMillis + setup_delay)) {} //  delay command 

    //   Define Blunami "DCC Characteristic"
          BlunamiCommand = Blucab_Blunami[i].characteristic("f688fd1d-da9a-2e8f-d003-ebcdcc259af9");
    //    confirm that blunami has a DCC characteristic
        //  if (BlunamiCommand){Serial.println("\tBlunami has a DCC characteristic");}
          //startMillis = millis(); while (millis() <= (startMillis + setup_delay)) {} //  delay command 

      //  Define motor/track voltage characteristic
          motorvoltage = Blucab_Blunami[i].characteristic("f688fd14-da9a-2e8f-d003-ebcdcc259af9");
      //  confirm that bunami has a motor voltage characteristic
        //  if(motorvoltage){Serial.println("\tBlunami has a Motor/voltage characteristic");}
    startmillis = millis(); while (millis() <= (startmillis + 100)) {} //  delay command (20)       
 
} // close for (i = 1; i <= nBlunamis; i++) 

}
//*************************bottom discover blunami attributes function***************


//******************************************top connection status**********************************
void connected_status(String label)
{ // open void connected_status(String label)
//  this function shows if conected

int i;

    Serial.print("\tconnected_status@ ");
    Serial.print(label);
    Serial.print("\t\t ");
    Serial.println("\connected\taddress");



for (i = 1; i <= (nBlunamis); i++)
{ // open for (i = 1; i <= nBlunamis; i++)
Serial.print("\t\t\t\t\t\t");
   if(Blucab_Blunami[i].connected())
{ // open if(Blucab_Blunami[i].connected())
    Serial.print(1);
} // close if(Blucab_Blunami[i].connected())
else
{  // open else
  Serial.print(0);
} // close else
    Serial.print("\t    ");
    Serial.println(Blucab_Blunami[i].address());
} // close for (i = 1; i <= nBlunamis; i++)

} // close void connected_status(label)
//**********************************************Bottom connection status**********************************