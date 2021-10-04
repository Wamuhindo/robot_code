/*YOU SHOULD NOT TOUCH ANYTHING IN THIS CODE 
UPLOAD JUST THE CODE IN THE BOARD TO CHANGE THE WIFI SETTINGS
1. GO TO TOOLS MENU
2. THEN BOARD -> ESP32 ARDUINO -> AI THINKER ESP32-CAM
3. THEN IN THE SAME TOLLS MENU CLICK ON PORT AND CHOOSE THE PORT THAT IS HILIGHTEN IN BLACK
4. THEN GO TO MENU SKETCH AND THEN CLICK ON UPLOAD
YOU SHOULD SEE THE STATE OF THE UPLOADING PROCESS STATE IN THE BLACK ZONE AT THE BOTTOM OF THIS WINDOW
MAKE SURE YOU SWITCH OFF THE ROBOT AND THE MODE SWITCH IS AT THE WIFI POSITION(II)
AFTER THE UPLOADING PROCESS IS FINISHED, UNPLUG THE CABLE, PUT THE MODE SWITCH TO THE RIGHT POSITION AND THEN YOU CAN USE THE ROBOT
*/


#include <PubSubClient.h>
#include <WiFi.h>
#include "Secret.h"
#define arr_size 4

#define GIRAFFE 2
#define MONKEY 1

char ssid[] = SSID_WIFI;   
char password[] = PASSWORD;   
int keyIndex = 0;           

int sGiraffe = 0;
int sMonkey = 0;

boolean badGesture = true;
int gesture = 0;
unsigned int flag_game = 0;
unsigned int flag_custom = 0;

int mRed = 0;
int mGreen = 0;
int mBlue = 0;
int mLetter = 0;

char* mqtt_server = "test.mosquitto.org";
int mqtt_port = 1883;
char* mqtt_clientID = "robotic1245";
char* mqtt_username = "";
char* mqtt_password = "";
void mycallback(char* topic, byte* payload, unsigned int length);

char* mqtt_publish_topic = "gameOverRandD";
char* mqtt_custom_flag ="onCustomPageRandD";
char* mqtt_game_flag ="onGamePageRandD";
char* mqtt_custom_data = "customRandD";
char* mqtt_game_data ="scoreRandD";
char* mqtt_camera_data ="gestureRandD";
char* mqtt_onOroscopePage = "onOroscopePageRandD";
char* mqtt_sign = "signRandD";
int game_index = 10;
int winner = 0;
unsigned long lastMillis = 0;
boolean personDetected = false;
int flagOroscope = 0;
int valueSign = -1;
bool monkey = false;

WiFiClient espClient;
PubSubClient client(mqtt_server, mqtt_port,mycallback,espClient);

//
int statusCode = 0;
int field_values[arr_size] = {0,0,0,0};
int score_values[2] = {0,0};

void setup() {

 //Initialize serial and wait for port to open
  Serial.begin(115200);  
  
  while(!Serial){
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  Serial.println("Serial connection ok");

  setup_wifi();

  lastMillis = millis();
}

void loop() {


   try_reconnect();

}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  //Serial.println();
 // Serial.print("Connecting to ");
  //Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("WiFi off");
  }
   Serial.println("WiFi on");
  //Serial.println("");
  //Serial.println("WiFi connected");
  //Serial.println("IP address: ");
  //Serial.println(WiFi.localIP());
}





//try to reconnect
void try_reconnect(){
    if (!client.connected()) {
    reconnect();
  }
 // publishTopic();
  client.loop();
  delay(150);
  
  }





void publishTopic(){
  
  if(Serial.available()>0){
      int index = 0;
      int last_index= 0;
      int last_i = 0;
      int index_i = 0;
      char bfr[50];
      memset(bfr,0, 50);
      Serial.readBytesUntil( '\n',bfr,49);
      
      if(client.connected()){
          client.publish(mqtt_publish_topic,bfr);
          Serial.println("GameOvePublished ");
          Serial.println(bfr);
          }
      }
}
     
