// include the library code:
#include <Ethernet.h>
#include <SPI.h>


////////////////////////////////////////////////////////////////////////
//USER DEFINED VARIBLES
////////////////////////////////////////////////////////////////////////
//


// IP address of LAMP server
char serverName[] = "192.168.0.27";

byte mac[] = {
  0x00,0x22,0x4D,0xAC,0xAF,0xF4};  


int dly = 10;

int node = 0;

//number of analog points 
#define analogpoints 5

// number of digital points
#define digitalpoints 5


// 1 for input
// 0 for output
//set equal to above numbers

//|D0|D1|D2|D3|D4|
int digital_io[] = {
  1, 1, 0, 0, 0};

//|A0|A1|A2|A3|A4|
int  analog_io[] = {
  1, 1, 0, 0, 0};
////////////////////////////////////////////////////////////////////////
//GLOBAL VARS
////////////////////////////////////////////////////////////////////////

EthernetClient client; 

int  analog_values[analogpoints];

int  digital_values[digitalpoints];




////////////////////////////////////////////////////////////////////////
//SETUP LOOP
////////////////////////////////////////////////////////////////////////
void setup(){

  Serial.begin(9600);

  Serial.println(F("booting......."));

  startEthernet();

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//MAIN LOOP
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop(){

  connectAndRead(); 

  analog_values[0] =  random(0,100);
  analog_values[1] =  random(0,100);
  if(random(0,100)<50){ digital_values[0] =  1;}else{digital_values[0] =  0;}
  if(random(0,100)<50){ digital_values[1] =  1;}else{digital_values[1] =  0;}
  
  Serial.println("Ditgital - 0  ==  " + String( digital_values[0]));
  Serial.println("Ditgital - 1  ==  " + String( digital_values[1]));
  Serial.println("Ditgital - 2  ==  " + String( digital_values[2]));
  Serial.println("Ditgital - 3  ==  " + String( digital_values[3]));
  Serial.println("Ditgital - 4  ==  " + String( digital_values[4]));


  Serial.println("Analog - 0  ==  " + String( analog_values[0]));
  Serial.println("Analog - 1  ==  " + String( analog_values[1]));
  Serial.println("Analog - 2  ==  " + String( analog_values[2]));
  Serial.println("Analog - 3  ==  " + String( analog_values[3]));
  Serial.println("Analog - 4  ==  " + String( analog_values[4]));

delay(3000);

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////







////////////////////////////////////////////////////////////////////////
//CONNECT TO SERVER --- SEND & READ inString
////////////////////////////////////////////////////////////////////////
void connectAndRead(){

  //Serial.println("connecting...");
  if (client.connect(serverName, 80)) {
    //Serial.println("connected");
    if( client.status() == 0){
      //Serial.println("bad client"); 
    }



   //client.print("GET /hmi/data/data.php?node=0");
    client.print("GET /hmi/data/data.php?node" + String(node));  
   


    for (int j = 0; j <= analogpoints; j++) {
    
    
        if ( analog_io[j] == 1){
    
           client.print("&a");
           client.print(String(j)); 
           client.print("="); 
           client.print(String(analog_values[j]));
    
    
        }
      }


    for (int j = 0; j <= digitalpoints; j++) {

      if ( digital_io[j] == 1){

       client.print("&d");
       client.print(String(j)); 
       client.print("="); 
       client.print(String(digital_values[j]));

      }

    }



 client.println();


   readPage(); //go and read the output
  }
  else{
    //return "connection failed";
  }

}
////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////
//READ PAGE
////////////////////////////////////////////////////////////////////////
void readPage(){
  //read the page, and capture & return everything between '>' and '<'

  char inString[128]; // string for incoming //Serial inString
  int stringPos = 0; // string index counter
  boolean startRead = false; // is reading?

  memset( &inString, 0, 128 ); //clear inString memory

  if( client.status() == 0){
    //Serial.println("bad client"); 

  }

  delay(200);
  while(client.connected()){
      //Serial.println("connected");
    if (client.available()) {
      char ca = client.read();
      //Serial.println(ca);
      if (ca == '>' ) { //'<' is our begining character
        startRead = true; //Ready to start reading the part 
      }
      else if(startRead){

        if(ca != '<'){ //'>' is our ending character
          inString[stringPos] = ca;

          stringPos ++;
        }
        else{
          //got what we need here! We can disconnect now
          startRead = false;
          client.stop();
          client.flush();

          //Serial.println(F("---------DATA STRING From SERVER--------")); 
          //Serial.println(inString);
          //Serial.println(F("----------------------------------------"));
          dataHandler(inString);
          

        }

      }
    }
    else{ 
      //Serial.println(F(" client not avalible"));
    }

  }

}







void startEthernet()
{

  client.stop();
  client.flush();

  Serial.println("Connecting Arduino to network...");
  Serial.println();  

  delay(1000);

  // Connect to network amd obtain an IP address using DHCP
  if (Ethernet.begin(mac) == 0)
  {
    Serial.println("DHCP Failed, reset Arduino to try again");
    Serial.println();
  }
  else
  {
    Serial.println("Arduino connected to network at: ");
    Serial.println(Ethernet.localIP());
  }

  delay(1000);
}





void dataHandler(char* data ){

  char analog[128];
  char digital[128];  

  memset( &analog, 0, 128 ); 
  memset( &digital, 0, 128 );  

  boolean startparse = false; // is reading?

  int pointer = 0; 
  int analog_pointer = 0; 
  int digital_pointer = 0; 
  int acount = 0;
  int dcount = 0;
  int z = 0;
  int y = 0;
  //########################################################
  //##     SPLIT DATA SECTIONS    ANALOG ###################
  //########################################################
  while(true){
    //Serial.println("analog " + String(pointer));
    if (data[pointer] == '[' and data[pointer+1] == 'a' ) { 
      startparse = true; 
      //Serial.println("startparse");
    }

    if(startparse){

      if(data[pointer] != ']'){ 
        analog[analog_pointer] = data[pointer];
          //Serial.println(data[pointer]);
        analog_pointer = analog_pointer + 1;
      }
      else{
        analog[analog_pointer] = data[pointer];
        startparse = false; 
        break;

      }
    }//if startparse
    pointer = pointer + 1;
  }//while 

  //Serial.println("ANALOG POINTER = " + String(pointer));

  for (int j = 0; j <= pointer+1; j++) {
    //Serial.print(analog[j]);
  }

  while(z < (pointer)){
    if(analog[z] == '='){
      acount = acount + 1;
    }
    z = z + 1;
  }

  //########################################################
  //##     SPLIT DATA SECTIONS    DIGITAL ##################
  //########################################################
  while(true){
    //Serial.println("DIGITAL " + String(pointer));
    if (data[pointer] == '[' and data[pointer+1] == 'd' ) { 
      startparse = true; 
      //Serial.println("startparse");
    }

    if(startparse){

      if(data[pointer] != ']'){ 
        digital[digital_pointer] = data[pointer];
          //Serial.println(data[pointer]);
        digital_pointer = digital_pointer + 1;
      }
      else{
        digital[digital_pointer] = data[pointer];
        startparse = false; 
        break;

      }
    }//if startparse
    pointer = pointer + 1;
  }//while 

  //Serial.println("DIGITAL POINTER = " + String(pointer));

  for (int j = 0; j <= pointer+5; j++) {
    //Serial.print(digital[j]);
  }



  z = 0;
  while(z < (pointer-analog_pointer)){
    if(digital[z] == '='){
      dcount = dcount + 1;
    }
    z = z + 1;
  }

  //Serial.println("analog count" + String(acount));
  //Serial.println("digital count" + String(dcount));
  //Serial.println("ENDING....." );



  int point_count = 0;
  int pos0 = 0;
  int pos1 = 0;
  int pos2 = 0;

  //########################################################
  //##     FIND POSITION  ANALOG   #########################
  //########################################################
  

  
  for (int j = 0; j <= acount - 1; j++){

     int k = pos2 - 1;
      
    while(true){

      if(analog[k] == 'a'){ 
        pos0 = k;
      }
      if(analog[k] == '='){
        pos1 = k;
        break;

      }
      k = k + 1;
    }//while

    int j = pos1;


    while(true){
      if(analog[j] == 'a'or analog[j] == ']'){
        pos2 = j;
        break;      
      }
      j = j + 1;   
    }//while
    
  int point_digits = pos1 - pos0 - 1;
  int value_digits =  pos2 - pos1 -1;
  int point_value = 0;
  int value = 0;   

 //Serial.println("pos0 = " + String(pos0)); //a
 //Serial.println("pos1 = " + String(pos1)); //=
 //Serial.println("pos2 = " + String(pos2)); //a

 //Serial.println("point_digits = " + String(point_digits));
 //Serial.println("value_digits = " + String(value_digits));
  //Serial.println("done>>>>");

  if (point_digits == 3){
    point_value = charToInt(analog[pos0 + 1], analog[pos0 + 2], analog[pos0 + 3]);
  }
  if (point_digits == 2){
    point_value = charToInt('0', analog[pos0 + 1], analog[pos0 + 2]);
  }
  if (point_digits == 1){
    point_value = charToInt('0', '0', analog[pos0 + 1]);
  }

  if (value_digits == 3){
    value = charToInt(analog[pos1 + 1], analog[pos1 + 2], analog[pos1 + 3]);
  }
  if (value_digits == 2){
    value = charToInt('0', analog[pos1 + 1], analog[pos1 + 2]);
  }
  if (value_digits == 1){
    value = charToInt('0', '0', analog[pos1 + 1]);
  }

  //Serial.println("ANALOG POINT " + String(point_value)+ " = "+ String(value));
  analog_values[point_value] = value;

  }
  
  
  
  
  //########################################################
  //##     FIND POSITION  DIGITAL   #########################
  //########################################################
  
   point_count = 0;
   pos0 = 0;
   pos1 = 0;
   pos2 = 0;
  
 
  for (int s = 0; s <= dcount - 1; s++){

     int h = pos2 - 1;
      
    while(true){

      if(digital[h] == 'd'){ 
        pos0 = h;
      }
      if(digital[h] == '='){
        pos1 = h;
        break;

      }
      h = h + 1;
    }//while

    int s = pos1;


    while(true){
      if(digital[s] == 'd'or digital[s] == ']'){
        pos2 = s;
        break;      
      }
      s = s + 1;   
    }//while
    
  int point_digits = pos1 - pos0 - 1;
  int value_digits =  pos2 - pos1 -1;
  int point_value = 0;
  int value = 0;   

 //Serial.println("pos0 = " + String(pos0)); //a
 //Serial.println("pos1 = " + String(pos1)); //=
 //Serial.println("pos2 = " + String(pos2)); //a

 //Serial.println("point_digits = " + String(point_digits));
  //Serial.println("value_digits = " + String(value_digits));
  //Serial.println("done>>>>");

  if (point_digits == 3){
    point_value = charToInt(digital[pos0 + 1], digital[pos0 + 2], digital[pos0 + 3]);
  }
  if (point_digits == 2){
    point_value = charToInt('0', digital[pos0 + 1], digital[pos0 + 2]);
  }
  if (point_digits == 1){
    point_value = charToInt('0', '0', digital[pos0 + 1]);
  }

  if (value_digits == 3){
    value = charToInt(digital[pos1 + 1], digital[pos1 + 2], digital[pos1 + 3]);
  }
  if (value_digits == 2){
    value = charToInt('0', digital[pos1 + 1], digital[pos1 + 2]);
  }
  if (value_digits == 1){
    value = charToInt('0', '0', digital[pos1 + 1]);
  }



  //Serial.println("DIGITAL POINT " + String(point_value)+ " = "+ String(value));

  digital_values[point_value] = value;
  }
  
  
}  




int charToInt(char hundred, char ten, char one){

  int num = 0;
  int hund_int = 0;
  int ten_int = 0;  
  int one_int = 0;  

  if(hundred == '1'){
    hund_int = 1;
  }
  else if (hundred == '2'){
    hund_int = 2;
  }
  else if (hundred == '3'){
    hund_int = 3;
  }
  else if (hundred == '4'){
    hund_int = 4;
  }
  else if (hundred == '5'){
    hund_int = 5;
  }
  else if (hundred == '6'){
    hund_int = 6;
  }
  else if (hundred == '7'){
    hund_int = 7;
  }
  else if (hundred == '8'){
    hund_int = 8;
  }
  else if (hundred == '9'){
    hund_int = 9;
  }
  else if (hundred == '9'){
    hund_int = 0;    
  }

  if(ten == '1'){
    ten_int = 1;
  }
  else if (ten == '2'){
    ten_int = 2;
  }
  else if (ten == '3'){
    ten_int = 3;
  }
  else if (ten == '4'){
    ten_int = 4;
  }
  else if (ten == '5'){
    ten_int = 5;
  }
  else if (ten == '6'){
    ten_int = 6;
  }
  else if (ten == '7'){
    ten_int = 7;
  }
  else if (ten == '8'){
    ten_int = 8;
  }
  else if (ten == '9'){
    ten_int = 9;
  }
  else if (ten == '9'){
    ten_int = 0;    
  }

  if(one == '1'){
    one_int = 1;
  }
  else if (one == '2'){
    one_int = 2;
  }
  else if (one == '3'){
    one_int = 3;
  }
  else if (one == '4'){
    one_int = 4;
  }
  else if (one == '5'){
    one_int = 5;
  }
  else if (one == '6'){
    one_int = 6;
  }
  else if (one == '7'){
    one_int = 7;
  }
  else if (one == '8'){
    one_int = 8;
  }
  else if (one == '9'){
    one_int = 9;
  }
  else if (one == '9'){
    one_int = 0;    
  }

  num = (hund_int*100) + (ten_int*10) + (one_int*1);

  // //Serial.println(num);  

  return num;

}  



























