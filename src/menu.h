#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <logo.h>
#include <Preferences.h>
#include <encoderHandling.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


Preferences prefs;

//Genereal Menu Navigation variable: 0 = Home, 1 = Menu, 2 = IP, 3 = Universe, 4 = SSID, 5 = Password
byte programPosition = 0;

byte ipPosition = 0;
byte uniPosition = 0;
byte ssidPosition = 0;
String ssidString [] = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z","0","1","2","3","4","5","6","7","8","9"};
String ssidArray[10];
byte CharPos = 0;
byte PreCharPos = 0;
byte displayIndex = 0;


byte ip1, ip2, ip3, ip4; // or whatever type these variables are supposed to be

//Pref variables
int uni;
String ssid;
String pwd;


void menuBegin(){
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
        Serial.println(F("SSD1306 allocation failed"));
        for(;;);
    }
    display.display();
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.drawBitmap(0, 0, epd_bitmap_Luzi_Thing, 128, 64, WHITE);
    display.display();
    delay(3000);
    display.clearDisplay();
    display.display();

    startKnob();

    prefs.begin("menu", false);
    //IPAddress ip = prefs.getUInt("ip", IPAddress(192,168,0,1));
    //IPAddress gateway = prefs.getUInt("gateway", IPAddress(192,168,0,1));
    //IPAddress subnet = prefs.getUInt("subnet",IPAddress(255,255,255,0));

    //Workaround for the IP
    ip1 = prefs.getUInt("ip1",192);
    ip2 = prefs.getUInt("ip2",168);
    ip3 = prefs.getUInt("ip3",0);
    ip4 = prefs.getUInt("ip4",22);
    IPAddress ip(ip1,ip2,ip3,ip4);
    
    uni = prefs.getUInt("uni",0);
    ssid = prefs.getString("ssid","YourSSID"); //Mögliche Schwachstelle weil string müsste char* sein
    pwd = prefs.getString("pwd","YourPW");
    prefs.end();

}

void setIp(){

    ipFlag = true;//Change the counter to IP (0-255)
    menuFlag = false;

    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(30,0);
    display.println("Set IP");
    display.setTextSize(1);
    if(ipPosition == 0){
        display.setCursor(15,35);
        display.print(ipCount);
        display.print(".");
        display.print(ip2);
        display.print(".");
        display.print(ip3);
        display.print(".");
        display.println(ip4);
        ip1 = ipCount;
    }else if(ipPosition == 1){
        display.setCursor(15,35);
        display.print(ip1);
        display.print(".");
        display.print(ipCount);
        display.print(".");
        display.print(ip3);
        display.print(".");
        display.println(ip4);
        ip2 = ipCount;
    }else if(ipPosition == 2){
        display.setCursor(15,35);
        display.print(ip1);
        display.print(".");
        display.print(ip2);
        display.print(".");
        display.print(ipCount);
        display.print(".");
        display.println(ip4);
        ip3 = ipCount;
    }else if(ipPosition == 3){
        display.setCursor(15,35);
        display.print(ip1);
        display.print(".");
        display.print(ip2);
        display.print(".");
        display.print(ip3);
        display.print(".");
        display.println(ipCount);
        ip4 = ipCount;
    }

    display.display();

    if(ipPosition == 4){
        programPosition = 1;
        ipPosition = 0;
            ipFlag = !ipFlag;//Change the counter to menu (0-4)
            menuFlag = !menuFlag;
    }
    if(buttonState == true){
        ipPosition++;
    }
}

void setUni(){
    uniFlag = true;
    menuFlag = false;
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(15,0);
    display.println("Universe");
    if(uniPosition == 0){
        display.setTextSize(2);
        display.setCursor(55,30);
        display.println(uniCount);
        uni = uniCount;
    }
    display.display();

    if(uniPosition == 1){
        programPosition = 1;
        uniPosition = 0;
        uniFlag = !uniFlag;
        menuFlag = !menuFlag;
    }
        if(buttonState == true){
        uniPosition++;
    }
}

void setSsid(){
    ssidFlag = true;
    menuFlag = false;
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0,0);
    display.println("Set SSID");

    for(int i = 0; i <= 9; i++){        //Textvorschau der SSID
        display.setCursor(PreCharPos,30);
        display.print(ssidArray[i]);
        PreCharPos += 12;
        if(i == 9){
            PreCharPos = 0;
        }
    }

    for(displayIndex; displayIndex < 9; ){        //Befüllen des Arrays
    display.setCursor(CharPos,30);
    display.print(ssidString[ssidCount]);

    if(ssidCount == 37){
        display.setCursor(0,45);
        display.setTextSize(2);
        display.print("Cancel");  
        display.setTextSize(1); 
        display.setCursor(90,45);     
        display.println("Set");
        
        if(buttonState == true){
            saveFlag = !saveFlag;
            menuFlag = !menuFlag;
            programPosition = 1;
        }
    }else if(ssidCount == 38){

        display.setCursor(0,45);
        display.setTextSize(1);
        display.print("Cancel");  
        display.setTextSize(2); 
        display.setCursor(90,45);     
        display.println("Set");
        
        if(buttonState == true){
            saveFlag = !saveFlag;
            menuFlag = !menuFlag;
            programPosition = 1;   
        }
    }
    if(buttonState == true){

        CharPos = CharPos + 12;
        ssidArray[displayIndex] = ssidString[ssidCount];
        displayIndex++;
        
    }
    break;
    }
    display.display();
}

