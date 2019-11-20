#define RED_PIN 3
#define YELLOW_PIN 4
#define GREEN_PIN 5
#define RED_STATE 0
#define RED_YELLOW_STATE 1
#define YELLOW_STATE 2
#define GREEN_STATE 3
#define RED_MILLIS 10000
#define buttonPin 7
#define buzzerPin 8

int tl_state = RED_STATE; // Traffic light state.
unsigned long tl_timer; // Traffic light timer.
bool is_to_red = true;
int counter = 0;
bool buzzerstate = true;
int buttonState = digitalRead(buttonPin);

void setup() {
  // Configure LED pins as outputs.
  pinMode(buttonPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  // Initial state for states and timers..
  tl_state = RED_STATE;
  tl_timer = millis() + RED_MILLIS;
  Serial.begin(9600);
}
void loop() {
  // TL state machine.
  switch (tl_state) {
    case RED_STATE:
      digitalWrite(RED_PIN, HIGH);
      if (counter % 250 == 0) {
        if (buzzerstate) {
          digitalWrite(buzzerPin, HIGH); 
          buzzerstate = !buzzerstate;
        } else {
          digitalWrite(buzzerPin, LOW);
          buzzerstate = !buzzerstate;
        }
      }
      if (counter == 10000) { // Turn red light off.
        counter = 0;
        digitalWrite(buzzerPin, LOW);
        tl_state = RED_YELLOW_STATE;
      }
      break;
    case RED_YELLOW_STATE:
      digitalWrite(YELLOW_PIN, HIGH);
      if (counter == 2000) {
        digitalWrite(RED_PIN, LOW);
        counter = 0;
        tl_state = YELLOW_STATE;
      }
      break;
    case YELLOW_STATE:
      digitalWrite(buzzerPin, LOW);
      digitalWrite(YELLOW_PIN, HIGH);
      if (counter == 2000) {
        if (is_to_red) {
          counter = 0;
          digitalWrite(YELLOW_PIN, LOW);
          tl_state = GREEN_STATE;
        } else {
          counter = 0;
          digitalWrite(YELLOW_PIN, LOW);
          tl_state = RED_STATE;
          is_to_red = true;
        }
      } 
      break;
    case GREEN_STATE:
      digitalWrite(GREEN_PIN, HIGH);
      if (counter % 500 == 0) {
        digitalWrite(buzzerPin, HIGH); 
      }
      if (counter % 1500 == 0) {
        digitalWrite(buzzerPin, LOW);
      }
      is_to_red = false;
      if (counter >= 5000 && buttonState == HIGH) {
        buttonState = LOW;
        digitalWrite(GREEN_PIN, LOW);
        tl_state = YELLOW_STATE;
        counter = 0;
      }
      break;
  }
  if (buttonState == LOW && tl_state == GREEN_STATE) {
    buttonState = digitalRead(buttonPin);
  }
  delay(1);
  counter++;
}
