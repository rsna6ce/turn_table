#define PIN_TOP 12
#define PIN_CNT 4
#define PIN_SW 2

#define SW_ON  HIGH
#define SW_OFF LOW

#define DELAY_LOW  5
#define DELAY_HIGH 2

void setup() {
    // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("hello. stepping_motor.");

  //各ピンを出力モードにする
  for(int i=0; i<PIN_CNT; i++) {
    pinMode(PIN_TOP+i, OUTPUT);
  }
  pinMode(PIN_SW, INPUT_PULLUP);
}

enum rotate_mode {MODE_STOP=0, MODE_RIGHT_LOW, MODE_RIGHT_HIGH, MODE_LEFT_LOW, MODE_LEFT_HIGH};
int bit_state = 0;
rotate_mode curr_rotate_mode = MODE_STOP;
int prev_switch = SW_OFF;
void loop() {
    int curr_switch = digitalRead(PIN_SW);
    if (prev_switch != curr_switch) {
        prev_switch = curr_switch;
        if (curr_switch == SW_ON) {
        curr_rotate_mode = (rotate_mode)((int)curr_rotate_mode+1);
            if (curr_rotate_mode > MODE_LEFT_HIGH) {
                curr_rotate_mode = MODE_STOP;
            }
        }
    }

    bit_state++;
    int bit_state2 = bit_state & 0x03;
    switch (curr_rotate_mode) {
        case MODE_RIGHT_LOW:
        case MODE_RIGHT_HIGH:
            digitalWrite(PIN_TOP+3, (bit_state2==3 || bit_state2==0));
            digitalWrite(PIN_TOP+2, (bit_state2==0 || bit_state2==1));
            digitalWrite(PIN_TOP+1, (bit_state2==1 || bit_state2==2));
            digitalWrite(PIN_TOP+0, (bit_state2==2 || bit_state2==3));
            break;
        case MODE_LEFT_LOW:
        case MODE_LEFT_HIGH:
            digitalWrite(PIN_TOP+0, (bit_state2==3 || bit_state2==0));
            digitalWrite(PIN_TOP+1, (bit_state2==0 || bit_state2==1));
            digitalWrite(PIN_TOP+2, (bit_state2==1 || bit_state2==2));
            digitalWrite(PIN_TOP+3, (bit_state2==2 || bit_state2==3));
            break;
        default:
            digitalWrite(PIN_TOP+0, 0);
            digitalWrite(PIN_TOP+1, 0);
            digitalWrite(PIN_TOP+2, 0);
            digitalWrite(PIN_TOP+3, 0);
            break;
    }

    
    // delay time
    switch (curr_rotate_mode) {
        case MODE_RIGHT_LOW:
        case MODE_LEFT_LOW:
            delay(DELAY_LOW);
            break;
        case MODE_RIGHT_HIGH:
        case MODE_LEFT_HIGH:
            delay(DELAY_HIGH);
            break;
        default:
            delay(10);
            break;
    }
}
