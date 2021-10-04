#include <Servo.h>
#include <FastLED.h>
#include <Wire.h>
#include <DFPlayerMini_Fast.h>

// 0x4B is the default i2c address
#define MAX9744_I2CADDR 0x4B
#define MIC 0
#define ARR_SIZE 4
#define CUSTOM_DATA_START 12
#define FLAG_CUSTOM_DATA_START 12
#define FLAG_GAME_DATA_START 10
#define FLAG_OROSCOPE_START 14
#define SIGN_DATA_START 5
#define DATA_GAME_START 11
#define MONKEY 1
#define GIRAFFE 2

#define PIR_LEFT 2
#define PIR_RIGHT 3
#define START_STRAW 0
#define END_STRAW 19
#define STRAW_STRIPE 22
#define LOGO_STRIPE 23
#define EYEGIRAFFE_STRIPE 4
#define EYEMONKEY_STRIPE 5
#define servoPinHandMonkey 11
#define servoPinBodyMonkey 10
#define servoPinHeadGiraffe 9
#define servoPinTailGiraffe 8
#define NUM_LEDS_STRAW 20
#define NUM_LEDS_LOGO 33
#define NUM_LEDS_EYEGIRAFFE 5
#define NUM_LEDS_EYEMONKEY 2

//define the sound number
#define MUSIC 6
#define GLUGLU 1
#define TASTY 2
#define EVERYONE 38
#define GLIN 3 
#define COME 4
#define WAVE_ME 7
#define LAUGHS 8 
#define PLIK 9
#define PLOK 10
#define DIFF_CLIENT 11
#define CHOOSE_S 12
#define CAPRIC_S 13
#define CAPRIC_R 14
#define AQUARI_S 15
#define AQUARI_R 16
#define PISCES_S 17
#define PISCES_R 18
#define ARIES_S 19
#define ARIES_R 20
#define TAURUS_S 21
#define TAURUS_R 22
#define GEMINI_S 23
#define GEMINI_R 24
#define CANCER_S 25
#define CANCER_R 26
#define LEO_S 27
#define LEO_R 28
#define VIRGO_S 29
#define VIRGO_R 30
#define LIBRA_S 31
#define LIBRA_R 32
#define SCORPIO_S 33
#define SCORPIO_R 34
#define SAGITAR_S 35
#define SAGITAR_R 36
#define M_NOISE 37 
#define THANKS 39
#define G_SCAN 40
#define M_SCAN 41
#define INSIDE 42
#define OUTSIDE 43
#define WIFI 44
#define CONN 45 
#define DISCONN 46 
#define THUMBS 47


#define C_LETTER 3
#define A1_LETTER 4
#define M_LETTER 5
#define P_LETTER 6
#define A2_LETTER 7
#define R_LETTER 8
#define I_LETTER 9
#define ALL_LETTER 10
#define E_MONKEY 1
#define E_GIRAFFE 2
#define logo_stripe 7
#define NUM_LEDS 5
#define C_START 0
#define C_END 5
#define A1_START 7
#define A1_END 8
#define M_START 11
#define M_END 14
#define P_START 16
#define P_END 19
#define A2_START 21
#define A2_END 22
#define R_START 24
#define R_END 28
#define I_START 30
#define I_END 32
#define EYE_M_END 2
#define EYE_G_END 5
#define BTN_MODE 12

//instatiate the four servos
Servo ServoHandMonkey; 
Servo ServoBodyMonkey; 
Servo ServoHeadGiraffe;
Servo ServoTailGiraffe;

//this array is the ine used to receive the values of the colors
int field_values[ARR_SIZE] = {0,0,0,0};
int arraySigns[12] = {CAPRIC_S,AQUARI_S,PISCES_S,ARIES_S,TAURUS_S,GEMINI_S,CANCER_S,LEO_S,VIRGO_S,LIBRA_S,SCORPIO_S,SAGITAR_S};
int arrayRecipes[12] = {CAPRIC_R,AQUARI_R,PISCES_R,ARIES_R,TAURUS_R,GEMINI_R,CANCER_R,LEO_R,VIRGO_R,LIBRA_R,SCORPIO_R,SAGITAR_R};
int score_values[2] = {0,0};
int flagGame = 0;
int flagCustom = 0;
int flagOroscope = 0;
int sMonkey = 0;
int sGiraffe = 0;
int game_index = 10 ;
int winner = 0;
bool gameOver = false;
int valueGesture = -1;
bool pirLeft = false;
bool pirRight = false;
bool wifiOn = false;

int posHandMonkey = 1566;
int posBody = 1570;
int posHeadGiraffe = 0;
int posTail = 1166;
int startHand = 1566;
int endHandOutside = 1275;
int endHandInside = 950;
int startPosBody = 1550;
int startHeadGiraffe =0;
int endHeadGiraffe = 1350;
int startHeadGame = 800;
int endHeadPleasure = 1500;
int endLeft = 917;
int endRight = 2000;//2083
int startTailGiraffe =1166;
int endUpTailGiraffe = 950;
int endDownTailGiraffe = 1500;
bool personDetected = false;
bool fault = false;
byte initVol = 30;
bool inside = false;
int valueSign = -1;
bool receiveData=false;
int onCustomSwing = 0;
bool pedestrianDetected = false;
bool monkey = false;
bool enterGame = false;
int endUpReachedT = 0;
int endUpHandT = 0;
int posTailT = 0;
int eyeOnG = 0;
bool onOroscope = false;
unsigned long playDelay = 0;
bool mode_changed = false;
bool wifi_conn = false;
bool stateChanged = false;
bool first = false;
bool firstContact = true;
DFPlayerMini_Fast myMP3;

CRGB leds_straw[NUM_LEDS_STRAW];
CRGB leds_logo[NUM_LEDS_LOGO];
CRGB leds_eyeGiraffe[NUM_LEDS_EYEGIRAFFE];
CRGB leds_eyeMonkey[NUM_LEDS_EYEMONKEY];

