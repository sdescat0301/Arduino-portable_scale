#include "U8glib.h"
#include "Adafruit_ADS1015.h"
#include "Wire.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);	  // 屏幕型号，I2C / TWI

String mess_in_Kg = "weight(kg):";
String mess_in_lb = "weight(lb):";

Adafruit_ADS1115 ads(0x48);    //声明一个ads对象，设置模块地址
float base_voltage = 0.0;   //空载电压，用于调零
float Voltage = 0.0;    //存储检测到的电压值
float true_voltage = 0.0;   //存储由于重物改变的电压值，也就是理论上的电压值
float Weight = 0.0;    //存储计算得到的质量

void draw(int Weight)   //函数：在显示屏上输出,单位为kg
{
    u8g.setFont(u8g_font_fur17r);    //设置第一行字体
    u8g.setPrintPos(20,10);    //设置第一行输出位置
    u8g.print(mess_in_Kg);    //输出第一行"weight(kg):"
    u8g.setFont(u8g_font_fur30n);    //设置第二行字体
    u8g.setPrintPos(20,50);    //设置第二行输出单位
    u8g.print(Weight);    //输出第二行：重量
}

void setup()
{
    Serial.begin(9600);    //开始串口通信，设置波特率为9600
    ads1115.begin();    //开始读数
    ads1115.setGain(GAIN_TWOTHIRDS);    // 设置增益为2/3x，输入电压的范围为6.144V，实际输入0-5V，输出数字为0-26667
    uint16_t base_adc = ads.readADC_SingleEnded(0);    //读取空载时力传感器的信号（系统误差）
    base_voltage = (float)base_adc * 0.003;    //计算由于系统误差带来的空载电压
}

void loop() 
{
    uint16_t adc = ads.readADC_SingleEnded(0);    //读取挂上重物时力传感器的信号
    Voltage = (float)adc * 0.003;    //计算挂上重物之后的电压值

    true_voltage = Voltage - base_voltage;    //计算由于挂上重物而增加的电压值
    Weight = true_voltage * 4;    //计算质量
    Weight = ( (float)( (int)( (Weight+0.005)*100 ) ) )/100;    //取重量小数点后两位

    u8g.firstPage();  
    do {
        draw(Weight);
    } while( u8g.nextPage() );    //输出质量（单位为kg）
  
    delay(30);
}