/**************************
 * 作者：顾加1
 * 日期：2022年1月19日
 * copy后记得一键三连
 */
#include "arduinoFFT.h"
#include <Adafruit_SSD1306.h>
#define samples 64 //采样点数，2的N次幂
#define halfsamples samples/2
#define NumofCopy halfsamples*sizeof(double)
#define Interval 128/(halfsamples)
Adafruit_SSD1306 display(100);
arduinoFFT FFT = arduinoFFT();

double vReal[samples];
double vImag[samples];
double vTemp[halfsamples];

void setup() {
  setP32();  //设置32分频,加快AD采样
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void loop() {
//  Serial.println("start:"+String(micros()));
  for(int i=0;i<samples;i++)
  {
    vReal[i]=analogRead(A0)-512;  //采样
    vImag[i] = 0.0;
  }
//  Serial.println("end:"+String(micros()));
  display.clearDisplay();
  FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);//加窗
  FFT.Compute(vReal, vImag, samples, FFT_FORWARD);  //计算fft
  FFT.ComplexToMagnitude(vReal, vImag, samples);  //计算幅度
  for(int i=0;i<halfsamples;i++)
  {
    display.drawPixel(i*Interval,vTemp[halfsamples-i-1]*0.007+1, WHITE); //显示
    display.drawLine(i*Interval, 0, i*Interval,vReal[halfsamples-i-1]*0.007, WHITE);
  }
  display.display();
  memcpy(vTemp, vReal, NumofCopy);
  delay(20);  //改为128点时可以注释掉，防止刷新太慢
}
void setP32()//设置32分频
{
  ADCSRA|=(1<<ADPS2);  
  ADCSRA&=~(1<<ADPS1);
  ADCSRA|=(1<<ADPS0);
}
void setP16( ) {  //设置16分频，不建议
  ADCSRA |=  (1 << ADPS2);   // 1
  ADCSRA &=  ~(1 << ADPS1);  // 0
  ADCSRA &=  ~(1 << ADPS0);  // 0
}
