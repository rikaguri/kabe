#include <DFRobotDFPlayerMini.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
//LED関連
const int NUM_PIXELS = 24;//LEDの数
const int DATA_PIN =2;//LEDの信号線のピン番号
//リードスイッチ関連
const int NUM_SWITCH = 24;//リードスイッチの数
const int reedsw[24]={13,12,11,10,9,8,7,6,5,4,3,22,23,24,25,26,27,28,29,30,31,32,33,34};//リードスイッチのピン番号
//看板関連
const int NUM_NEON = 20;//ネオンの数
const int PINK_PIN = 36;//ピンク看板のpin番号
const int BLUE_PIN = 37;//ブルー看板のpin番号
//スタートボタン関連
const int START_PIN=50;//スタートボタンのピン
//ゲームモード
const int MODE_GAME_START=2;//ゲーム開始した瞬間モード
const int MODE_GAME_NOW=1;//ゲーム中のモード
const int MODE_MATI=0;//ゲームしてなくて待ってるモード
// 48秒間（ミリ秒）
const unsigned long FF_SEC = 48L * 1000L;
//SE関係
SoftwareSerial mySoftwareSerial(18,19); // RX, TX
DFRobotDFPlayerMini myDFPlayer;//dfplayer使うためのやつ

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, DATA_PIN, NEO_RGB + NEO_KHZ800);//NeoPixelライブラリ使うためのやつ
Adafruit_NeoPixel pinkneon =Adafruit_NeoPixel(NUM_NEON, PINK_PIN, NEO_RGB + NEO_KHZ800);//NeoPixelライブラリ使うためのやつ
Adafruit_NeoPixel blueneon =Adafruit_NeoPixel(NUM_NEON, BLUE_PIN, NEO_RGB + NEO_KHZ800);//NeoPixelライブラリ使うためのやつ
uint32_t white = strip.Color(255, 255, 255);//白の色味
uint32_t blue = strip.Color(0, 0, 255);//青の色味
uint32_t pink = strip.Color(255, 0, 20);//ピンクの色味
uint32_t black = strip.Color(0,0,0);//黒
uint32_t violet = strip.Color(128,0,128);//紫

int blueteam=0;//ブルーチーム得点
int pinkteam=0;//ピンクチーム得点

int reedst[24]={0};//リードスイッチのステータス(0or1)

int cnt1[24]={0};//1をカウント(チャタリング対策用)
int cnt0[24]={0};//0をカウント(チャタリング対策用)

int G_mode = MODE_MATI;//モードを待ちに設定
unsigned long G_start_time;//スタートした時間を記録

const int a[8]={0,1,6,7,10,11,20,21};
const int b[8]={4,5,8,9,14,15,18,19};
const int c[8]={2,3,12,13,16,17,22,23};
  int count=0;

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show();
  pinkneon.begin();
  pinkneon.show();
  blueneon.begin();
  blueneon.show();
  for(int i=0;i<21;i++){
    pinkneon.setPixelColor(i,0,255,100);
    }
    pinkneon.show();
    for(int i=0;i<21;i++){
      blueneon.setPixelColor(i,blue);
    }
    blueneon.show();
  strip.setBrightness(255);
  //リードスイッチのpin設定
  for(int i=0;i<NUM_SWITCH;i++){
    pinMode(reedsw[i], INPUT_PULLUP);
    reed(i);
  }
  pinMode(START_PIN,INPUT);//スタートボタンpin設定
  mySoftwareSerial.begin(9600);//
  //mp3_set_serial(mySoftwareSerial);
  myDFPlayer.begin(mySoftwareSerial);
  myDFPlayer.volume(30);
}


void loop(){
  unsigned long current_time;
  switch(G_mode){
    case MODE_MATI://もし待ち状態だったら
    gamemode();//開始状態になってないか確認
    break;
    
    case MODE_GAME_START://ゲーム開始状態になったら
    blueteam=0;//得点を0に戻す
    pinkteam=0;
    for(int i=0;i<NUM_PIXELS;i++){//全部のLEDを白にする
        strip.setPixelColor(i,white);
        strip.show();
    }
    //Serial.println("音出せ~");
    myDFPlayer.play(1); //Play the first mp3
    delay(3000);
    shokika();//LEDを初期化
    G_mode=MODE_GAME_NOW;//ゲーム中にする
    break;
    
    case MODE_GAME_NOW://ゲーム中のモードだったら
    current_time=millis();//現在時刻を確認
    //Serial.print("ゲーム中だよ");
    if(current_time - G_start_time>=FF_SEC){//終了
      for(int i=0;i<NUM_PIXELS;i++){//全部のLEDを白にする
        strip.setPixelColor(i,white);
        strip.show();
      }

      //Serial.print("blue:");
      //Serial.print("fin");
      //Serial.print("\t");
      Serial.print(blueteam);
      Serial.print("\t");
      //Serial.print("pink:");
      Serial.print(pinkteam);
      Serial.println("");
      G_mode=MODE_MATI;
      delay(3000);//3秒間 
      }
    else{
    gamemain();
    }
        break;
  }
}


  


