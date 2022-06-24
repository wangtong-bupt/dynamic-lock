#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <U8glib.h>
#include <IRremote.h>//引入红外解码库
#define INTERVAL_LCD             20             //定义OLED刷新时间间隔  
unsigned long lcd_time = millis();                 //OLED刷新时间计时器
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);     //设置OLED型号  
//-------字体设置，大、中、小
#define setFont_L u8g.setFont(u8g_font_7x13)
#define setFont_M u8g.setFont(u8g_font_fixed_v0r)
#define setFont_S u8g.setFont(u8g_font_fixed_v0r)
#define setFont_SS u8g.setFont(u8g_font_fub25n)
int RECV_PIN = 7;   //红外线接收器OUTPUT端接在pin 7
IRrecv irrecv(RECV_PIN);  //定义IRrecv对象来接收红外线信号
decode_results results;   //解码结果放在decode_results构造的对象results里
//引入ESP8266.h头文件，建议使用教程中修改后的文件
#include "ESP8266.h"
#include "SoftwareSerial.h"


//配置ESP8266WIFI设置
#define SSID "HUAWEI-9KDQST_HiLink"    //填写2.4GHz的WIFI名称，不要使用校园网
#define PASSWORD "ly20021018"//填写自己的WIFI密码
#define HOST_NAME "api.heclouds.com"  //API主机名称，连接到OneNET平台，无需修改
#define DEVICE_ID "963091773"       //填写自己的OneNet设备ID
#define HOST_PORT (80)                //API端口，连接到OneNET平台，无需修改
String APIKey = "0a0D3UwkuGG4UDr1bwpxgMz39UA="; //与设备绑定的APIKey

SoftwareSerial mySerial(3, 2);
ESP8266 wifi(mySerial);

void setup(){
  Serial.begin(9600); //初始化串口，9600baud
  irrecv.enableIRIn(); // 启动红外解码  
  mySerial.begin(115200); //初始化软串口
  Serial.print("setup begin\r\n");

  //以下为ESP8266初始化的代码
  Serial.print("FW Version: ");
  Serial.println(wifi.getVersion().c_str());

  if (wifi.setOprToStation()) {
    Serial.print("to station ok\r\n");
  } else {
    Serial.print("to station err\r\n");
  }

  //ESP8266接入WIFI
  if (wifi.joinAP(SSID, PASSWORD)) {
    Serial.print("Join AP success\r\n");
    Serial.print("IP: ");
    Serial.println(wifi.getLocalIP().c_str());
  } else {
    Serial.print("Join AP failure\r\n");
  }

  mySerial.println("AT+UART_CUR=9600,8,1,0,0");
  mySerial.begin(9600);
  Serial.println("setup end\r\n");
 
}  
  int password=random(1000,9999);  
  int a=password/1000;
  int b=password/100-10*a;
  int c=password/10-100*a-10*b;
  int d=password%10;
  int arr[4]={a,b,c,d};
  

void loop(){
 
    Serial.print("password: ");
    Serial.println(password);



    if (wifi.createTCP(HOST_NAME, HOST_PORT)) { //建立TCP连接，如果失败，不能发送该数据
      Serial.print("create tcp ok\r\n");
      char buf[10];
      //拼接发送data字段字符串
      String 
      jsonToSend = "{\"password\":";
      dtostrf(password,1,0,buf);
      jsonToSend += "\"" + String(buf) + "\"";
      jsonToSend += "}";
      
      //拼接POST请求字符串
      String postString = "POST /devices/";
      postString += DEVICE_ID;
      postString += "/datapoints?type=3 HTTP/1.1";
      postString += "\r\n";
      postString += "api-key:";
      postString += APIKey;
      postString += "\r\n";
      postString += "Host:api.heclouds.com\r\n";
      postString += "Connection:close\r\n";
      postString += "Content-Length:";
      postString += jsonToSend.length();
      postString += "\r\n";
      postString += "\r\n";
      postString += jsonToSend;
      postString += "\r\n";
      postString += "\r\n";
      postString += "\r\n";
      const char *postArray = postString.c_str(); //将str转化为char数组

      Serial.println(postArray);
      wifi.send((const uint8_t *)postArray, strlen(postArray)); //send发送命令，参数必须是这两种格式，尤其是(const uint8_t*)
      Serial.println("send success");
      if (wifi.releaseTCP()) { //释放TCP连接
        Serial.print("release tcp ok\r\n");
      } else {
        Serial.print("release tcp err\r\n");
      }
      postArray = NULL; //清空数组，等待下次传输数据
    } else {
      Serial.print("create tcp err\r\n");
    }
 

  if (irrecv.decode(&results)) {      //解码成功，收到一组红外线信号
    
      switch(results.value){
             case 16738455 :results.value=0;Serial.println(results.value);break;
             case 16724175 :results.value=1;Serial.println(results.value);break;
             case 16718055 :results.value=2;Serial.println(results.value);break;
             case 16743045 :results.value=3;Serial.println(results.value);break;
             case 16716015 :results.value=4;Serial.println(results.value);break;
             case 16726215 :results.value=5;Serial.println(results.value);break;
             case 16734885 :results.value=6;Serial.println(results.value);break;
             case 16728765 :results.value=7;Serial.println(results.value);break;
             case 16730805 :results.value=8;Serial.println(results.value);break;
             case 16732845 :results.value=9;Serial.println(results.value);break;
             }
         
    for(int i=0;i<4;i++){
      if(arr[i]==results.value){
                u8g.firstPage();//u8glib规定写法
    
           do {
                setFont_L;
                u8g.setPrintPos(4, 16);//选择位置
                u8g.print("密码正确");//写入字符串
              }while( u8g.nextPage() );//u8glib规定写法 
          irrecv.resume();  //接收下一个值
        }     else{ do {
                setFont_L;
                u8g.setPrintPos(4, 16);//选择位置
                u8g.print("no");//写入字符串
              }while( u8g.nextPage() );//u8glib规定写法 
    }
     irrecv.resume(); //  接收下一个值
      }
  }
}