boolean setvolume(int8_t volume);
void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  Serial1.begin(9600);
  pinMode(PIR_LEFT,INPUT);
  digitalWrite(PIR_LEFT,LOW);
  pinMode(PIR_RIGHT,INPUT);
  digitalWrite(PIR_RIGHT,LOW);
  // put your setup code here, to run once:
  attachInterrupt(digitalPinToInterrupt(PIR_LEFT),motionDetected1,RISING);
  attachInterrupt(digitalPinToInterrupt(PIR_RIGHT),motionDetected2,RISING);
  pinMode(BTN_MODE,INPUT);
  FastLED.addLeds<WS2812, STRAW_STRIPE, GRB>(leds_straw, NUM_LEDS_STRAW);
  FastLED.addLeds<WS2812, LOGO_STRIPE, GRB>(leds_logo, NUM_LEDS_LOGO);
  FastLED.addLeds<WS2812, EYEMONKEY_STRIPE, GRB>(leds_eyeMonkey, NUM_LEDS_EYEMONKEY);
  FastLED.addLeds<WS2812, EYEGIRAFFE_STRIPE, GRB>(leds_eyeGiraffe, NUM_LEDS_EYEGIRAFFE);
  
  colorStraw(START_STRAW, END_STRAW , 0, 0, 0);
  lightUpLetters(0, 0, 0);
  colorEyeMonkey(255,255,255);
  dotGiraffe();
  detachServos();
  FastLED.show();
  attachServos();
  myMP3.begin(Serial1, true);
  Serial.println("Setting volume to max");
  myMP3.volume(30);
  
  Wire.begin(); 
 
  // initialize the volume
  if (! setvolume(initVol)) {
    fault = true; //show that the amplifier is not connected
    Serial.println("Volume Not set");
  }
  //Verify inside/outside mode
    if(digitalRead(BTN_MODE)==HIGH){
    inside = false;
    }else{
      inside = true;
      }
   //Play inside/outside mode
  if(inside){
     myMP3.playFromMP3Folder(INSIDE);
    }else{
     myMP3.playFromMP3Folder(OUTSIDE);   
      }

  ServoHeadGiraffe.attach(servoPinHeadGiraffe);
  ServoHandMonkey.attach(servoPinHandMonkey);
  ServoBodyMonkey.attach(servoPinBodyMonkey);
  moveHeadGiraffeHigh();
  ServoTailGiraffe.attach(servoPinTailGiraffe);
  ServoTailGiraffe.writeMicroseconds(startTailGiraffe);
  ServoHandMonkey.writeMicroseconds(startHand);
  monkeySwing(1);
 
}

void loop() {

  verifyMode();
  if(!inside){
  if(pedestrianDetected){
  musicOutside();
  monkeySwing(4);
  myMP3.playFromMP3Folder(M_NOISE);
  waitMilliseconds(7000);
  giraffeBendNeck();
  strawUp();
  giraffeDrink();
  giraffeNeckUp();

  monkeyEveryOneLike();
  waveTail();
  CampariLetterIntensity();
  pedestrianDetected = false;
  }
  }else{
    if(firstContact){
    if(!pedestrianDetected){
      
      }else{
    myMP3.playFromMP3Folder(WAVE_ME);
    waitMilliseconds(4000);
    myMP3.playFromMP3Folder(THUMBS);
    waitMilliseconds(7000);
     if(!first){
      if(valueGesture==0 || valueGesture==-1){
        myMP3.playFromMP3Folder(DIFF_CLIENT);
        waitMilliseconds(6000);
        }else{
        myMP3.playFromMP3Folder(M_SCAN);
        waitMilliseconds(6000);
    }
      first = true;
    }
    firstContact = false;
    }
 }
    receiveDataFromWifi();

    if(receiveData){
      if(flagCustom){
      if(onCustomSwing ==0){
        monkeySwing(2);
        onCustomSwing = 1;
        }
      detachServos();
      customizeLetter( field_values[3],field_values[0],field_values[1],field_values[2]);
      attachServos();
      }else{
          if(winner != MONKEY)dotGiraffe();
          colorEyeMonkey(255,255,255);
          colorLetterLogo(0,32,0,0,0);
          onCustomSwing = 0;
          for(int i =0;i<4;i++){
            field_values[i]=0;
            }
        }
      if(flagGame==1){
           if(!enterGame){
            enterGame = true;
            moveHeadGiraffeHalf();
            }
           
          if(game_index>=0 && !gameOver){
            playGame(game_index);
            if (game_index == 0 || game_index>=20){
              gameOver = true;
              }  
            }else if(gameOver){
              if(winner==MONKEY){
               // Serial2.println("0");
                monkeyWin();
                sMonkey = 0;   
                }else if(winner == GIRAFFE){

                giraffeWin();
                sGiraffe =0;
               
                  }
              winner = -1;
              flagGame = 0;
              enterGame = false;
              gameOver = false;
              }
        
        }else{
          
          game_index = 10;

          }

         if(flagOroscope){
          if(!onOroscope){
          monkeyAskChoice();
          onOroscope = true;
          }

          if(valueSign!=-1){
            playOroscope(valueSign);
            }
          valueSign=-1;
          
          }else{
            onOroscope = false;
            }
      receiveData = false;
    }
  }
}

void verifyMode(){
   if(digitalRead(BTN_MODE)==HIGH && inside==true){
    inside = false;
    myMP3.playFromMP3Folder(OUTSIDE);
    waitMilliseconds(3000);
    }else if(digitalRead(BTN_MODE)==LOW && inside==false){
    inside = true;
    myMP3.playFromMP3Folder(INSIDE);
    waitMilliseconds(2000);
      } 
  }

void verifyWifi(){
  
  if(wifiOn && stateChanged){
     myMP3.playFromMP3Folder(CONN);
    }else if (!wifiOn && stateChanged){
       myMP3.playFromMP3Folder(DISCONN);
      }
  
  }  
//HOROSCOPE
void monkeyAskChoice(){
  
  unsigned long last = millis();
  unsigned long lastDelay = millis();
  int eyeOn = 0;
  //setvolume(20);
  myMP3.playFromMP3Folder(CHOOSE_S);
  colorEyeMonkey(0,0,0);
  while(millis()-lastDelay<4000){

    if(millis()-last>=300){
      if(eyeOn ==0){
        colorEyeMonkey(255,255,255);
        eyeOn =1;
        }else{ 
        colorEyeMonkey(0,0,0);
         eyeOn =0;
        }
        last = millis();
      }
    }
    colorEyeMonkey(255,255,255);  
  }


void giraffePlayTail(){
        ServoTailGiraffe.writeMicroseconds(posTailT);
      if(posTailT==endUpTailGiraffe){
        endUpReachedT = 1 ;
        }
      if(posTailT==endDownTailGiraffe){
        endUpReachedT = 0 ;
        }           
      delayMicroseconds(900);
      if(endUpReachedT == 1 ){
        posTailT +=1;
        }
       else{
        posTailT -=1;
        }

      if(millis()-playDelay>=300){
      if(eyeOnG ==0){
        dotGiraffe();
        eyeOnG =1;
        }else{ 
         offEyeGiraffe();
         eyeOnG =0;
        }
        playDelay = millis();
      }
  }

