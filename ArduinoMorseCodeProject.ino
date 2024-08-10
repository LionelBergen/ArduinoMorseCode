const int RX_PIN = 12;
const int TX_PIN = 2;
const int INPUT_BUTTON_PIN = 3;
const int BAUD_RATE = 9600;
const int DELAY = 10;

// Duration is greatly increased because of the delay
const int TIME_UNIT = 100;

// DOT is one unit, a space between words is 7 units
const struct {
  const int MS_DOT;
  const int END_OF_MESSAGE;
  const int END_OF_CHARACTER;
} TIMER = { TIME_UNIT, 5000, (TIME_UNIT * 7) };

const struct {
  const char DAH;
  const char DOT;
  const char SPACE;
} SYMBOL = {'_', '.', ' '};

const struct {
  const String M;
  const String K;
} LETTERS = {"__", "_._"};

unsigned long pushButtonTimer = 0;
unsigned long pushButtonNotPressedTimer = 0;

const int MESSAGE_SIZE = 30;
char MESSAGE[MESSAGE_SIZE] = {};
int currentPos = 0;


void setup() {
  // Activate the internal pull-up resistor
  pinMode(INPUT_BUTTON_PIN, INPUT_PULLUP);

  Serial.begin(9600);
}

void handleMessageSend() {
  if (MESSAGE[0] != NULL) {
    for (int i=0; i<MESSAGE_SIZE; i++) {
      if (MESSAGE[i] != NULL) {
        Serial.print(MESSAGE[i]);
      }
    }

    Serial.print("\n");
    currentPos = 0;
    memset(MESSAGE, NULL, sizeof(MESSAGE));
  }
}

void loop() {
  unsigned long currentTime = millis();

  // If button pushed
  if (digitalRead(INPUT_BUTTON_PIN) == LOW) {
    // Set the timer if it's the first execution since button push
    if (pushButtonTimer == 0) {
      pushButtonTimer = currentTime;
    }

    if (pushButtonNotPressedTimer != 0) {
      pushButtonNotPressedTimer = 0;
    }
  } else {
    if (pushButtonTimer != 0) {
      unsigned long buttonPressDuration = currentTime - pushButtonTimer;

      const char result = buttonPressDuration > TIMER.MS_DOT ? SYMBOL.DAH : SYMBOL.DOT;

      MESSAGE[currentPos++] = result;
      pushButtonTimer = 0;
    }

    if (pushButtonNotPressedTimer == 0) {
      pushButtonNotPressedTimer = currentTime;
    } else {
      unsigned long buttonNotPressedDuration = currentTime - pushButtonNotPressedTimer;
      if (buttonNotPressedDuration >= TIMER.END_OF_MESSAGE) {
        handleMessageSend();
      } else if (buttonNotPressedDuration >= TIMER.END_OF_CHARACTER) {
        // If we don't already have a space
        if (MESSAGE[currentPos - 1] != SYMBOL.SPACE) {
          MESSAGE[currentPos++] = SYMBOL.SPACE;
        }
      }
    }
  }

  delay(DELAY);
}
