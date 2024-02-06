// clang-format off
#include <SPIFFS.h>
#include "main.hpp"
// clang-format on

const int LEFT_RELAY_MODULE_PIN = 1;   //左のrelayModuleピン
const int RIGHT_RELAY_MODULE_PIN = 2;  //右のrelayModuleピン


// int prev_x = -1;
// int prev_y = -1;

static m5::touch_state_t prev_state;    //タッチ状態管理用変数

long oldUseValue = 0;   //ダイヤルの管理用変数

const double TSP = 2.0;  //小さじの単位秒数
const double TBSP = 6.0;  //大さじの単位秒数


const int LEFT_SEASONING = 0;   //左の調味料を使用するとき用の定数
const int RIGHT_SEASONING = 1; //右の調味料を使用するとき用の定数
int usingSeasoning = -99;    //どちらの調味料を使用するかの管理用変数

bool useFlag = false;   //調味料を使用するかどうか

//タッチ状態の種類と合致する定数
static constexpr const char* STATE_NAME[16] = {
    "none", "touch", "touch_end", "touch_begin",
    "___",  "hold",  "hold_end",  "hold_begin",
    "___",  "flick", "flick_end", "flick_begin",
    "___",  "drag",  "drag_end",  "drag_begin"};
const int TOUCH = 1;        //タッチイベント
const int TOUCH_BEGIN = 3;  //タッチ開始イベント

bool outSeasoning(int usingSeasoning,long useValue){
    // 調味料を出す処理
    M5Dial.Display.fillRect(0, 0, M5Dial.Display.width(),
                                    M5Dial.Display.height(), BLACK);
                        M5Dial.Display.drawString("executing",
                                    M5Dial.Display.width() / 2,
                                    M5Dial.Display.height() / 2);
    if (usingSeasoning == LEFT_SEASONING){
        if(useValue >= 1 && useValue <= 2){
            Serial.println("LEFT_RELAY_MODULE ON");
            Serial.println(String(useValue) + " tsp out.");
            Serial.println("Wait " + String(TSP * useValue) + " seconds.");
            digitalWrite(LEFT_RELAY_MODULE_PIN, HIGH);
            delay(TSP * useValue * 1000);
            digitalWrite(LEFT_RELAY_MODULE_PIN, LOW);
        }else if (useValue >= 3 && useValue <= 7){
            useValue -= 2;
            Serial.println("LEFT_RELAY_MODULE ON");
            Serial.println(String(useValue) + " tbsp out.");
            Serial.println("Wait " + String(TBSP * useValue) + " seconds.");
            digitalWrite(LEFT_RELAY_MODULE_PIN, HIGH);
            delay(TBSP * useValue * 1000);
            digitalWrite(LEFT_RELAY_MODULE_PIN, LOW);
        }
    }else if (usingSeasoning == RIGHT_SEASONING){
        if(useValue >= 1 && useValue <= 2){
            Serial.println("RIGHT_RELAY_MODULE ON");
            Serial.println(String(useValue) + " tsp out.");
            Serial.println("Wait " + String(TSP * useValue) + " seconds.");
            digitalWrite(RIGHT_RELAY_MODULE_PIN, HIGH);
            delay(TSP * useValue * 1000);
            digitalWrite(RIGHT_RELAY_MODULE_PIN, LOW);
        }else if (useValue >= 3 && useValue <= 7){
            useValue -= 2;
            Serial.println("RIGHT_RELAY_MODULE ON");
            Serial.println(String(useValue) + " tbsp out.");
            Serial.println("Wait " + String(TBSP * useValue) + " seconds.");
            digitalWrite(RIGHT_RELAY_MODULE_PIN, HIGH);
            delay(TBSP * useValue * 1000);
            digitalWrite(RIGHT_RELAY_MODULE_PIN, LOW);
        }
    }
    
    return true;
}

