void setup() {
  Serial.begin(115200);

}

void loop() {
  Serial.print(analogRead(A1));
  Serial.print(" , ");
  Serial.println(analogRead(A2));
}