//Callback for the mqqt

void mycallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  char mdata[length] ;
  char fields[arr_size];
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    mdata[i] = (char)payload[i];
  }
  
  String data = String(mdata);
  //fields[4] = strtok(data, ",");
  int index = 0;
  int last_index= 0;
  int last_i = 0;
   int index_i = 0;
  Serial.println();
  if(String(topic).equals(String(mqtt_custom_data))){

    /*For loop which will separate the String in parts
    and assign them the the variables we declared*/
    Serial.println("custom data "+data.substring(0,length));//12
    for (int i = 0; i < length; i++) {
      if (data.substring(i, i+1) == ",") {
        field_values[index] = data.substring(last_index, i).toInt();
        index++;
        last_index=i+1;
      }
       if (i == length - 1) {
              // Grab the last part of the string from the lastIndex to the end
              field_values[index] = data.substring(last_index, i+1).toInt();
           }
    }
  }else if(String(topic).equals(String(mqtt_custom_flag))){
    flag_custom = data.toInt();
    Serial.println("Flag custom "+String(flag_custom));//12
    
  }else if(String(topic).equals(String(mqtt_game_flag))){
    flag_game = data.toInt();
    Serial.println("Flag game "+String(flag_game));//11
    
  }else if(String(topic).equals(String(mqtt_game_data))){
    Serial.println("score game "+data.substring(0,length));//11
    for (int i = 0; i < length; i++) {
      if (data.substring(i, i+1) == ",") {
        score_values[index_i] = data.substring(last_i, i).toInt();
        index_i++;
        last_i=i+1;
      }
       if (i == length-1) {
              // Grab the last part of the string from the lastIndex to the end
              score_values[index_i] = data.substring(last_i, i+1).toInt();
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
          game_index = 10;
          winner = MONKEY;
          if(client.connected());
          client.publish(mqtt_publish_topic,"0");
          }
      }else if(diff>=3 && monkey==false){
        game_index++ ;
        if (game_index>=20){
          game_index = 10;
          winner = GIRAFFE;
          if(client.connected());
          client.publish(mqtt_publish_topic,"1");
          }
           
        }
    
    
    } 
    else if(String(topic).equals(String(mqtt_camera_data))){
    int value = data.toInt();
    if(value==0){
      badGesture = true;
      gesture =0;
      }else{
       gesture = value;
       badGesture = false;
        }
       Serial.println("dataGesture "+String(gesture));//12
  }else if(String(topic).equals(String(mqtt_onOroscopePage))){

      flagOroscope = data.toInt();
      Serial.println("Flag oroscope "+String(flagOroscope));//14
    
    }else if(String(topic).equals(String(mqtt_sign))){
      valueSign = data.toInt();
      Serial.println("sign "+String(valueSign));//5
      
    }
      data = "";
      index = 0;
      last_index = 0;
      last_i = 0;
      index_i = 0;
    //   Serial.println("Game index: "+game_index);
  Serial.println();
  //Serial.println(field_values[0]);
  //Serial.println(field_values[1]);
  //Serial.println(field_values[2]);
  //Serial.println(field_values[3]);
  //Serial.println("flag_custom: "+String(flag_custom));
  //Serial.println("flag_game: "+String(flag_game));
  Serial.println("Score Giraffe: "+String(sGiraffe));
  Serial.println("Score Monkey: "+String(sMonkey));

}


//reconnect to mqqt server
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(mqtt_clientID,mqtt_username,mqtt_password)) {
      Serial.println("connected");

      //Subscribe to topics
      client.subscribe(mqtt_custom_flag);
      client.subscribe(mqtt_game_flag);
      client.subscribe(mqtt_game_data);
      client.subscribe(mqtt_custom_data);
      client.subscribe(mqtt_camera_data);
      client.subscribe(mqtt_onOroscopePage);
      client.subscribe(mqtt_sign);

    } else {
      //Serial.print("failed, rc=");
      //Serial.print(client.state());
      //Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      Serial.println("Broker off");
      delay(3000);
    }
  }
}