void drawUseRelayModule(long useValue){
    // M5Dial.Speaker.tone(8000, 20);
    // M5Dial.Display.clear();
    // 画像にするときに消す！
    M5Dial.Display.fillRect(0, 0,
                            M5Dial.Display.width(),
                            M5Dial.Display.height() / 3, BLACK);
    M5Dial.Display.drawString("back",
                M5Dial.Display.width() / 2,
                M5Dial.Display.height() / 3 - 30);
    M5Dial.Display.fillRect(0, (M5Dial.Display.height() / 3) * 2,
                            M5Dial.Display.width(),
                            M5Dial.Display.height() / 3, BLUE);
    M5Dial.Display.drawString("execute",
                M5Dial.Display.width() / 2,
                (M5Dial.Display.height() / 3) * 2 + 30);



    // ここに画像を張り付ける！
    if(useValue == 0){
        M5Dial.Display.fillRect(0, (M5Dial.Display.height() / 3) * 1,
                            M5Dial.Display.width(),
                            M5Dial.Display.height() / 3, RED);
        M5Dial.Display.drawString("tsp:" + String(useValue * -1),
                            M5Dial.Display.width() / 2,
                            M5Dial.Display.height() / 2);
    }else if (useValue == 1){
        M5Dial.Display.fillRect(0, (M5Dial.Display.height() / 3) * 1,
                            M5Dial.Display.width(),
                            M5Dial.Display.height() / 3, RED);
        M5Dial.Display.drawString("tbsp:" + String(useValue),
                            M5Dial.Display.width() / 2,
                            M5Dial.Display.height() / 2);
    }else if (useValue == 2){
        M5Dial.Display.fillRect(0, (M5Dial.Display.height() / 3) * 1,
                            M5Dial.Display.width(),
                            M5Dial.Display.height() / 3, RED);
        M5Dial.Display.drawString("tbsp:" + String(useValue),
                            M5Dial.Display.width() / 2,
                            M5Dial.Display.height() / 2);
    }else if (useValue == 3){
        M5Dial.Display.fillRect(0, (M5Dial.Display.height() / 3) * 1,
                            M5Dial.Display.width(),
                            M5Dial.Display.height() / 3, RED);
        M5Dial.Display.drawString("tbsp:" + String(useValue),
                            M5Dial.Display.width() / 2,
                            M5Dial.Display.height() / 2);
    }else if (useValue == 4){
        M5Dial.Display.fillRect(0, (M5Dial.Display.height() / 3) * 1,
                            M5Dial.Display.width(),
                            M5Dial.Display.height() / 3, RED);
        M5Dial.Display.drawString("tbsp:" + String(useValue),
                            M5Dial.Display.width() / 2,
                            M5Dial.Display.height() / 2);
    }else if (useValue == 5){
        M5Dial.Display.fillRect(0, (M5Dial.Display.height() / 3) * 1,
                            M5Dial.Display.width(),
                            M5Dial.Display.height() / 3, RED);
        M5Dial.Display.drawString("tbsp:" + String(useValue),
                            M5Dial.Display.width() / 2,
                            M5Dial.Display.height() / 2);
    }else if (useValue == 6){
        M5Dial.Display.fillRect(0, (M5Dial.Display.height() / 3) * 1,
                            M5Dial.Display.width(),
                            M5Dial.Display.height() / 3, RED);
        M5Dial.Display.drawString("tbsp:" + String(useValue),
                            M5Dial.Display.width() / 2,
                            M5Dial.Display.height() / 2);
    }else if (useValue == 7){
        M5Dial.Display.fillRect(0, (M5Dial.Display.height() / 3) * 1,
                            M5Dial.Display.width(),
                            M5Dial.Display.height() / 3, RED);
        M5Dial.Display.drawString("tbsp:" + String(useValue),
                            M5Dial.Display.width() / 2,
                            M5Dial.Display.height() / 2);
    }
}


void setup(void) {
    M5_BEGIN();
    auto cfg = M5.config();
    M5Dial.begin(cfg, true, false);
    M5Dial.Display.setTextColor(GREEN);
    M5Dial.Display.setTextDatum(middle_center);
    // M5Dial.Display.setTextFont(&fonts::Orbitron_Light_24);
    M5Dial.Display.setFont(&fonts::Orbitron_Light_24);
    M5Dial.Display.setTextSize(1);

    pinMode(LEFT_RELAY_MODULE_PIN, OUTPUT);
    digitalWrite(LEFT_RELAY_MODULE_PIN, LOW);

    pinMode(RIGHT_RELAY_MODULE_PIN, OUTPUT);
    digitalWrite(RIGHT_RELAY_MODULE_PIN, LOW);

    // pinMode(LEFT_RELAY_MODULE_PIN, OUTPUT);
    // pinMode(RIGHT_RELAY_MODULE_PIN, OUTPUT);
    // digitalWrite(LEFT_RELAY_MODULE_PIN, LOW);
    // digitalWrite(RIGHT_RELAY_MODULE_PIN, LOW);

    SPIFFS.begin();

    M5.Lcd.drawBmpFile(SPIFFS, "/Cheifoon_title.bmp", 0, 0);
}

