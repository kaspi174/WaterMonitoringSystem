void setup() {
  Serial.begin(9600);  // Initialize serial communication at 9600 bits per second
}

void loop() {
  int decimalValue;
  int bitArray[8] = {0};
  int i = 7;
  
  // Prompt user to enter a decimal value between 0 and 255
  Serial.println("Enter a decimal value between 0 and 255:");
  
  // Read the input value from the serial monitor
  while (Serial.available() == 0) {
    // Wait for input
  }
  decimalValue = Serial.parseInt();

  // Check if the input value is valid
  if (decimalValue < 0 || decimalValue > 255) {
    Serial.println("Invalid input.");
    return;
  }

  // Convert the decimal value to binary and store it in the bitArray
  while (decimalValue > 0) {
    bitArray[i] = decimalValue % 2;
    decimalValue /= 2;
    i--;
  }

  // Print the binary value to the serial monitor
  Serial.print("Binary value: ");
  for (i = 0; i < 8; i++) {
    Serial.print(bitArray[i]);
  }
  Serial.println();
}