void monkeyPlay(){
  if(millis()-playDelay>=300){
      if(eyeOnG ==0){
        colorEyeMonkey(255,255,255);
        eyeOnG =1;
        }else{ 
         colorEyeMonkey(0,0,0);
         eyeOnG =0;
        }
        playDelay = millis();
      }
  }
  
void playOroscope(int index){
  unsigned long last = millis();
  playDelay = millis();
  myMP3.playFromMP3Folder(arraySigns[index]);
  eyeOnG = 0; 
  posTailT = startTailGiraffe;
  endUpReachedT = 0;
  endUpHandT = 0;
  switch(index){
    case 0:
    moveHeadGiraffeHalf();
    while(millis()-last<26000){
        
        giraffePlayTail();
        
      }
    waitMilliseconds(1000);
     myMP3.stop();
     
    myMP3.playFromMP3Folder(arrayRecipes[index]);
    last = millis();
    while(millis()-last<13000){
        giraffePlayTail();
        
      }
    break;
    case 1:
     moveHeadGiraffeHalf();
    while(millis()-last<20000){
       
        giraffePlayTail();
       
      }
      waitMilliseconds(1000);
      myMP3.stop();
      
    myMP3.playFromMP3Folder(arrayRecipes[index]);
    last = millis();
    while(millis()-last<10000){
        giraffePlayTail();
       
      }
    break;
    case 2:
     moveHeadGiraffeHalf();
    while(millis()-last<24000){
        
        giraffePlayTail();
        
      }
      waitMilliseconds(1000);
      myMP3.stop();
      
    myMP3.playFromMP3Folder(arrayRecipes[index]);
    last = millis();
    while(millis()-last<16000){
        giraffePlayTail();
       
      }
    break;
    case 3:
     moveHeadGiraffeHalf();
    while(millis()-last<22000){
       
        giraffePlayTail();
       
      }
      waitMilliseconds(1000);
      myMP3.stop();
      
    myMP3.playFromMP3Folder(arrayRecipes[index]);
    last = millis();
    while(millis()-last<16000){
        giraffePlayTail();
        
      }
    break;
    case 4:
    moveHeadGiraffeHalf();
    while(millis()-last<26000){
        
        giraffePlayTail();
     
      }
     waitMilliseconds(1000);
     myMP3.stop();
     
    myMP3.playFromMP3Folder(arrayRecipes[index]);
    last = millis();
    while(millis()-last<19000){
        giraffePlayTail();
      
      }
    break;
    case 5:
    moveHeadGiraffeHalf();
    while(millis()-last<27000){
        
        giraffePlayTail();
       
      }
     waitMilliseconds(1000);
     myMP3.stop();
     
    myMP3.playFromMP3Folder(arrayRecipes[index]);
    last = millis();
    while(millis()-last<17000){
        giraffePlayTail();
       
      }
      
    break;
    case 6:
    monkeySwing(1);
    while(millis()-last<7000){
    monkeyPlay();
    
    }
    waitMilliseconds(1000);
    myMP3.stop();
   
    myMP3.playFromMP3Folder(arrayRecipes[index]);
     last = millis();
    while(millis()-last<21000){
    monkeyPlay();
    
    }
    break;
    case 7:
    monkeySwing(1);
    while(millis()-last<7000){
    monkeyPlay();
    
    }
    waitMilliseconds(1000);
    myMP3.stop();
    myMP3.playFromMP3Folder(arrayRecipes[index]);
    last = millis();
    while(millis()-last<17000){
    monkeyPlay();
   
    }
    break;
    case 8:
    monkeySwing(1);
    while(millis()-last<8000){
    monkeyPlay();
    }
    waitMilliseconds(1000);
    myMP3.stop();
    myMP3.playFromMP3Folder(arrayRecipes[index]);
    last = millis();
    while(millis()-last<16000){
    monkeyPlay();
   
    }
    break;
    case 9:
    monkeySwing(1);
    while(millis()-last<9000){
    monkeyPlay();
    
    }
    waitMilliseconds(1000);
    myMP3.stop();
    myMP3.playFromMP3Folder(arrayRecipes[index]);
    last = millis();
    while(millis()-last<17000){
    monkeyPlay();
  
    }
    break;
    case 10:
    monkeySwing(1);
    while(millis()-last<8000){
    monkeyPlay();
  
    }
    waitMilliseconds(1000);
    myMP3.stop();
    myMP3.playFromMP3Folder(arrayRecipes[index]);
    last = millis();
    while(millis()-last<16000){
    monkeyPlay();
  
    }
    break;
    case 11:
    monkeySwing(1);
    while(millis()-last<8000){
    monkeyPlay();
    
    }
    waitMilliseconds(1000);
    myMP3.stop();
    myMP3.playFromMP3Folder(arrayRecipes[index]);
    last = millis();
    while(millis()-last<20000){
    monkeyPlay();
   
    }
    break;
    default:
    break; 
    }

    posHeadGiraffe = startHeadGame;
    for (posHeadGiraffe = startHeadGame; posHeadGiraffe<=endHeadGiraffe; posHeadGiraffe++){
      ServoHeadGiraffe.writeMicroseconds(posHeadGiraffe);
      delayMicroseconds(1200);
      }
  
  }


//MOTION DETECTED
void motionDetected1(){
  
  pedestrianDetected = true;
  //Serial.println("Motion detected left");
  //pirLeft = true;
  

  }

void motionDetected2(){
  
  pedestrianDetected = true;
  //Serial.println("Motion detected right");
  //pirRight = true;
  
  }

void detachServos(){
  
      ServoHandMonkey.detach();
      ServoBodyMonkey.detach();
      ServoTailGiraffe.detach();
      ServoHeadGiraffe.detach();
  
  }

void attachServos(){
  ServoHeadGiraffe.attach(servoPinHeadGiraffe);
  ServoHandMonkey.attach(servoPinHandMonkey);
  ServoBodyMonkey.attach(servoPinBodyMonkey);
  ServoTailGiraffe.attach(servoPinTailGiraffe);
  
  }
