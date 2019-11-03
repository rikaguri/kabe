#include <Adafruit_NeoPixel.h>
const int NUM_PIXELS = 24;
const int DATA_PIN =2;
const int NUM_SWITCH = 24;//13,12,11,10,9,8,7,6,5,4,3,22,23,24,25,26,27,28,29,30,31,32,33,34
#define face1 0
#define face2 0

int reedsw[24]={13,12,11,10,9,8,7,6,5,4,3,22,23,24,25,26,27,28,29,30,31,32,33,34};//リードスイッチのピン番号
int reedst[24]={0};//リードスイッチのステータス(0or1)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, DATA_PIN, NEO_RGB + NEO_KHZ800);

uint32_t white = strip.Color(255, 255, 255);
uint32_t blue = strip.Color(0, 0, 255);
uint32_t pink = strip.Color(255, 0, 100);

int blueteam=0;
int pinkteam=0;

int cnt1[24]={0};
int cnt0[24]={0};

//uint32_t color = strip.getPixelColor();

void setup() {
  Serial.begin(38400);
  // put your setup code here, to run once:
strip.begin();
strip.show();
strip.setBrightness(255);
//リードスイッチのpin設定
for(int i=0;i<NUM_SWITCH;i++){
  pinMode(reedsw[i], INPUT_PULLUP);
  reed(i);
  }

//初期化
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




void loop() {
  //押し引きがあったかの処理
for(int i=0;i<NUM_SWITCH;i++){
  int st=reed(i);
  if((st==1 || st==0)&& strip.getPixelColor(i)==blue){ //青の棒の押し引きがあったら
    blueteam++;//青チームに得点
    Serial.print("blue:");
    Serial.println(blueteam);
    ChangeWhite(i);
    ChangeLED(blue);
    strip.show();
  }
  else if((st==1 || st==0)&& strip.getPixelColor(i)==pink){ //ピンクの棒の押し引きがあったら
    pinkteam++;//ピンクチームに得点
    Serial.print("pink:");
    Serial.println(pinkteam);
    ChangeWhite(i);
    ChangeLED(pink);
    strip.show();
  }
}

//delay(100);
}
//=========================================================//
// LEDを他の色に変える関数
//=========================================================//
void ChangeLED(uint32_t color){
  for(int i=0;i<30;i++){
  int index = random(24); // index番目の白い棒の色を変える
  Serial.println(index);
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
// リードスイッチの押し引き判定(1(true)か0(false)が返ってくるようにする)
//=========================================================//
int reed(int i){//iはリードスイッチの番号
  cnt1[i]=0;//ついてる時間をカウント
  cnt0[i]=0;//消えてる時間をカウント
  for(int j=0;j<20;j++){
  if(digitalRead(reedsw[i])==1){
    cnt1[i]++;
    }
  else if(digitalRead(reedsw[i])==0){
    cnt0[i]++;
    }
  }
  if(cnt1[i]>15 && reedst[i]==0){//リードスイッチONでステータスがOFFなら
    reedst[i]=1;//ステータスをONにする
    return 1;
  }
  else if(cnt0[i]>15 && reedst[i]==1){//リードスイッチOFFでステータスがONなら
    reedst[i]=0;//ステータスをOFFにする
    return 0;
  }
  else return 2;
}
