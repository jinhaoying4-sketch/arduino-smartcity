/*
程序实现目标：
通过超声波和蜂鸣器检测后车距离，距离过近则蜂鸣器发声提示
通过dht11传感器感知环境温湿度并显示在lcd屏幕

今天可能会下雨，记得带好雨伞！
今天温度高，请注意预防中暑！
今天温度低，请穿好厚衣服！
今天可能会下雪，请留意！
今天温度适宜。
오늘 비가 올 것 같으니 우산을 잘 챙기세요!
오늘은 온도가 높으니 열사병 예방에 주의하세요!
오늘은 온도가 낮으니 두꺼운 옷을 입으세요!
오늘은 눈이 올 수 있으니 유의하세요!
오늘은 온도가 적당하다.

*/
#include <avr/wdt.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define buzzer_PIN 13
#define DHT_PIN A0
#define DHT_TYPE DHT11
#define LCD1602_I2C 0x27
#define LCD1602_w 16
#define LCD1602_h 2
#define echoPin 3
#define trigPin 4
#define button_PIN 5
#define trafficLightRed_PIN 6
#define trafficLightYellow_PIN 7
#define trafficLightGreen_PIN 8
#define pepoeTrafficLightRed_PIN 9
#define pepoeTrafficLightGreen_PIN 10
bool errordata[8] = {
  false,
  false,
  false,
  false,
  false,
  false,
  false,
  false
};
//byte* charCollection[] = {cloudWeather, coffeeCup, musicNote, bellAlert, heartShape, starIcon, lockSymbol, wifiSignal};
float celsius, humidity, DHTtime=0, rebootTime=0;
String text = "Welcome to the smart city!";
String errorText="";
String errorSize="";
String tempText = text;
unsigned long textPrintTime = 0,errorTime=0,runTime=0,sleepTime=0;
int textPrint = 0,dhtErrorSun=0,lcdEnd = 11,SR04ErrorSun=0,systemErrorSun=0;
int ballX=0,ballY=0,ballRx=2,ballRy=1,tempX=0,tempY=0;
//以下是文本“스마트 시티”的文字建模
byte seu[8] = {  //스
  0b00000,
  0b00000,
  0b00100,
  0b01010,
  0b10001,
  0b00000,
  0b11111,
  0b00000
};
byte ma[8] = {
  //마
  0b00000,
  0b00010,
  0b11110,
  0b10111,
  0b10110,
  0b11110,
  0b00010,
  0b00000,
};
byte teu[8] = {  //트
  0b01111,
  0b10000,
  0b11110,
  0b10000,
  0b11111,
  0b00000,
  0b11111,
  0b00000
};
byte si[8] = {  //시
  0b00000,
  0b00001,
  0b01001,
  0b01001,
  0b10101,
  0b10101,
  0b00001,
  0b00000
};
byte ti[8] = {  //티
  0b00000,
  0b00001,
  0b01101,
  0b10001,
  0b11101,
  0b10001,
  0b11101,
  0b00001
};
//以下是温湿度：
byte on[8] = {
  0b01110,
  0b10001,
  0b01110,
  0b00100,
  0b11111,
  0b00000,
  0b10000,
  0b11111
};
byte seup[8] = {
  0b00100,
  0b01010,
  0b11111,
  0b00000,
  0b01010,
  0b01110,
  0b01010,
  0b01110
};
byte Do[8] = {
  0b01111,
  0b10000,
  0b11111,
  0b10000,
  0b11111,
  0b00000,
  0b00100,
  0b11111
};
//以下是符号：
byte printCelsius[8] = {  //℃
  0b10000,
  0b00111,
  0b01000,
  0b01000,
  0b01000,
  0b01000,
  0b01000,
  0b00111
};
byte printHumidity[8] = {  //φ(环境湿度phi)
  0b00000,
  0b01000,
  0b10010,
  0b10101,
  0b01110,
  0b00100,
  0b00100,
  0b00000
};
byte warning[8] = {  //警告三角符号
  0b00100,
  0b00100,
  0b01010,
  0b01010,
  0b11011,
  0b11111,
  0b11011,
  0b11111
};
byte rani1[8] = {  //下雨标
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b00010,
  0b01000,
  0b01010,
  0b00000
};
byte rani2[8] = {  //下雨标2
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b01000,
  0b00010,
  0b01010,
  0b00000
};
byte hot1[8] = {  //火
  0b00100,
  0b00100,
  0b01010,
  0b01010,
  0b01010,
  0b10101,
  0b10101,
  0b01110
};
byte hot2[8] = {  //火
  0b00000,
  0b00000,
  0b00100,
  0b01010,
  0b01010,
  0b10001,
  0b10101,
  0b01110
};
byte cold1[8] = {  //下雪标
  0b00000,
  0b00000,
  0b10101,
  0b01110,
  0b11011,
  0b01110,
  0b10101,
  0b00000
};
byte cold2[8] = {  //下雪标
  0b00000,
  0b10101,
  0b01110,
  0b11011,
  0b01110,
  0b10101,
  0b00000,
  0b00000
};
byte shenglue[8] = {//...
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b10101
};
byte happy[8] = {//笑脸
  0b00000,
  0b01010,
  0b01010,
  0b00000,
  0b00000,
  0b10001,
  0b01110,
  0b00000
};
byte unhappy1[8] = {//笑脸
  0b00000,
  0b01010,
  0b01010,
  0b00000,
  0b00000,
  0b01010,
  0b10101,
  0b00000
};
byte unhappy2[8] = {//笑脸
  0b00000,
  0b00000,
  0b01010,
  0b01010,
  0b00000,
  0b00000,
  0b01010,
  0b10101
};
byte ball2222[8] = {
  0b00000,
  0b10000,
  0b00100,
  0b00000,
  0b00100,
  0b00000,
  0b00000,
  0b00000
};
byte dai[8]{//대
  0b00101,
  0b11101,
  0b10101,
  0b10111,
  0b10101,
  0b10101,
  0b11101,
  0b00101
};
byte gi[8]{//기
  0b00001,
  0b11101,
  0b00101,
  0b00101,
  0b00101,
  0b01001,
  0b10001,
  0b00001
};
byte zung[8]{//중
  0b11111,
  0b00100,
  0b11011,
  0b11111,
  0b00100,
  0b01110,
  0b10001,
  0b01110
};
void wre(){
  wdt_reset();
}
LiquidCrystal_I2C lcd(LCD1602_I2C, LCD1602_w, LCD1602_h);
DHT dht(DHT_PIN, DHT_TYPE);
void print_ball(int x, int y){//待机弹球
  // lcd.setCursor(0,0);
  // lcd.print("                ");
  // lcd.setCursor(0,1);
  // lcd.print("            ");
  if(!((x/5==tempX)&&(y/8==tempY))){
    lcd.setCursor(tempX,tempY);
    lcd.print(" ");
  }
  for(int i = 0;i < 8;i++){
    ball2222[i]=0b00000;
    if(y%8==i) ball2222[i]=(1<<4-(x%5));
  }
  lcd.createChar(0,ball2222);
  lcd.setCursor(x/5,y/8);
  tempX=x/5;
  tempY=y/8;
  lcd.write(0);
}
float HC_SR04(){//距离测算（返回值单位：厘米）
  if(errordata[0]==true) return -1;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long i = pulseIn(echoPin, HIGH,30000);
  if(i==0) {
    SR04ErrorSun++;
    return -1;
  }
  SR04ErrorSun=0;
  float cm = i * 0.034 / 2;
  return cm;
}
void handleScrollDisplay() {//公告栏字符滚动
  if ((millis() - textPrintTime) >= 300) {
    String temp2Text=tempText+" ";
    for(int i=0;i<lcdEnd;i++){
      temp2Text=" "+temp2Text;
    }
    int textSize = temp2Text.length();
    lcd.setCursor(0, 1);
    lcd.print(temp2Text.substring(textPrint,textPrint+lcdEnd));
    if((text!=tempText)&&(textPrint >=textSize-1)){
      textPrint = 0;
      tempText=text;
    }
    textPrint++;
    textPrint = (int)textPrint%textSize;
    textPrintTime = millis();
  }
}
void lcdRefresh() {//显示温湿度
  lcd.setCursor(0, 0);
  lcd.write(0);
  lcd.write(2);
  lcd.print(":");
  lcd.print((int)celsius);
  lcd.write(3);
  lcd.print(" ");
  lcd.setCursor(7, 0);
  lcd.write(1);
  lcd.write(2);
  lcd.print(":");
  lcd.print((int)humidity);
  lcd.print("%");
  if ((int)humidity < 100) lcd.print(" ");
  else if ((int)humidity < 10) lcd.print("  ");
}
void weatherForecast(){//显示天气状态
  float prob=-1000;
  if (humidity>=80&&celsius<=-2){
    prob = (humidity-80)/20*100;
    if(millis()%1000>=500) lcd.createChar(4, cold1);
    else lcd.createChar(4, cold2);
    text = "Chance of snow. See status bar.";
  }else if(humidity>=80&&celsius<=30){
    prob = (humidity-80)/20*100;
    if(millis()%1000>=500) lcd.createChar(4, rani1);
    else lcd.createChar(4, rani2);
    text = "Chance of rain. See status bar.";
  }else if(celsius<20){
    if(millis()%1000>=500) lcd.createChar(4, unhappy1);
    else lcd.createChar(4, unhappy2);
    text = "Chilly today. Bundle up! :(";
  }else if(celsius>26){
    if(millis()%1000>=500) lcd.createChar(4, hot1);
    else lcd.createChar(4, hot2);
    text = "High temperature. Beware of heatstroke.";
  }else{
    lcd.createChar(4, happy);
    text = "The weather is pleasant. Welcome to the Smart City.";
  }
  lcd.setCursor(14, 0);
  if(humidity<=999) lcd.print(" ");
  lcd.write(4);
  lcd.print("    ");
  lcd.setCursor(12, 1);
  if(prob==-1000){
    lcd.print(" --%");
  }else{
    if(prob<=9) lcd.print("  ");
    else if(prob<=99) lcd.print(" ");
    lcd.print((int)prob);
    lcd.print("%   ");
  }
}
void DHTrefresh(){//更新温湿度
  humidity = dht.readHumidity();
  celsius = dht.readTemperature();
}
void trafficLight(){
  float time = millis()%20000;
  if(time<=4500+3000){
    digitalWrite(trafficLightRed_PIN,LOW);
    digitalWrite(trafficLightYellow_PIN,LOW);
    digitalWrite(trafficLightGreen_PIN,HIGH);
    digitalWrite(pepoeTrafficLightRed_PIN,HIGH);
    digitalWrite(pepoeTrafficLightGreen_PIN,LOW);
  }else if(time<=5000+3000){
    digitalWrite(trafficLightRed_PIN,LOW);
    digitalWrite(trafficLightYellow_PIN,HIGH);
    digitalWrite(trafficLightGreen_PIN,LOW);
    digitalWrite(pepoeTrafficLightRed_PIN,HIGH);
    digitalWrite(pepoeTrafficLightGreen_PIN,LOW);
  }else if(time<=5500+3000){
    digitalWrite(trafficLightRed_PIN,LOW);
    digitalWrite(trafficLightYellow_PIN,LOW);
    digitalWrite(trafficLightGreen_PIN,LOW);
    digitalWrite(pepoeTrafficLightRed_PIN,HIGH);
    digitalWrite(pepoeTrafficLightGreen_PIN,LOW);
  }else if(time<=6000+3000){
    digitalWrite(trafficLightRed_PIN,LOW);
    digitalWrite(trafficLightYellow_PIN,HIGH);
    digitalWrite(trafficLightGreen_PIN,LOW);
    digitalWrite(pepoeTrafficLightRed_PIN,HIGH);
    digitalWrite(pepoeTrafficLightGreen_PIN,LOW);
  }else if(time<=6500+3000){
    digitalWrite(trafficLightRed_PIN,LOW);
    digitalWrite(trafficLightYellow_PIN,LOW);
    digitalWrite(trafficLightGreen_PIN,LOW);
    digitalWrite(pepoeTrafficLightRed_PIN,HIGH);
    digitalWrite(pepoeTrafficLightGreen_PIN,LOW);
  }else if(time<=7000+3000){
    digitalWrite(trafficLightRed_PIN,LOW);
    digitalWrite(trafficLightYellow_PIN,HIGH);
    digitalWrite(trafficLightGreen_PIN,LOW);
    digitalWrite(pepoeTrafficLightRed_PIN,HIGH);
    digitalWrite(pepoeTrafficLightGreen_PIN,LOW);
  }else if(time<=7500+3000){
    digitalWrite(trafficLightRed_PIN,LOW);
    digitalWrite(trafficLightYellow_PIN,LOW);
    digitalWrite(trafficLightGreen_PIN,LOW);
    digitalWrite(pepoeTrafficLightRed_PIN,HIGH);
    digitalWrite(pepoeTrafficLightGreen_PIN,LOW);
  }else{
    digitalWrite(trafficLightRed_PIN,HIGH);
    digitalWrite(trafficLightYellow_PIN,LOW);
    digitalWrite(trafficLightGreen_PIN,LOW);
    digitalWrite(pepoeTrafficLightRed_PIN,LOW);
    digitalWrite(pepoeTrafficLightGreen_PIN,HIGH);
  }
}
void errorChecking(){
  if(millis()-runTime>=100){
    systemErrorSun++;
  }else{
    systemErrorSun=0;
  }
  if(systemErrorSun>=3){
    errorText="system error! The system will restart in 45 seconds.";
    errorSize="system";
    rebootTime=45000;
    error();
  }
  if(millis()-errorTime>=1000){
    errorTime=millis();
    if(isnan(humidity) || isnan(celsius) || (humidity == 0.0 && celsius == 0.0) ){
      dhtErrorSun++;
    }
    else{
      dhtErrorSun=0;
    }
    if(dhtErrorSun>=5){
      errorText="Module DHT11/22 Error";
      errorSize="DHT11";
      rebootTime=10000;
      miniError();
    }
  }
  if(SR04ErrorSun>=100){
    errorText="SR04 Error.";
    errorSize="SR04";
    rebootTime=10000;
    miniError();
  }
  runTime=millis();
}
void error(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcdEnd = 16;
  lcd.print("[E]");
  lcd.print(errorSize);
  unsigned long timesssss = millis();
  textPrint = 0;
  // errordata = true;
  text=errorText;
  tempText=errorText;
  while(millis()-timesssss<=rebootTime){
    int Delay=(int)((rebootTime-(millis()-timesssss))/1000);
    lcd.setCursor(14, 0);
    if(Delay<10) lcd.print(" ");
    lcd.print(Delay);
    handleScrollDisplay();
    wre();
  }
  lcd.createChar(7,shenglue);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("restart");
  lcd.write(7);
  wdt_enable(WDTO_30MS);
  lcd.clear();
  while(true){}
}
void miniError(){
  lcdEnd = 16;
  // errordata = true;
  if(tempText==errorText){
    text=errorText;
    tempText=errorText; 
    lcd.setCursor(10, 1);
    lcd.print("      ");
  }else{
    lcd.clear();
    textPrint = 0;
    text=errorText;
    tempText=errorText;
  }
  lcd.setCursor(0, 0);
  lcd.print("[mini-E]");
  lcd.print(errorSize);
  unsigned long timesssss = millis();
  while(millis()-timesssss<=rebootTime){
    int Delay=(int)((rebootTime-(millis()-timesssss))/1000);
    lcd.setCursor(14, 0);
    if(Delay<10) lcd.print(" ");
    lcd.print(Delay);
    handleScrollDisplay();
    wre();
  }
  lcdEnd = 11;
  lcd.setCursor(10, 1);
  lcd.print("     ");
  runTime=millis();
  systemErrorSun=0;
  return;
}
void daiji() {//待机
  digitalWrite(trafficLightRed_PIN,LOW);
  digitalWrite(trafficLightYellow_PIN,LOW);
  digitalWrite(trafficLightGreen_PIN,LOW);
  digitalWrite(pepoeTrafficLightRed_PIN,LOW);
  digitalWrite(pepoeTrafficLightGreen_PIN,LOW);
  lcd.clear();
  lcd.createChar(0, ball2222);
  lcd.createChar(1, dai);
  lcd.createChar(2, gi);
  lcd.createChar(3, zung);
  lcd.setCursor(12, 1);
  lcd.write(1);
  lcd.write(2);
  lcd.print(" ");
  lcd.write(3);//显示“대기 중”
  unsigned long htime = millis();
  while(HC_SR04()>30){
    if(htime+125>millis()) continue;
    ballX+=ballRx;
    ballY+=ballRy;
    if(ballX<=0||ballX>=80-1||(ballY>=8-1&&ballX>=60-1&&ballX<=60)) ballRx*=-1;
    if(ballY<=0||ballY>=16-1||(ballY>=8-1&&ballX>=60-1&&ballY<=8)) ballRy*=-1;
    print_ball(ballX,ballY);
    lcd.setCursor(12, 1);
    lcd.write(1);
    lcd.write(2);
    lcd.print(" ");
    lcd.write(3);
    htime = millis();
    wre();
  }
  lcd.clear();
  lcd.createChar(0, on);
  lcd.createChar(1, seup);
  lcd.createChar(2, Do);
  lcd.createChar(3, printCelsius);
  runTime=millis();
  lcd.backlight();
}
void setup() {
  Serial.begin(9600);
  Serial.print("[loading] ");
  wdt_disable();
  lcd.init();
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Init Screen");
  lcd.backlight();
  lcd.createChar(0, seu);
  lcd.createChar(1, ma);
  lcd.createChar(2, teu);
  lcd.createChar(3, si);
  lcd.createChar(4, ti);
  lcd.createChar(5, shenglue);
  lcd.setCursor(2, 0);
  lcd.write(0);
  lcd.write(1);
  lcd.write(2);
  lcd.print(" ");
  lcd.write(3);
  lcd.write(4);
  lcd.print("   V5.0");
  lcd.setCursor(0, 1);
  lcd.print("Init GPIO");
  lcd.write(5);
  lcd.print("          ");
  pinMode(buzzer_PIN, OUTPUT);
  //pinMode(button_PIN, OUTPUT);
  pinMode(trafficLightGreen_PIN, OUTPUT);
  pinMode(trafficLightYellow_PIN, OUTPUT);
  pinMode(trafficLightRed_PIN, OUTPUT);
  pinMode(pepoeTrafficLightRed_PIN,OUTPUT);
  pinMode(pepoeTrafficLightGreen_PIN,OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  lcd.setCursor(0, 1);
  lcd.print("Init DHT11/22");
  lcd.write(5);
  lcd.print("          ");
  dht.begin();
  lcd.setCursor(0, 1);
  lcd.print("Read DHT11 data");
  lcd.write(5);
  lcd.print("          ");
  delay(2000);
  DHTrefresh();
  lcd.setCursor(0, 1);
  lcd.print("start main");
  lcd.print("          ");
  lcd.createChar(0, shenglue);
  lcd.write(0);
  lcd.clear();
  lcd.noBacklight();
  delay(10);
  lcd.backlight();
  lcd.createChar(0, on);
  lcd.createChar(1, seup);
  lcd.createChar(2, Do);
  lcd.createChar(3, printCelsius);
  lcdRefresh();
  DHTtime = millis();
  runTime = millis();
  wdt_enable(WDTO_2S);
}

void loop() {
  // put your main code here, to run repeatedly:
  float Distance = HC_SR04();
  if (millis() - DHTtime >= 1000) {
    Serial.println(Distance);
    DHTtime = millis();
  }
  if(Distance<60)sleepTime=millis();
  if(Distance<=5&&Distance!=-1) tone(buzzer_PIN, 4500);
  else noTone(buzzer_PIN);//digitalWrite(buzzer_PIN,LOW);
  if(millis()-sleepTime>30000) daiji();
  DHTrefresh();
  weatherForecast();
  lcdRefresh();
  handleScrollDisplay();
  trafficLight();
  errorChecking();
  wre();
}
