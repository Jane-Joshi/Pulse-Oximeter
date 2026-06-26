include <Wire.h>
include <Adafruit_GFX.h>
include <Adafruit_SSD1306.h>

define SCREEN_WIDTH 128  // OLED display width, in pixels
define SCREEN_HEIGHT 64  // OLED display height, in pixels
define OLED_RESET    -1   // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

define RED_LED_PIN 10      // Red LED connected to Pin 10
define IR_LED_PIN 9        // IR LED connected to Pin 9
define PHOTODIODE_PIN A0   // Photodiode output connected to Analog Pin A0

void setup() {
  // Initialize pin modes
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(IR_LED_PIN, OUTPUT);

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  
  display.display();
  delay(2000);  // Wait for display to initialize
  display.clearDisplay();

  // Start Serial Monitor
  Serial.begin(9600);
}

void loop() {
  // Switch on Red LED and read photodiode signal
  digitalWrite(RED_LED_PIN, HIGH);
  delay(10); // Give time for light to stabilize
  int redValue = analogRead(PHOTODIODE_PIN);  // Read photodiode output for red light
  digitalWrite(RED_LED_PIN, LOW);

  // Switch on IR LED and read photodiode signal
  digitalWrite(IR_LED_PIN, HIGH);
  delay(10); // Stabilize the light before reading
  int irValue = analogRead(PHOTODIODE_PIN);   // Read photodiode output for IR light
  digitalWrite(IR_LED_PIN, LOW);

  // Calculate SpO2 (simple estimation formula, for demo purposes)
  float ratio = (float)redValue / (float)irValue;
  int spo2 = 110 - (25 * ratio);  // Simplified SpO2 estimation

  // Display values on the OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Pulse Oximeter");
  
  display.setCursor(0, 20);
  display.print("Red Light: ");
  display.println(redValue);
  
  display.setCursor(0, 30);
  display.print("IR Light: ");
  display.println(irValue);

  display.setCursor(0, 40);
  display.print("SpO2: ");
  display.print(spo2);
  display.print("%");

  display.display(); // Update display

  // Log data in the Serial Monitor for debugging
  Serial.print("Red Light Value: ");
  Serial.print(redValue);
  Serial.print("  IR Light Value: ");
  Serial.print(irValue);
  Serial.print("  SpO2: ");
  Serial.println(spo2);

  delay(1000);  // Wait 1 second before the next measurement
}
