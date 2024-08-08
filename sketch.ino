const int RX_PIN = 12;
const int TX_PIN = 2;
const int INPUT_BUTTON_PIN = 3;
const int BAUD_RATE = 9600;
const int DELAY = 10;

// Duration is greatly increased because of the delay
const int MS_DOT = 100;

const String DAH_SYMBOL = "_";
const String DOT_SYMBOL = ".";

unsigned long pushButtonTimer = 0;
unsigned long pushButtonNotPressedTimer = 0;
unsigned long 


void setup() {
  // Activate the internal pull-up resistor
  pinMode(INPUT_BUTTON_PIN, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  // If button pushed
  if (digitalRead(INPUT_BUTTON_PIN) == LOW) {
    // Set the timer if it's the first execution since button push
    if (pushButtonTimer == 0) {
      pushButtonTimer = millis();
    }
  } else {
    if (pushButtonTimer != 0) {
      unsigned long buttonPressDuration = millis() - pushButtonTimer;

      const String result = buttonPressDuration > MS_DOT ? DAH_SYMBOL : DOT_SYMBOL;

      Serial.println(result);
      pushButtonTimer = 0;
    }
  }

  if (pushButtonNotPressedTimer == 0) {
    pushButtonNotPressedTimer = millis();
  } else {
    pushButtonNotPressedTimer = 0;
  }

  delay(DELAY);
}