void receiveDataFromWifi(){
  
  if(Serial2.available()>0){
      int index = 0;
      int last_index= 0;
      int last_i = 0;
      int index_i = 0;
      char bfr[501];
      memset(bfr,0, 501);
      Serial2.readBytesUntil( '\n',bfr,500);
      String data = String(bfr);
      if(data.substring(0,CUSTOM_DATA_START).equals("custom data ")){ 
        String data2 = data.substring(CUSTOM_DATA_START,data.length()); 
      for (int i = 0; i < data2.length(); i++) {
      if (data2.substring(i, i+1) == ",") {
        field_values[index] = data2.substring(last_index, i).toInt();
        index++;
        last_index=i+1;
      }
       if (i == data2.length() - 1) {
              // Grab the last part of the string from the lastIndex to the end
              field_values[index] = data2.substring(last_index, i+1).toInt();
           }
    } 
      }else if(data.substring(0,FLAG_CUSTOM_DATA_START).equals("Flag custom ")){
        String data2 = data.substring(FLAG_CUSTOM_DATA_START,data.length());
        flagCustom = data2.toInt();
        
      }else if(data.substring(0,FLAG_GAME_DATA_START).equals("Flag game ")){
        String data2 = data.substring(FLAG_GAME_DATA_START,data.length());
        flagGame = data2.toInt();
        
      }else if(data.substring(0,DATA_GAME_START).equals("score game ")){

      String data2 = data.substring(DATA_GAME_START,data.length());
      for (int i = 0; i < data2.length(); i++) {
      if (data2.substring(i, i+1) == ",") {
        score_values[index_i] = data2.substring(last_i, i).toInt();
        index_i++;
        last_i=i+1;
      }
       if (i == data2.length()-1) {
              // Grab the last part of the string from the lastIndex to the end
              score_values[index_i] = data2.substring(last_i, i+1).toInt();
           }
    }

    if(score_values[0]==1){
  
      sMonkey = score_values[1];
      monkey = true;
      //Serial.println("Score monkey "+String(sMonkey ));
      }else if(score_values[0]==2){
  
      sGiraffe = score_values[1];
      monkey = false;
      //Serial.println("Score Giraffe "+String(sGiraffe ));
      }
     int diff = sGiraffe-sMonkey;
     if (diff <=-3 && monkey==true){
        game_index-- ;
        if (game_index<=0){
          game_index = 0;
          winner = MONKEY;

          }
      }else if(diff>=3 && monkey==false){
        game_index++ ;
        if (game_index>=20){
          game_index = 20;
          winner = GIRAFFE;

          }  
        }        
      }else if(data.substring(0,FLAG_OROSCOPE_START).equals("Flag oroscope ")){
        String data2 = data.substring(FLAG_OROSCOPE_START,data.length());
        flagOroscope = data2.toInt();
        
      }else if(data.substring(0,SIGN_DATA_START).equals("sign ")){
        String data2 = data.substring(SIGN_DATA_START,data.length());
        valueSign = data2.toInt();
        
      }else if(data.substring(0,12).equals("dataGesture ")){
        String data2 = data.substring(12,data.length());
        valueGesture = data2.toInt();
        
      }else if(data.equals("Wifi on")){
        wifiOn = true;
        stateChanged = true;
        
      }else if(data.equals("Wifi off")){
        wifiOn = false; 
        stateChanged = true;
      }
     receiveData = true;
    Serial.println(bfr);
    Serial.println(field_values[0]);
    Serial.println(field_values[1]);
    Serial.println(field_values[2]);
    Serial.println(field_values[3]);
    Serial.println("flag custom: "+String(flagCustom));
    Serial.println("flag game : "+String(flagGame));
    Serial.println("flag oroscope "+String(flagOroscope));
    Serial.println("Score Giraffe "+String(sGiraffe));
    Serial.println("Score monkey "+String(sMonkey));
    Serial.println("value sign "+String(valueSign));
     Serial.println("value gesture "+String(valueGesture));  
    }  
}



void musicOutside(){
  unsigned long startPlay = millis();
 
  myMP3.playFromMP3Folder(MUSIC);
  waitMilliseconds(6000);
  
}
void monkeyWaveGiraffeTail(){
  for(int i=0; i<=4; i++){
    for (posHandMonkey = startHand; posHandMonkey >= endHandOutside; posHandMonkey -= 2) {
      ServoHandMonkey.writeMicroseconds(posHandMonkey);              
      delayMicroseconds(1200);                      
    }
 
    for (posHandMonkey = endHandOutside; posHandMonkey <= startHand ; posHandMonkey += 2) { 
      // in steps of 1 degree
      ServoHandMonkey.writeMicroseconds(posHandMonkey);             
      delayMicroseconds(1200);                    
    }
  }
} 

void monkeySwing(int times){
  int posHand = startHand;//1566  1200
  int posTail = startTailGiraffe;
  posBody = startPosBody;
  int endUpReached = 0;
  int endUpHand = 0;
  for(int i = 0; i<times; i++){
    
    for (posBody = startPosBody; posBody >= endLeft; posBody -= 1) { 
      ServoBodyMonkey.writeMicroseconds(posBody);
      ServoHandMonkey.writeMicroseconds(posHand);
      ServoTailGiraffe.writeMicroseconds(posTail);


      if(posTail==endUpTailGiraffe){
        endUpReached = 1 ;
        }
      if(posTail==endDownTailGiraffe){
        endUpReached = 0 ;
        }           
      delayMicroseconds(900);
      if(endUpReached == 1 ){
        posTail +=1;
        }
       else{
        posTail -=1;
        } 
      if((posBody%2)==0){ 
        posHand -=1;}
    }
    
    for (posBody = endLeft; posBody <= startPosBody; posBody += 1) { 
      ServoBodyMonkey.writeMicroseconds(posBody);
      ServoHandMonkey.writeMicroseconds(posHand); 
      ServoTailGiraffe.writeMicroseconds(posTail);

      
      if(posTail==endUpTailGiraffe){
        endUpReached = 1 ;
        }
      if(posTail==endDownTailGiraffe){
        endUpReached = 0 ;
        }           
      delayMicroseconds(900);
      if(endUpReached == 1 ){
        posTail +=1;
        }
       else{
        posTail -=1;
        } 
         if((posBody%2)==0 ){ 
        posHand +=1;}
    }
   
    
    for (posBody = startPosBody; posBody <= endRight; posBody += 1) { 
      ServoBodyMonkey.writeMicroseconds(posBody);          
     
      ServoTailGiraffe.writeMicroseconds(posTail);
      
      if(posTail==endUpTailGiraffe){
        endUpReached = 1 ;
        }
      if(posTail==endDownTailGiraffe){
        endUpReached = 0 ;
        }           
      delayMicroseconds(900);
      if(endUpReached == 1 ){
        posTail +=1;
        }
       else{
        posTail -=1;
        }              
       
    }
     
    for (posBody = endRight; posBody >=startPosBody ; posBody -= 1) { 
      ServoBodyMonkey.writeMicroseconds(posBody);   
                  
      ServoTailGiraffe.writeMicroseconds(posTail);


      if(posTail==endUpTailGiraffe){
        endUpReached = 1 ;
        }
      if(posTail==endDownTailGiraffe){
        endUpReached = 0; 
        }           
      delayMicroseconds(900);
      if(endUpReached == 1 ){
        posTail +=1;
        }
       else{
        posTail -=1;
        }  
                         
    }
  }
  ServoTailGiraffe.writeMicroseconds(startTailGiraffe);
  detachServos();
  leds_straw[0] = CRGB ( 255, 0, 0);
  FastLED.show();
  attachServos();
  waitMilliseconds(1500);
}


