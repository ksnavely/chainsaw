/*
  Text-to-Morse converter

  Blinks morse of TEXT out on pin 13 LED.

  (c) Kyle Snavely 2016
*/

// Pin 13 has an LED connected on most Arduino boards.
int LED = 13;
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
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  Serial.println("Setup complete...");
}

void loop() {
  TEXT.toUpperCase();
  Serial.println("Text: " + TEXT);
  String morse = ascii_to_morse(TEXT);
  Serial.println("Morse: " + morse);
  int i;
  for (i = 0; i < morse.length(); i++){
    handle_char(morse[i]);
  }
}

/* Durations of dot, dash, word, and char gap are international convention */

void blink_long(){
  /* Do a long led blink */
  digitalWrite(LED, HIGH);
  delay(3*TUNIT);
  digitalWrite(LED, LOW);
  delay(TUNIT);
}

void blink_short(){
  /* Do a short led blink */
  digitalWrite(LED, HIGH);
  delay(TUNIT);
  digitalWrite(LED, LOW);
  delay(TUNIT);
}

void word_gap(){
  /* Go low for 7 units */
  digitalWrite(LED, LOW);
  delay(7*TUNIT);
}

void char_gap(){
  /* Go low for 3 units */
  digitalWrite(LED, LOW);
  delay(3*TUNIT);
}

/* Wait on the loop just for ease of watching blinks */

void loop_gap(){
  /* Go low for 28 units */
  digitalWrite(LED, LOW);
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