void loop(void) {
    M5_UPDATE();
    
    M5Dial.update();
    
    auto t = M5Dial.Touch.getDetail();

    if(!useFlag){
        if (prev_state != t.state) {
            prev_state                                  = t.state;
            M5Dial.Display.fillRect(0, 0, M5Dial.Display.width(),
                                    M5Dial.Display.height() / 2, BLACK);
            M5Dial.Display.drawString("left seasoning",
                                    M5Dial.Display.width() / 2,
                                    M5Dial.Display.height() / 2 - 40);
            M5Dial.Display.fillRect(0, M5Dial.Display.height() / 2, M5Dial.Display.width(),
                                    M5Dial.Display.height() / 2, RED);
            M5Dial.Display.drawString("right seasoning",
                                    M5Dial.Display.width() / 2,
                                    M5Dial.Display.height() / 2 + 40);
            if(t.state == TOUCH){
                if(t.y <= 120){
                    Serial.println("left seasoning");
                    usingSeasoning = LEFT_SEASONING;
                }else{
                    Serial.println("right seasoning");
                    usingSeasoning = RIGHT_SEASONING;
                }
                useFlag = true;
                M5Dial.Encoder.write(0);
                drawUseRelayModule(0);
            }
        }
    }else{
        long newUseValue = M5Dial.Encoder.read() / 4;
        if(newUseValue < 0){
            M5Dial.Encoder.write(0);
            newUseValue = 0;
        }else if (newUseValue > 7){
            M5Dial.Encoder.write(7 * 4);
            newUseValue = 7;
        }
        
        if (newUseValue != oldUseValue) {
            drawUseRelayModule(newUseValue);
            oldUseValue = newUseValue;
            Serial.println(newUseValue);
        }
        if (prev_state != t.state) {
            prev_state                                  = t.state;
            if(t.state == TOUCH){
                if(t.y <= 80){
                    Serial.println("back event");
                    useFlag = false;
                }else if (t.y >= 160){
                    if(newUseValue != 0){
                        if(outSeasoning(usingSeasoning,newUseValue)){
                            M5Dial.Display.fillRect(0, 0, M5Dial.Display.width(),
                                        M5Dial.Display.height(), BLACK);
                            M5Dial.Display.drawString("complete",
                                        M5Dial.Display.width() / 2,
                                        M5Dial.Display.height() / 2);
                            delay(1500);
                        }else{
                            M5Dial.Display.fillRect(0, 0, M5Dial.Display.width(),
                                        M5Dial.Display.height(), BLACK);
                            M5Dial.Display.drawString("execute cancel",
                                        M5Dial.Display.width() / 2,
                                        M5Dial.Display.height() / 2);
                            delay(1500);
                        }
                        useFlag = false;
                    }
                }else{                
                    M5Dial.Encoder.write(0);
                }
            }
        }
        if (M5Dial.BtnA.pressedFor(1500)) {
            M5Dial.Encoder.write(0);
        }
    }
    
    // if (prev_x != t.x || prev_y != t.y) {
    //     M5Dial.Display.fillRect(0, M5Dial.Display.height() / 2,
    //                             M5Dial.Display.width(),
    //                             M5Dial.Display.height() / 2, BLACK);
    //     M5Dial.Display.drawString(
    //         "X:" + String(t.x) + " / " + "Y:" + String(t.y),
    //         M5Dial.Display.width() / 2, M5Dial.Display.height() / 2 + 30);
    //     prev_x = t.x;
    //     prev_y = t.y;
    //     M5Dial.Display.drawPixel(prev_x, prev_y);
    // }
}
