/*
  Text-to-Morse converter

  Translates input text into morse code voltage out.

  Input is handled via the Serial interface, text must be newline terminated.
  Supported characters are in the `encoding` struct.

  Output is digital voltage high to PIN. 13 has an LED most of the time.

  (c) Kyle Snavely 2016
*/

// Pin 13 has an LED connected on most Arduino boards.
int PIN = 13;
int TUNIT = 75; // ms, base time unit
String TEXT = "I LOVE EMILY";

// Text to morse map, ITU, symbols from wikipedia
static const struct {const char letter, *morse;} encoding[] =
{
  { 'A', ".-" },
  { 'B', "-..." },
  { 'C', "-.-." },
  { 'D', "-.." },
  { 'E', "." },
  { 'F', "..-." },
  { 'G', "--." },
  { 'H', "...." },
  { 'I', ".." },
  { 'J', ".---" },
  { 'K', ".-.-" },
  { 'L', ".-.." },
  { 'M', "--" },
  { 'N', "-." },
  { 'O', "---" },
  { 'P', ".--." },
  { 'Q', "--.-" },
  { 'R', ".-." },
  { 'S', "..." },
  { 'T', "-" },
  { 'U', "..-" },
  { 'V', "...-" },
  { 'W', ".--" },
  { 'X', "-..-" },
  { 'Y', "-.--" },
  { 'Z', "--.." },
  { '1', ".----" },
  { '2', "..---" },
  { '3', "...--" },
  { '4', "....-" },
  { '5', "....." },
  { '6', "-...." },
  { '7', "--..." },
  { '8', "---.." },
  { '9', "----." },
  { '0', "-----" },
  { '.', "·–·–·–" },
  { ',', "--..--" },
  { '?', "..--.." },
  { '\'', ".----."},
  { '!', "-.-.--" },
  { '/', "-..-." },
  { '(', "-.--." },
  { ')', "-.--.-" },
  { '&', ".-..." },
  { ':', "---..." },
  { ';', "-.-.-." },
  { '=', "-...-" },
  { '+', ".-.-." },
  { '-', "-....-" },
  { '_', "..__._" },
  { '$', "...-..-"},
  { '"', ".-..-." },
  { '@', ".--.-." },
  { ' ', "     " } //word gap is seven time units
};

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect.
  }
  // initialize the digital pin as an output.
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, LOW);
  Serial.flush();
  Serial.println("Setup complete...");
  Serial.println("\nWaiting for input text...");
}

void loop() {
  // Wait for serial input, terminated by a newline
  while (Serial.available() > 0) {
    // Load the input
    String text;
    text = Serial.readStringUntil('\n');
	text.toUpperCase();
	Serial.println("Text: " + text);

    // Convert to morse code
	String morse = ascii_to_morse(text);
	Serial.println("Morse: " + morse);

    // Transmit morse on the configured pin
	int i;
	for (i = 0; i < morse.length(); i++){
	  handle_char(morse[i]);
	}

    Serial.println("\nWaiting for input text...");
  }
}

/* Durations of dot, dash, word, and char gap are international convention */

void blink_long(){
  /* Do a long led blink */
  digitalWrite(PIN, HIGH);
  delay(3*TUNIT);
  digitalWrite(PIN, LOW);
  delay(TUNIT);
}

void blink_short(){
  /* Do a short led blink */
  digitalWrite(PIN, HIGH);
  delay(TUNIT);
  digitalWrite(PIN, LOW);
  delay(TUNIT);
}

void word_gap(){
  /* Go low for 7 units */
  digitalWrite(PIN, LOW);
  delay(7*TUNIT);
}

void char_gap(){
  /* Go low for 3 units */
  digitalWrite(PIN, LOW);
  delay(3*TUNIT);
}

/* Wait on the loop just for ease of watching blinks */

void loop_gap(){
  /* Go low for 28 units */
  digitalWrite(PIN, LOW);
  delay(28*TUNIT);
}

/* Given a dot, dash, or blank, blink and wait appropriately */
void handle_char(char c){
  if (c == '.')
  {
    blink_short();
    char_gap();
  }
  else if (c == '-')
  {
    blink_long();
    char_gap();
  }
  else if (c == ' ')
  {
    char_gap();
  }
}

String ascii_to_morse(String text){
  int i;
  int j;
  String morse = "";
  int encoding_size = sizeof encoding / sizeof *encoding;
  for (i = 0; i < text.length(); i++){
    for (j = 0; j < encoding_size; ++j){
      if (text[i] == encoding[j].letter){
        morse += encoding[j].morse;
        break;
      }
    }
    morse += " ";
  }
  return morse;
}
