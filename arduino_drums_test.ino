// Piezo inputs
int piezoPins[5] = {A0, A1, A2, A3, A4};
// LED outputs
int ledPins[5] = {2, 3, 4, 12, 13};
// Names for Serial output
String names[5] = {"KICK", "SNARE", "TOM", "HAT", "CRASH"};
// Smoothed envelope values
int envelope[5] = {0, 0, 0, 0, 0};
// Track how long each pad stays above threshold
bool isActive[5] = {false, false, false, false, false};
unsigned long hitStart[5] = {0, 0, 0, 0, 0};
// Fade speed (bigger = faster fade)
int fadeSpeed = 15;
// Sensitivity
int threshold = 60;
String getSpeedLevel(unsigned long durationMs) {
  if (durationMs < 60) return "1";   // short / fast
  if (durationMs < 125) return "2";  // normal
  return "3";                        // long / slow
}
void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 5; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
}
void loop() {
  for (int i = 0; i < 5; i++) {
    int raw = analogRead(piezoPins[i]);
    if (raw > envelope[i]) {
      envelope[i] = raw;
    } else {
      envelope[i] -= fadeSpeed;
      if (envelope[i] < 0) envelope[i] = 0;
    }
    
    //TrunoffLED
    analogWrite(ledPins[i], 0);

    if (raw > threshold) {
      if (!isActive[i]) {
        isActive[i] = true;
        hitStart[i] = millis();
      }
    } else if (isActive[i]) {
      unsigned long durationMs = millis() - hitStart[i];
      Serial.println(names[i] + getSpeedLevel(durationMs));
      isActive[i] = false;

        //TurnOnLED
        int brightness = map(envelope[i], 0, 1023, 0, 255);
        if (brightness > 0 && brightness < 25) {
            brightness = 25;
        }
        
        analogWrite(ledPins[i], brightness);

      }

    
   
  }
  delay(3);
}