void monkeySwingGame(int times){
  int posHand = startHand;//1566  1200
  int posTail = startTailGiraffe;
  int endUpReached = 0;
  int endUpHand = 0;
  for(int i = 0; i<times; i++){
    
    for (posBody = startPosBody; posBody >= endLeft; posBody -= 1) { 
      ServoBodyMonkey.writeMicroseconds(posBody);
      ServoHandMonkey.writeMicroseconds(posHand);
          
      delayMicroseconds(900);
     
      if((posBody%2)==0 && posHand>endHandInside){ 
        posHand -=1;}else if(posHand<=endHandInside){posHand=endHandInside;}
    }   
    for (posBody = endLeft; posBody <= startPosBody; posBody += 1) { 
      ServoBodyMonkey.writeMicroseconds(posBody);
      ServoHandMonkey.writeMicroseconds(posHand); 
           
      delayMicroseconds(900);
     
    }
   
    for (posBody = startPosBody; posBody <= endRight; posBody += 1) { 
      ServoBodyMonkey.writeMicroseconds(posBody);          
            
      delayMicroseconds(900);
                 
    }
     
    for (posBody = endRight; posBody >=startPosBody ; posBody -= 1) { 
      ServoBodyMonkey.writeMicroseconds(posBody);   
 
      delayMicroseconds(900);
       
                         
    }
  }
  ServoTailGiraffe.writeMicroseconds(startTailGiraffe);
  
  leds_straw[0] = CRGB ( 255, 0, 0);
  detachServos();
  FastLED.show();
  attachServos();
  //waitMilliseconds(200);
   for (posBody = startPosBody; posBody >= endLeft; posBody -= 1) { 
      ServoBodyMonkey.writeMicroseconds(posBody);
      delayMicroseconds(800); 
    }   
}

void giraffeCollapse(){
  posHeadGiraffe = endHeadGiraffe;
  int posTail = startTailGiraffe;
  int endUpReached = 0;
  for (posHeadGiraffe = endHeadGiraffe; posHeadGiraffe >= startHeadGiraffe ; posHeadGiraffe -= 2) { 
    ServoHeadGiraffe.writeMicroseconds(posHeadGiraffe);
    ServoTailGiraffe.writeMicroseconds(endDownTailGiraffe);
    delayMicroseconds(1000); 
     
    if(posTail==endUpTailGiraffe){
        endUpReached = 1 ;
        }
      if(posTail==endDownTailGiraffe){
        endUpReached = 0 ;
        }           
      delayMicroseconds(1200);
      if(endUpReached == 1 ){
        posTail +=1;
        }
       else{
        posTail -=1;
        }     
  }
  ServoHeadGiraffe.writeMicroseconds(startHeadGiraffe);
  crossGiraffe();
  }


void restPosition(){
  
  posHeadGiraffe = startHeadGiraffe;
  int posTail = startTailGiraffe;
  int endUpReached = 0;
  for (posHeadGiraffe = startHeadGiraffe; posHeadGiraffe <= endHeadGiraffe ; posHeadGiraffe += 2) { 
    ServoHeadGiraffe.writeMicroseconds(posHeadGiraffe);
    ServoTailGiraffe.writeMicroseconds(startTailGiraffe);
    delayMicroseconds(1000); 
         
  }
  dotGiraffe();
    posBody = endLeft;
    for (posBody = endLeft ; posBody <= startPosBody; posBody += 1) { 
      ServoBodyMonkey.writeMicroseconds(posBody);
      delayMicroseconds(800); 
    }
     int posHand = endHandInside;  
     for (posHand = endHandInside ; posHand  <= startHand; posHand  += 1) { 
      ServoHandMonkey.writeMicroseconds(posHand );
      delayMicroseconds(800); 
    }
  
  }



void giraffeWin(){
  giraffeNeckUpGame();
  giraffeBendNeck();
  strawUp();
  giraffeDrink();
  giraffeNeckUp(); 
  waveTailGame();
  }
  
void monkeyWin(){
  
    monkeySwingGame(2);
    myMP3.playFromMP3Folder(LAUGHS);
    giraffeCollapse();
    waitMilliseconds(5000);
    restPosition();
  }

  
void giraffeBendNeck(){
  int posTail = startTailGiraffe;
  int endUpReached = 0;
  for (posHeadGiraffe = endHeadGiraffe; posHeadGiraffe >= startHeadGiraffe ; posHeadGiraffe -= 1) { 
    ServoHeadGiraffe.writeMicroseconds(posHeadGiraffe);
    delayMicroseconds(900); 
    if(posTail==endUpTailGiraffe){
        endUpReached = 1 ;
        }
      if(posTail==endDownTailGiraffe){
        endUpReached = 0 ;
        }           
      delayMicroseconds(1200);
      if(endUpReached == 1 ){
        posTail +=1;
        }
       else{
        posTail -=1;
        }     
  }
}

void giraffeDrink(){
  detachServos();
  unsigned long last = millis();
   unsigned long lastDelay = millis();
  
  int eyeOn = 0;
  offEyeGiraffe();
  for(int i = 0; i<2; ){
    if(millis()-last>=500){
      if (eyeOn ==0){
        dotGiraffe();
        eyeOn = 1;
        last = millis();
        }else{
          offEyeGiraffe();
          eyeOn = 0;
          last=millis();
        }
    }
    if(millis()-lastDelay>=1100){
      
      i+=1;
      
      }
  }
  attachServos();
}
void strawUp(){
  detachServos();
   int ledOn = 1;
  unsigned long last= millis();
 for (int i = START_STRAW ; i <= END_STRAW; i++) {
      leds_straw[i] = CRGB ( 255, 0, 0);

      if(i==10){ myMP3.playFromMP3Folder(GLUGLU);}
      detachServos();
      FastLED.show();
      waitMilliseconds(200);
      if(millis()-last>500){
        if(ledOn==1){
          offEyeGiraffe();
          ledOn = 0;
          }else{
          dotGiraffe(); 
          ledOn = 1;
          }
          last = millis();
        }
    }
    dotGiraffe();
  attachServos();
}