void setPw(){
    pwFlag = true;
    menuFlag = false;
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0,0);
    display.println("Set Password");
    display.display();
        if(buttonState == true){
        programPosition = 1;
        pwFlag = !pwFlag;
        menuFlag = !menuFlag;
    }
}

void SaveAndRestart(){
    saveFlag = true;
    menuFlag = false;
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(37,0);
    display.println("Save");
    display.setCursor(12,20);
    display.setTextSize(1);
    display.println("Node will restart!");
    display.setCursor(25,30);
    display.println("Are you sure?");

    if(saveCount == 0){

        display.setCursor(0,45);
        display.setTextSize(2);
        display.print("No");  
        display.setTextSize(1); 
        display.setCursor(90,45);     
        display.println("Yes");
        display.display();
        if(buttonState == true){
            saveFlag = !saveFlag;
            menuFlag = !menuFlag;
            programPosition = 1;
        }
    }else if(saveCount == 1){

        display.setCursor(0,45);
        display.setTextSize(1);
        display.print("No");  
        display.setTextSize(2); 
        display.setCursor(90,45);     
        display.println("Yes");
        display.display();
        if(buttonState == true){
            prefs.begin("menu", false);
            prefs.putUInt("ip1",ip1);
            prefs.putUInt("ip2",ip2);
            prefs.putUInt("ip3",ip3);
            prefs.putUInt("ip4",ip4);
            prefs.putUInt("uni",uni);
            prefs.putString("ssid","YourSSID");
            prefs.putString("pwd","YourPW");
            prefs.end();
            display.clearDisplay();
            display.setTextSize(2);
            display.setCursor(20,25);
            display.println("Reboot...");
            display.display();
            delay(2000);
            ESP.restart();
        }
    }
}


void mainMenu(){
    //Serial.println(menuCounter);
if(menuCounter == 0){
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(2);
    display.println("IP Address");
    display.setTextSize(1);
    display.println("Universe");
    display.println("SSID");
    display.println("Password");
    display.println("Save");
    display.println("Back");
    display.display();
    if(buttonState == true){
        programPosition = 2;
    }
}else if(menuCounter == 1){
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(1);
    display.println("IP Address ");
    display.setTextSize(2);
    display.println("Universe");
    display.setTextSize(1);
    display.println("SSID");
    display.println("Password");
    display.println("Save");
    display.println("Back");
    display.display();
        if(buttonState == true){
        programPosition = 3;
    }
}else if(menuCounter == 2){
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(1);
    display.println("IP Address ");
    display.println("Universe");
    display.setTextSize(2);
    display.println("SSID");
    display.setTextSize(1);
    display.println("Password");
    display.println("Save");
    display.println("Back");
    display.display();
    if(buttonState == true){
        programPosition = 4;
    }
}else if(menuCounter == 3){
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(1);
    display.println("IP Address ");
    display.println("Universe");
    display.println("SSID");
    display.setTextSize(2);
    display.println("Password");
    display.setTextSize(1);
    display.println("Save");
    display.println("Back");
    display.display();
    if(buttonState == true){
        programPosition = 5;
    }
}else if(menuCounter == 4){
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(1);
    display.println("IP Address ");
    display.println("Universe");
    display.println("SSID");
    display.println("Password");
    display.setTextSize(2);
    display.println("Save");
    display.setTextSize(1);
    display.println("Back");
    display.display();
    if(buttonState == true){
        programPosition = 6;
    }
}else if(menuCounter == 5){
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(1);
    display.println("IP Address ");
    display.println("Universe");
    display.println("SSID");
    display.println("Password");
    display.println("Save");
    display.setTextSize(2);
    display.println("Back");
    display.display();
    if(buttonState == true){
        programPosition = 0;
    }
}
}


void homescreen(){
    display.clearDisplay();
    display.setTextSize(1.5);
    display.setCursor(0,0);
    display.print("IP: ");
    display.print(ip1);
    display.print(".");
    display.print(ip2);
    display.print(".");
    display.print(ip3);
    display.print(".");
    display.println(ip4);
    display.print("Uni: ");
    display.println(uni);
    display.print("SSID: ");
    display.println(ssid);
    display.display();
}

//Main Handler
void menuExec(){

readBtn(btnInterval);//Read the button every cicle  

switch (programPosition)
{ case 0:
    homescreen();
    if(buttonState == true){
        programPosition = 1;
    }
    break;
  case 1:
    mainMenu();
    break;      
  case 2:
    setIp();
    break;
  case 3:
    setUni();
    break;
  case 4:
    setSsid();
    break; 
  case 5:
    setPw();
    break; 
  case 6:
    SaveAndRestart();
    break;    

}
}

