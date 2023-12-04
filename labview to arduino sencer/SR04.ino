
int sr_echo=6;
int sr_trig=7;
 int AD_Value ;
 float float_AD_Value;
 int inByte;
//-----------------------------------------------//
//這是距離獲取函數
//這個函數執行時將執行對應操作通過sr04獲取距離
//-----------------------------------------------//
float get_distances() {
  float sr_time=0,sr_dis=0;
  digitalWrite(sr_trig,LOW);        //先發送低電位，為發送啟動信號做準備
  delay(1);          //延遲1ms
  digitalWrite(sr_trig,HIGH);       //發送準備信號，準備信號是10us以上的高電位，這裡我們發送1ms的高電位
  delay(1);         //讓12號接腳保持高電位1ms
  digitalWrite(sr_trig,LOW);        //高電位發送後，延遲結束，這時我們變低電位，為讀取回傳做準備
  sr_time=pulseIn(sr_echo,HIGH);    //開始讀取回傳時間,即echo讀入為高電位的時間
  sr_time=sr_time/1000/1000;      //將微秒轉為秒
  sr_dis=sr_time*340/2;             //回傳時間是往返時間所以要除2得到單程時間，聲速大約是340M/S，因此再乘
  sr_dis*=100;          //將米單位轉為厘米單位
  return sr_dis;
}
 
//-----------------------------------------------//
//這是設置程序
//這個函數在arduino通電後或reset時執行
//-----------------------------------------------//
void setup() {
  // put your setup code here, to run once:
  pinMode(sr_echo, INPUT);    //設定sr_echo為輸入模式
  pinMode(sr_trig, OUTPUT);   //設定sr_trig為輸出模式
  Serial.begin(115200);       //設定的鮑率
   pinMode(3, OUTPUT);   //3腳設定為輸出
    pinMode(13, OUTPUT);   //13腳設定為輸出
     pinMode(12, OUTPUT);   //12腳設定為輸出
}
 
void loop() 
{
  if (Serial.available() > 0) //檢查串列通訊介面是否有可用的資料。
  {
    // get incoming byte:
    inByte = Serial.read();//讀取一個位元組的資料並儲存在 inByte 變數中。
    if(inByte==49)//如果讀取的位元組等於 49（ASCII 碼對應數字 '1'），則執行
    analogWrite(3,inByte);
    
   { digitalWrite(12, HIGH); }//腳位 12 設置為高電位
    
     if(inByte==48)//如果讀取的位元組等於 48（ASCII 碼對應數字 '0'），則執行
   { digitalWrite(12, LOW); }//腳位 12 設置為低電位
     if(inByte==50)//如果讀取的位元組等於 50（ASCII 碼對應數字 '2'），則執行
   { digitalWrite(13, HIGH); }//腳位 13 設置為高電位
    
     if(inByte==51)//如果讀取的位元組等於 51（ASCII 碼對應數字 '3'），則執行
   { digitalWrite(13, LOW); }//腳位 13 設置為低電位
    
  }
  float sr_dis=get_distances();//呼叫 get_distances() 函式，並將返回的距離值存儲在 sr_dis 變數中。
  Serial.print(sr_dis);         //向PC發送測得的距離
     Serial.print(',');//傳送逗號分隔符，用於在距離值和類比數值之間劃分
    AD_Value = analogRead(A0);  //輸入腳位 A0 讀取類比數值，並將其存儲在 AD_Value 變數中
    
    float_AD_Value=(float)AD_Value/1023*5.00;  //換算為浮點電壓值
     Serial.println(float_AD_Value,2);          //保留兩位小數發送數據
            delay(400);                //  延遲400ms
 
}
