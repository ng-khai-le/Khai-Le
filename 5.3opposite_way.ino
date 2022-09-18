#include <Servo.h>
#include <math.h>
int light_1;
int light_2;
int light_3;
int light_4;
static int pos1=90;
static int pos2=90;
const int range=20;
Servo Servo_1;
Servo Servo_2;
int Vo_AADAT;
int Vo_Fixed;
double Vin_AADAT;
double Vin_Fixed;
double real_Vo_AADAT;
double real_Vo_Fixed;
double C_AADAT;
double C_Fixed;
double P_AADAT;
double P_Fixed;

////////////////////////////////////////////////////////////////////////////////////////////////////////
int horizontal_right (int pos2){           
  Servo_2.attach(10);
  while( pos2 < 180) { // goes from 180 degrees to 0 degrees
    
   int light_2=analogRead(A1);
   int light_4=analogRead(A3);
  
   if( ((light_4-light_2)<range) && ((light_4-light_2)>-range)){  //-range< x <range, means balance
     break;}   
   else if(pos1>90){
      if((light_4-light_2)<-range){
          break;} }
   else{ //pos1<=90
      if((light_4-light_2)>range){
          break;} }                
   
    pos2+= 1;
    Servo_2.write(pos2);              // tell servo 2 to go to position in variable 'pos'
    delay(15);                        // waits 15 ms for the servo to reach the position  
  
  }//end while
  Servo_2.detach();
  return pos2;
}//end void

////////////////////////////////////////////////////////////////////////////////////////////////////////
int horizontal_left (int pos2){
  Servo_2.attach(10);
  while( pos2 > 0) { // goes from 180 degrees to 0 degrees

   int light_2=analogRead(A1);
   int light_4=analogRead(A3);

   if( ((light_4-light_2)<range) && ((light_4-light_2)>-range)){  //-range< x <range, means balance
      break;}
   else if(pos1>90){
      if((light_4-light_2)>range){
          break;} }
   else{ //pos1<=90
       if((light_4-light_2)<-range){
          break;} }

   pos2-= 1;
   Servo_2.write(pos2);              // tell servo 2 to go to position in variable 'pos'
   delay(15);                        // waits 15 ms for the servo to reach the position
   
  }//end while
  Servo_2.detach();
  return pos2;
}//end void
////////////////////////////////////////////////////////////////////////////////////////////////////////
int vertical_up (int pos1){
  Servo_1.attach(9);
  while(pos1 < 145){ // goes from 0 degrees to 180 degrees

   light_1=analogRead(A0);
   light_3=analogRead(A2);

   if( ((light_1-light_3)<range) && ((light_1-light_3)>-range)){  //-range< x <range, means balance
    break;}
   else if((light_1-light_3)<-range){ 
    break;} 
   else{pos1 += 1;
    Servo_1.write(pos1);              // tell servo 1 to go to position in variable 'pos'
    delay(15); }                      // waits 15 ms for the servo to reach the position
   
  }//end while
  Servo_1.detach();
  return pos1;
}//end function

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int vertical_down (int pos1){
  Servo_1.attach(9);
  while(pos1 > 35){ // goes from 0 degrees to 180 degrees

   light_1=analogRead(A0);
   light_3=analogRead(A2);

   if( ((light_1-light_3)<range) && ((light_1-light_3)>-range)){  //-range< x <range, means balance
    break;}
   else if((light_1-light_3)>range){
    break;}
   else{pos1 -= 1;
    Servo_1.write(pos1);      // tell servo 1 to go to position in variable 'pos'
    delay(15); }               // waits 15 ms for the servo to reach the position
   
  }//end while
  Servo_1.detach();
  return pos1;
}//end function

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  
Servo_1.attach(9);
Servo_2.attach(10);
Servo_1.write(pos1);    //Reset Servo1 to original location
Servo_2.write(pos2);    //Reset Servo1 to original location
delay(15);
Servo_1.detach();
Servo_1.attach(9);

Serial.begin(9600);          //to trigger serial monitor
Serial.println("CLEARDATA"); //clears up any data left from previous projects
Serial.println("LABEL,Time,Timer,Date,ini_pos1,ini_pos2,Fin_LDR1,Fin_LDR3,Fin_LDR2,Fin_LDR4,VIn_AADAT,VIn_fixed,realV_AADAT(mV),realV_fixed(mV),C_AADAT(mA),C_fixed(mA),P_AADAT,P_fixed"); 
//always write LABEL, so excel knows the next things will be the names of the columns (instead of Acolumn you could write Time for instance)
Serial.println("RESETTIMER"); //resets timer to 0
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} //end void setup
///////////////////////////////////////////

void loop() {
/*
if( (light_1 || light_3)<=300){
  pos1=180-pos1;
  };
*/

Serial.print("DATA,TIME,TIMER,DATE,"); //writes the time in the first column A and the time since the measurements started in column B
Serial.print(pos1);
Serial.print(",");
Serial.print(pos2);
Serial.print(",");
////////////////////////////////////////////////////////////////////
//vertical first,then horizontal 
//condition 3
light_1=analogRead(A0);
light_3=analogRead(A2);
if(  (light_1 - light_3)>range  ){   
  pos1 = vertical_up(pos1); };//end condtion 3
/////////////////////////////////////
// conditon 4
light_1=analogRead(A0);
light_3=analogRead(A2);
if(  (light_1 - light_3)<-range  ){ 
  pos1 = vertical_down(pos1); };//end condtion 4
/////////////////////////////////////
light_2=analogRead(A1);
    light_4=analogRead(A3);
    if(  (light_4-light_2)>range   ){
      if(pos1>90){
        pos2 = horizontal_right(pos2);}else{
        pos2 = horizontal_left(pos2);}
      };//end condtion 1
///////////////////////////////////////
    light_2=analogRead(A1);
    light_4=analogRead(A3);
    if(  (light_4-light_2)<-range   ){
      if(pos1>90){
        pos2 = horizontal_left(pos2);}else{ 
        pos2 = horizontal_right(pos2);}
      };//end condtion 2
///////////////////////////////////////      
Vo_AADAT  = analogRead(A5);
Vo_Fixed  = analogRead(A4);
real_Vo_AADAT = (Vo_AADAT * 5) /1023;
real_Vo_Fixed = (Vo_Fixed * 5) /1023;
C_AADAT   = real_Vo_AADAT/ 12250;
C_Fixed   = real_Vo_Fixed/ 12250;
Vin_AADAT  = (( C_AADAT * 22750 ) + real_Vo_AADAT);
Vin_Fixed  = (( C_Fixed * 22750 ) + real_Vo_Fixed);
P_AADAT   = pow(Vin_AADAT,2)/35000;
P_Fixed   = pow(Vin_Fixed,2)/35000;

Serial.print(light_1);
Serial.print(",");
Serial.print(light_3);
Serial.print(",");
Serial.print(light_2);
Serial.print(",");
Serial.print(light_4);
Serial.print(",");
Serial.print(Vo_AADAT);
Serial.print(",");
Serial.print(Vo_Fixed);
Serial.print(",");
Serial.print(real_Vo_AADAT);
Serial.print(",");
Serial.print(real_Vo_Fixed);
Serial.print(",");
Serial.print(C_AADAT);
Serial.print(",");
Serial.print(C_Fixed);
Serial.print(",");
Serial.print(P_AADAT);
Serial.print(",");
Serial.print(P_Fixed);
Serial.print(",");
Serial.println();     //extra spaces to make debugging data easier to read
Serial.println();  

delay(1000);
//////////////////////////////////////////////////////////////////////////////////////////////////  
}//end void loop