//=========================================================//
// LEDを他の色に変える関数
//=========================================================//
void ChangeLED(uint32_t color){
  for(int i=0;i<30;i++){
  int index = random(24); // index番目の白い棒の色を変える
  //Serial.println(index);
  if(strip.getPixelColor(index)==white){
    strip.setPixelColor(index,color);
    if(index%2==0){
    strip.setPixelColor(index+1,color);
    }
    else strip.setPixelColor(index-1,color);
    break;
    }
  }
}
//=========================================================//
// LEDを白に変える関数
//=========================================================//
void ChangeWhite(int num){
  strip.setPixelColor(num,white);//棒を白にする
  if(num%2==0){
    strip.setPixelColor(num+1,white);
    }
    else strip.setPixelColor(num-1,white);
}
//=========================================================//
// リードスイッチの押し引き判定関数(1(true)か0(false)が返ってくるようにする)
//=========================================================//
int reed(int i){//iはリードスイッチの番号
  cnt1[i]=0;//ついてる時間をカウント
  cnt0[i]=0;//消えてる時間をカウント

  //チャタリング対策
  for(int j=0;j<20;j++){
  if(digitalRead(reedsw[i])==1){
    cnt1[i]++;
    }
  else if(digitalRead(reedsw[i])==0){
    cnt0[i]++;
    }
  }

  //ステータス更新
  if(cnt1[i]>15 && reedst[i]==0){//リードスイッチONでステータスがOFFなら
    reedst[i]=1;//ステータスをONにする
    //Serial.print(i);
    //Serial.print("がonになりました");
    return 1;
  }
  else if(cnt0[i]>15 && reedst[i]==1){//リードスイッチOFFでステータスがONなら
    reedst[i]=0;//ステータスをOFFにする
    //Serial.print(i);
    //Serial.print("がoffになりました");
    return 0;
  }
  else return 2;//別に変化がなければ2を返す
  
}
//=========================================================//
// ゲームモード選択関数
//=========================================================//
void gamemode(){
  if(digitalRead(START_PIN)==HIGH){//スタートボタンのピンを読んでONになってたら
    G_mode=MODE_GAME_START;//ゲーム開始にする
    //Serial.println("start!");
    G_start_time=millis();//ゲーム開始時間測り始める
    delay(100);
    }
  else{//待機中なのでランダムでledが光るようにする
    led();
    delay(3000);
  }
}
//=========================================================//
// 待機中のLED関数
//=========================================================//
void led(){
  switch(count){
    case 0:
    for(int i=0;i<8;i++){
    strip.setPixelColor(a[i],white);
    strip.setPixelColor(b[i],pink);
    strip.setPixelColor(c[i],blue);
    }
    count=1;
    break;
    case 1:
    for(int i=0;i<8;i++){
    strip.setPixelColor(a[i],pink);
    strip.setPixelColor(b[i],blue);
    strip.setPixelColor(c[i],white);
    }
    count=2;
    break;
    case 2:
    for(int i=0;i<8;i++){
    strip.setPixelColor(a[i],blue);
    strip.setPixelColor(b[i],white);
    strip.setPixelColor(c[i],pink);
    }
    count=0;
    break;
  }
    /*
    for(int i=0;i<NUM_PIXELS;i++){
      int x=count1[i]*5;
      switch(count2[i]){
        case 0://数字が上がっていく方
        strip.setPixelColor(i, strip.Color(255-x, 0, 0+x));
        if(count1[i]>46){
          count1[i]=51;
          count2[i]=1;
        }
        else
        count1[i]+=5;
        break;
        case 1://数字が下がっていく方
        strip.setPixelColor(i, strip.Color(x, 0, 255-x));
        if(count1[i]<5){
          count1[i]=0;
          count2[i]=0;
        }
        else
        count1[i]-=5;
        break;
      }
    }
    */
    
    strip.show();
  }
//=========================================================//
// LED初期化関数
//=========================================================//
void shokika(){
for(int i=0;i<NUM_PIXELS;i+=2){
      if(i==4||i==12||i==18||i==22){//3,7,10,12
        strip.setPixelColor(i,white);
         strip.setPixelColor(i+1,white);
      }
      else if(i==0||i==8||i==10||i==20){//1,5,6,11
        strip.setPixelColor(i,blue);
        strip.setPixelColor(i+1,blue); 
        }
        else if(i==2||i==6||i==14||i==16){//2,4,8,9
          strip.setPixelColor(i,pink);
          strip.setPixelColor(i+1,pink); 
          }
          }
           strip.show();
}

void gamemain(){
  for(int i=0;i<NUM_SWITCH;i++){
      int st=reed(i);
      if((st==1 || st==0)&& strip.getPixelColor(i)==blue){ //青の棒の押し引きがあったら
        //myDFPlayer.play(2);
        blueteam++;//青チームに得点
        //Serial.print(i);
        //Serial.println("がきたよ");
        //Serial.print("blue:");
        //Serial.println(blueteam);
        ChangeWhite(i);
        ChangeLED(blue);
        strip.show();
      }
      else if((st==1 || st==0)&& strip.getPixelColor(i)==pink){ //ピンクの棒の押し引きがあったら
        //myDFPlayer.play(2);
        pinkteam++;//ピンクチームに得点
        //Serial.print(i);
        //Serial.println("がきたよ");
        //Serial.print("pink:");
        //Serial.println(pinkteam);
        ChangeWhite(i);
        ChangeLED(pink);
        strip.show();
      }
}
}