void giraffeNeckUpGame(){
  detachServos();
 int i = 19; 
 unsigned long tmp_start = millis();
 unsigned long last = millis();
 posHeadGiraffe = startHeadGiraffe;
 dotGiraffe();
 for (posHeadGiraffe = startHeadGiraffe; posHeadGiraffe <= endHeadGiraffe; posHeadGiraffe += 1) { 
    attachServos();
    ServoHeadGiraffe.writeMicroseconds(posHeadGiraffe);   
    delayMicroseconds(2000);    
  } 
}



void giraffeNeckUp(){
  detachServos();
 int i = 19; 
 unsigned long tmp_start = millis();
 unsigned long last = millis();
 posHeadGiraffe = startHeadGiraffe;
 dotGiraffe();
 for (posHeadGiraffe = startHeadGiraffe; posHeadGiraffe <= endHeadGiraffe; posHeadGiraffe += 1) { 
    attachServos();
    ServoHeadGiraffe.writeMicroseconds(posHeadGiraffe);   
    delayMicroseconds(2000); 
    if(posHeadGiraffe >= (startHeadGiraffe+300)){
    if(millis()-last>130  && i>0){ 
     leds_straw[i] = CRGB ( 0, 0, 0);
     
     i= i-1;
     last=millis();
     detachServos();
     FastLED.show();  
    }   
   }   
 }
  attachServos(); 
  myMP3.playFromMP3Folder(TASTY);
  waitMilliseconds(3000);  
}


void monkeyEveryOneLike(){
  unsigned long last = millis();
  unsigned long lastDelay = millis();
  int eyeOn = 0;
  //setvolume(20);
  myMP3.playFromMP3Folder(EVERYONE);
  colorEyeMonkey(0,0,0);
  while(millis()-lastDelay<4000){

    if(millis()-last>=300){
      if(eyeOn ==0){
        colorEyeMonkey(255,255,255);
        eyeOn =1;
        }else{ 
        colorEyeMonkey(0,0,0);
         eyeOn =0;
        }
        last = millis();
      }
    }
    colorEyeMonkey(255,255,255);
}

void HeartEyeGlowsUp(){
 heartGiraffe();
}

void GiraffeSmallNeckMvt(){
  for (posHeadGiraffe = endHeadGiraffe; posHeadGiraffe >= startHeadGiraffe ; posHeadGiraffe -= 2) { 
    ServoHeadGiraffe.writeMicroseconds(posHeadGiraffe);
    delayMicroseconds(100); 
  }
  for (posHeadGiraffe = startHeadGiraffe; posHeadGiraffe <= endHeadGiraffe; posHeadGiraffe += 2) { 
    ServoHeadGiraffe.writeMicroseconds(posHeadGiraffe);            
    delayMicroseconds(100);
  }   
}

void waveTail(){
  unsigned long last = millis();
  int posHead = endHeadGiraffe;
  int eyeOn = 1;
  heartGiraffe();
  myMP3.playFromMP3Folder(GLIN);
  for(int i= 0; i<4;i++){
  for (posTail = startTailGiraffe; posTail >= endUpTailGiraffe ; posTail -= 1) { 
    ServoTailGiraffe.writeMicroseconds(posTail);
    if(posHead<endHeadPleasure){
       ServoHeadGiraffe.writeMicroseconds(posHead);
      }
   
    delayMicroseconds(1200); 
     if(millis()-last>=300){
      if(eyeOn ==0){
        heartGiraffe();
        eyeOn =1;
        }else{ 
        offEyeGiraffe();
         eyeOn =0;
        }
        last = millis();
      }
      posHead +=1;
  }
  for (posTail = endUpTailGiraffe; posTail <= startTailGiraffe; posTail += 1) { 
    ServoTailGiraffe.writeMicroseconds(posTail);            
    delayMicroseconds(1200);
     if(millis()-last>=300){
      if(eyeOn ==0){
        heartGiraffe();
        eyeOn =1;
        }else{ 
        offEyeGiraffe();
         eyeOn =0;
        }
        last = millis();
      }
  }
   for (posTail = startTailGiraffe; posTail <= endDownTailGiraffe ; posTail += 1) { 
    ServoTailGiraffe.writeMicroseconds(posTail);
    delayMicroseconds(1200); 
      if(millis()-last>=300){
      if(eyeOn ==0){
        heartGiraffe();
        eyeOn =1;
        }else{ 
        offEyeGiraffe();
         eyeOn =0;
        }
        last = millis();
      }
  }
  for (posTail = endDownTailGiraffe; posTail >= startTailGiraffe; posTail -= 1) { 
    ServoTailGiraffe.writeMicroseconds(posTail);            
    delayMicroseconds(1200);
     if(millis()-last>=300){
      if(eyeOn ==0){
        heartGiraffe();
        eyeOn =1;
        }else{ 
        offEyeGiraffe();
         eyeOn =0;
        }
        last = millis();
      }
  } 
  if(i==2){
     myMP3.playFromMP3Folder(COME);
    }
  }
  dotGiraffe();
  posHead = endHeadPleasure;
  for(posHead=endHeadPleasure;posHead>=endHeadGiraffe;posHead--){
    ServoHeadGiraffe.writeMicroseconds(posHead);
    delayMicroseconds(900);
    
    }
  waitMilliseconds(2000);
}


void waveTailGame(){
  unsigned long last = millis();
  int posHead = endHeadGiraffe;
  int eyeOn = 1;
  heartGiraffe();
  myMP3.playFromMP3Folder(GLIN);
  for(int i= 0; i<4;i++){
  for (posTail = startTailGiraffe; posTail >= endUpTailGiraffe ; posTail -= 1) { 
    ServoTailGiraffe.writeMicroseconds(posTail);
    if(posHead<endHeadPleasure){
       ServoHeadGiraffe.writeMicroseconds(posHead);
      }
   
    delayMicroseconds(1200); 
     if(millis()-last>=300){
      if(eyeOn ==0){
        heartGiraffe();
        eyeOn =1;
        }else{ 
        offEyeGiraffe();
         eyeOn =0;
        }
        last = millis();
      }
      posHead +=1;
  }
  for (posTail = endUpTailGiraffe; posTail <= startTailGiraffe; posTail += 1) { 
    ServoTailGiraffe.writeMicroseconds(posTail);            
    delayMicroseconds(1200);
     if(millis()-last>=300){
      if(eyeOn ==0){
        heartGiraffe();
        eyeOn =1;
        }else{ 
        offEyeGiraffe();
         eyeOn =0;
        }
        last = millis();
      }
  }
   for (posTail = startTailGiraffe; posTail <= endDownTailGiraffe ; posTail += 1) { 
    ServoTailGiraffe.writeMicroseconds(posTail);
    delayMicroseconds(1200); 
      if(millis()-last>=300){
      if(eyeOn ==0){
        heartGiraffe();
        eyeOn =1;
        }else{ 
        offEyeGiraffe();
         eyeOn =0;
        }
        last = millis();
      }
  }
  for (posTail = endDownTailGiraffe; posTail >= startTailGiraffe; posTail -= 1) { 
    ServoTailGiraffe.writeMicroseconds(posTail);            
    delayMicroseconds(1200);
     if(millis()-last>=300){
      if(eyeOn ==0){
        heartGiraffe();
        eyeOn =1;
        }else{ 
        offEyeGiraffe();
         eyeOn =0;
        }
        last = millis();
      }
  } 

  }
  dotGiraffe();
  posHead = endHeadPleasure;
  for(posHead=endHeadPleasure;posHead>=endHeadGiraffe;posHead--){
    ServoHeadGiraffe.writeMicroseconds(posHead);
    delayMicroseconds(900);
    
    }
 
}


