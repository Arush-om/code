#include <Servo.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#include <LiquidCrystal_I2C.h>

#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

LiquidCrystal_I2C lcd(0x27, 20, 4);

Servo myservo;

#define ir_enter 2
#define ir_back 4
#define ir_car1 5
#define ir_car2 6
#define ir_car3 7
#define ir_car4 8
#define ir_car5 9
#define ir_car6 10

#define slot1 11
#define slot2 12
#define slot3 13
#define slot4 22
#define slot5 23
#define slot6 24

int pin_array[6] = {slot1, slot2, slot3, slot4, slot5, slot6};
int data_array[6];
int flag1 = 0, flag2 = 0;
int slot = 6;

void setup()
{
    Serial.begin(9600);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;
    }
    display.display();
    display.clearDisplay();

    pinMode(ir_car1, INPUT);
    pinMode(ir_car2, INPUT);
    pinMode(ir_car3, INPUT);
    pinMode(ir_car4, INPUT);
    pinMode(ir_car5, INPUT);
    pinMode(ir_car6, INPUT);

    pinMode(slot1, OUTPUT);
    pinMode(slot2, OUTPUT);
    pinMode(slot3, OUTPUT);
    pinMode(slot4, OUTPUT);
    pinMode(slot5, OUTPUT);
    pinMode(slot6, OUTPUT);

    pinMode(ir_enter, INPUT);
    pinMode(ir_back, INPUT);

    myservo.attach(3);
    myservo.write(90);

    lcd.begin();
    lcd.setCursor(0, 1);
    lcd.print(" STIST Car parking ");
    lcd.setCursor(0, 2);
    lcd.print("       System     ");
    delay(2000);
    lcd.clear();
    for (int i = 0; i < 6; i++)
    {
    digitalWrite(pin_array[i], LOW);
    }
    Read_Sensor();
}

void loop()
{

    Read_Sensor();

    lcd.setCursor(0, 0);
    lcd.print("   Have Slot: ");
    lcd.print(slot);
    lcd.print("    ");
    lcd.setCursor(0, 1);

    if (data_array[0] == 0)
    {
        lcd.print("S1:Fill ");
    }
    else
    {
        lcd.print("S1:Empty");
    }

    lcd.setCursor(10, 1);
    if (data_array[1] == 0)
    {
        lcd.print("S2:Fill ");
    }
    else
    {
        lcd.print("S2:Empty");
    }

    lcd.setCursor(0, 2);
    if (data_array[2] == 0)
    {
        lcd.print("S3:Fill ");
    }
    else
    {
        lcd.print("S3:Empty");
    }

    lcd.setCursor(10, 2);
    if (data_array[3] == 0)
    {
        lcd.print("S4:Fill ");
    }
    else
    {
        lcd.print("S4:Empty");
    }

    lcd.setCursor(0, 3);
    if (data_array[4] == 0)
    {
        lcd.print("S5:Fill ");
    }
    else
    {
        lcd.print("S5:Empty");
    }

    lcd.setCursor(10, 3);
    if (data_array[5] == 0)
    {
        lcd.print("S6:Fill ");
    }
    else
    {
        lcd.print("S6:Empty");
    }

    if (digitalRead(ir_enter) == 0 && flag1 == 0)
    {
        if (slot > 0)
        {
            flag1 = 1;
            if (flag2 == 0)
            {
                myservo.write(180);
                find_slot();
            }
        }
        else
        {
            lcd.setCursor(0, 0);
            lcd.print(" Sorry Parking Full ");
            delay(1500);
        }
    }

    if (digitalRead(ir_back) == 0 && flag2 == 0)
    {
        flag2 = 1;
        if (flag1 == 0)
        {
            myservo.write(180);
        }
    }

    if (flag1 == 1 && flag2 == 1)
    {
        delay(1000);
        myservo.write(90);
        flag1 = 0, flag2 = 0;
    }

    delay(1);
}

void find_slot()
{
    Read_Sensor();
    for (int i = 0; i < 6; i++)
    {
        if (data_array[i] == 1)
        {
            display.clearDisplay();
            display.setTextSize(1);
            display.setTextColor(WHITE);
            display.setCursor(0, 10);
            display.println(i + 1);
            display.display();
            Serial.print("Please park in slot number:");
            Serial.print(i + 1);
            Serial.println("");
            digitalWrite(pin_array[i], HIGH);
            delay(1500);
            digitalWrite(pin_array[i], LOW);
            return;
        }
    }
}

void Read_Sensor()
{
    slot = 0;

    for (int i = 0; i < 6; i++)
    {
        int sensor_value = digitalRead(i + 5);
        data_array[i] = sensor_value;
        slot += sensor_value;
    }
}
