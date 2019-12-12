//Modify the ssid and password to your own AP.
const char* ssid = "NCTUECE";
const char* password = "ece2018wifi";

//Set your own clientID.
//You can generate one with this: https://goo.gl/OezKTw or use the MAC address of the ESP8266 onboard.
//Copy and paste the string after the slash, EX:"/youOwnID".
const char* randomID = "/s9LrTwMdfD3u5fc"; 

//Add one your own channel and subscribing the corresponding topic.
//Step 1. Increase numOfChannels by 1
//Step 2. Channel's name is the alias of the topic. Set your channel's name and give it a serial number using #define marco.     EX: #define channelName 2
//Step 3. Add your own topic starting with "/"(slash) into the channel array.     EX: "/myChannel"
//Then you can receive the MQTT messages with topic "/myChannel". The topic name can be returned by using channel[channelName].

#define numOfChannels 4
#define STR 0
#define RGBLED 1
#define BREATH_PERIOD 2
#define RAINBOW_PERIOD 3

const char* channel[] {
  "/STR",
  "/RGBLED",
  "/BREATH_PERIOD",
  "/RAINBOW_PERIOD"
};