void CampariLetterIntensity(){
 unsigned long last = millis();
 int ledOn = 0;

  
    lightUpLetters(150, 0, 0);             
    waitMilliseconds(300);
    lightUpLetters(0, 0, 0);
    waitMilliseconds(300);
    lightUpLetters(255, 0, 0);              
    waitMilliseconds(300);
    lightUpLetters(0, 0, 0);
}

void fast(){
   ServoHandMonkey.write(95);
  delay(200);
  ServoHandMonkey.write(57);
  delay(1000);
  }

void bodyMovementLeft(){
   int i = 20; 
   unsigned long tmp_start = millis();
   unsigned long last = millis();
   for (posBody = startPosBody; posBody >= endLeft; posBody -= 2) { 
    ServoBodyMonkey.writeMicroseconds(posBody);             
    delayMicroseconds(1200);  
 
  }

  for (posBody = endLeft; posBody <= startPosBody; posBody += 2) { 
    ServoBodyMonkey.writeMicroseconds(posBody); 
    delayMicroseconds(1200);  
  }  
}

void moveHeadGiraffeDown(){
    for (posHeadGiraffe = endHeadGiraffe; posHeadGiraffe >= startHeadGiraffe ; posHeadGiraffe -= 1) { 
    ServoHeadGiraffe.writeMicroseconds(posHeadGiraffe);             
    delayMicroseconds(10);                    
  }
 }
void moveHeadGiraffeHigh(){
 
   int i = 19; 
   unsigned long tmp_start = millis();
   unsigned long last = millis();
   for (posHeadGiraffe = startHeadGiraffe; posHeadGiraffe <= endHeadGiraffe; posHeadGiraffe += 2) {
    ServoHeadGiraffe.writeMicroseconds(posHeadGiraffe);             
    delayMicroseconds(1200);  

     if(i==20){
      leds_straw[i] = CRGB ( 0, 0, 0);
      FastLED.show();
      i= i-1;
      }
    if(millis()-last>25 && posHeadGiraffe >= startHeadGiraffe+50  && i>=0){
      
     leds_straw[i] = CRGB ( 0, 0, 0);
     FastLED.show();
     last=millis();
     i= i-1;
     }
  }
 }
 
void moveHeadGiraffeHalf(){
 
   posHeadGiraffe = endHeadGiraffe;
   for (posHeadGiraffe = endHeadGiraffe; posHeadGiraffe >= startHeadGame; posHeadGiraffe -= 1) {
    ServoHeadGiraffe.writeMicroseconds(posHeadGiraffe);             
    delayMicroseconds(1200);  
  
  }
 }
 
void HeadGiraffeMovement(){
   int i = 20; 
   unsigned long tmp_start = millis();
   unsigned long last = millis();
   for (posHeadGiraffe = startHeadGiraffe; posHeadGiraffe <= endHeadGiraffe; posHeadGiraffe += 1) { 
    ServoHeadGiraffe.writeMicroseconds(posHeadGiraffe);             
    delayMicroseconds(0);  
   
  }

  for (posHeadGiraffe = endHeadGiraffe; posHeadGiraffe >= startHeadGiraffe ; posHeadGiraffe -= 1) { 
   
    ServoHeadGiraffe.writeMicroseconds(posHeadGiraffe);           
    delayMicroseconds(0);        
    Serial.print("Iam here too ");

  }
}

void bodyMovementRight(){

   for (posBody = startPosBody; posBody <= endRight; posBody += 1) { 
    ServoBodyMonkey.writeMicroseconds(posBody);          
    delayMicroseconds(1200);  

  }

  for (posBody = endRight; posBody >=startPosBody ; posBody -= 1) { 
  
    ServoBodyMonkey.writeMicroseconds(posBody);              
    delayMicroseconds(1200);                    
  }
 }



void increment(){
  
   for (posHandMonkey = 95; posHandMonkey >= 57; posHandMonkey -= 1) {
    ServoHandMonkey.write(posHandMonkey);              
    delay(50);                    
  }
 
  for (posHandMonkey = 57; posHandMonkey <=95 ; posHandMonkey += 5) { 
    ServoHandMonkey.write(posHandMonkey);              
    delay(50);                     
  }
  }

void moveHandMonkey(){
  
   for (posHandMonkey = startHand; posHandMonkey >= endHandInside; posHandMonkey -= 2) { 
    ServoHandMonkey.writeMicroseconds(posHandMonkey);            
    delayMicroseconds(900);                    
  }

  for (posHandMonkey = endHandInside; posHandMonkey <=startHand ; posHandMonkey += 2) { 

    ServoHandMonkey.writeMicroseconds(posHandMonkey);            
    delayMicroseconds(900);                    
  }
  }

void colorStraw(int startLetter, int endLetter , int R, int G, int B){
  detachServos();
  for (int i = startLetter ; i <= endLetter; i++) {
   
          leds_straw[i] = CRGB ( R, G, B);
          FastLED.show();
          delay(150);
        }
   attachServos(); 
  }
  
void strawHigh(){
  detachServos();
   for (int i = START_STRAW ; i <= END_STRAW; i++) {
   
          leds_straw[i] = CRGB ( 255, 0, 0);
          FastLED.show();
          delay(150);
        }
   attachServos();
  }
  
void strawDown(){
   for (int i = END_STRAW ; i >=  START_STRAW; i--) {
   
          leds_straw[i] = CRGB ( 0, 0, 0);
          FastLED.show();
          delay(150);
        }
  }



//EYES GIRAFFES FUNCTIONS

void heartGiraffe(){

  detachServos();
  for(int i=0 ; i<5;i++){
            if(i==0 || i==2){
               leds_eyeGiraffe[i] = CRGB ( 0, 0, 0);
              }
             else{
              leds_eyeGiraffe[i] = CRGB ( 0, 255, 0);
              } 
    }
     fadeToBlackBy( leds_eyeGiraffe, 5, 200);
     FastLED.show();
     attachServos();
}

  
void crossGiraffe(){
  
  for(int i=0 ; i<5;i++){
            leds_eyeGiraffe[i] = CRGB ( 0, 255, 0);
            detachServos();
            FastLED.show();
        
  }
   detachServos();
  fadeToBlackBy( leds_eyeGiraffe, 5, 200);
  FastLED.show();
  attachServos();
}

void dotGiraffe(){

for(int i=0 ; i<5;i++){
          if(i==1){
             leds_eyeGiraffe[i] = CRGB ( 255,255,255);
            }
           else{
            leds_eyeGiraffe[i] = CRGB ( 0, 0, 0);
            }
          //FastLED.show();
          //delay(10);
}
      detachServos();
      fadeToBlackBy( leds_eyeGiraffe, 5, 200);
      FastLED.show();
   attachServos();
  }

void offEyeGiraffe(){

for(int i=0 ; i<5;i++){

          leds_eyeGiraffe[i] = CRGB ( 0, 0, 0);
          detachServos();
          FastLED.show();
         // waitMilliseconds(10);
}
attachServos();
  }

void colorEyeGiraffe(int R, int G, int B){
  
  for(int i=0 ; i<5;i++){
          if(i==0 || i==2){
             leds_eyeGiraffe[i] = CRGB ( 0, 0, 0);
            }
           else{
            leds_eyeGiraffe[i] = CRGB ( R, G, B);
            }
           detachServos();
          FastLED.show();
          //waitMilliseconds(1);
}
    attachServos();
  }



//EYES MONKEY FUNCTIONS

void colorEyeMonkey(int R, int G, int B){
  detachServos();
  for(int i= 0 ; i<2;i++){
          
      leds_eyeMonkey[i] = CRGB ( G, R, B);
      FastLED.show();
          
}
   fadeToBlackBy( leds_eyeMonkey, 2, 200);
   FastLED.show();
   attachServos();
 }


//LOGO CUSTOMIZATION FUNCTIONS
void customizeLetter( int letter,int R,int G,int B){
  if (letter == C_LETTER){
       colorLetterLogo(C_START,C_END,R,G,B);
      }else if (letter == A1_LETTER){
        colorLetterLogo(A1_START,A1_END,R,G,B);
        }else if(letter == M_LETTER){
          colorLetterLogo(M_START,M_END,R,G,B);
          }else if(letter == P_LETTER){
            colorLetterLogo(P_START,P_END,R,G,B);
            }else if(letter == A2_LETTER){
              colorLetterLogo(A2_START,A2_END,R,G,B);
              }else if(letter == R_LETTER){
                colorLetterLogo(R_START,R_END,R,G,B);
                }else if(letter ==I_LETTER){
                  colorLetterLogo(I_START,I_END,R,G,B);
                  }else if(letter == ALL_LETTER){
                    colorLetterLogo(C_START,I_END,R,G,B);
                    }else if(letter == E_MONKEY){
                      colorEyeMonkey(R,G,B);
                      myMP3.playFromMP3Folder(LAUGHS);
                      }else if(letter == E_GIRAFFE){
                      colorEyeGiraffe(G,R,B);
                      myMP3.playFromMP3Folder(GLIN);
                      }
  }


void lightUpLetters(int R, int G, int B){
       
       colorLetterLogo(C_START,C_END,R,G,B);
       colorLetterLogo(A1_START,A1_END,R,G,B);
       colorLetterLogo(M_START,M_END,R,G,B);
       colorLetterLogo(P_START,P_END,R,G,B);
       colorLetterLogo(A2_START,A2_END,R,G,B);
       colorLetterLogo(R_START,R_END,R,G,B);
       colorLetterLogo(I_START,I_END,R,G,B);
       colorLetterLogo(C_START,I_END,R,G,B);
       lightOffIntLeds();
       attachServos();
  }

void lightOffIntLeds(){
  leds_logo[6] = CRGB ( 0, 0, 0);
  leds_logo[9] = CRGB ( 0, 0, 0);
  leds_logo[10] = CRGB ( 0, 0, 0);
  leds_logo[15] = CRGB ( 0, 0, 0);
  leds_logo[20] = CRGB ( 0, 0, 0);
  leds_logo[23] = CRGB ( 0, 0, 0);
  leds_logo[29] = CRGB ( 0, 0, 0);
  detachServos();
  FastLED.show();
   attachServos();
  
  }
 void colorLetterLogo(int startLetter, int endLetter , int R, int G, int B){
  
  unsigned long last = millis();
  for (int i = startLetter ; i <= endLetter; ) {
          if(millis()-last>=5){
          leds_logo[i] = CRGB ( R, G, B);
          last=millis();
          i+=1;
          }
        }
        detachServos();
        FastLED.show();
 
  lightOffIntLeds();
   attachServos();
 }


// MAX9744 FUNCTIONS
void volumeAdjustment(){
  int val = analogRead(MIC);
  Serial.println(val);
  if(val>100){
    setvolume(43);
    }else{
      setvolume(initVol);
      }
  }

//MULTIPLAYER

void playGame(int l_index){
    //Serial.println("game");
    int last = millis();
    for (int i = START_STRAW ; i < l_index; i++) {
          if(millis()-last>10){
          leds_straw[i] = CRGB ( 255, 0, 0);
          detachServos();
          FastLED.show();
          last = millis();
          }
        }
    for(int i = l_index; i<=END_STRAW; i++){
          if(millis()-last>10){
          leds_straw[i] = CRGB ( 0, 0, 0);
          detachServos();
          FastLED.show();
          last = millis();
          }
      }
    attachServos();
    if(l_index>=10 && l_index<21){
      ServoHeadGiraffe.writeMicroseconds(800-((l_index-10)*20));
      delayMicroseconds(1200);
      }else if (l_index<10){
       ServoHeadGiraffe.writeMicroseconds(800+((10-l_index)*55));
       delayMicroseconds(1200);
        }
    
  }


//function to set the volume of the amplifier
boolean setvolume(int8_t volume) {
  // The volume can't be higher than 63 or lower than 0
  if (volume > 63) volume = 63;
  if (volume < 0) volume = 0;
  
  Serial.print("Setting volume to ");
  Serial.println(volume);
  Wire.beginTransmission(MAX9744_I2CADDR);
  Wire.write(volume);
  if (Wire.endTransmission() == 0) 
    return true;
  else
    return false;
}

//delay function
void waitMilliseconds(uint16_t msWait)
{
  uint32_t start = millis();
  
  while ((millis() - start) < msWait)
  {
  }
}
