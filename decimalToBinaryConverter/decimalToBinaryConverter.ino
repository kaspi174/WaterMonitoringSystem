void setup() {
  Serial.begin(9600);  // Initialize serial communication at 9600 bits per second
}

void loop() {
  int i, value;
  uint8_t array1[5], array2[5], array3[5], array4[5], array5[5]; // Use uint8_t data type to ensure values fit within one byte

  // Prompt user to enter 5 values
  Serial.println("Enter 5 values (0-255):");

  // Read in each value and store it in the corresponding array
  for (i = 0; i < 5; i++) {
    while (Serial.available() == 0) {
      // Wait for input
    }
    value = Serial.parseInt();
    array1[i] = constrain(value, 0, 255); // Constrain the value to fit within one byte (0-255)

    while (Serial.available() == 0) {
      // Wait for input
    }
    value = Serial.parseInt();
    array2[i] = constrain(value, 0, 255);

    while (Serial.available() == 0) {
      // Wait for input
    }
    value = Serial.parseInt();
    array3[i] = constrain(value, 0, 255);

    while (Serial.available() == 0) {
      // Wait for input
    }
    value = Serial.parseInt();
    array4[i] = constrain(value, 0, 255);

    while (Serial.available() == 0) {
      // Wait for input
    }
    value = Serial.parseInt();
    array5[i] = constrain(value, 0, 255);
  }

  // Convert each value to binary and combine them into a single binary string
  String binaryString = "";
  for (i = 0; i < 5; i++) {
    binaryString += String(array1[i], BIN);
    binaryString += String(array2[i], BIN);
    binaryString += String(array3[i], BIN);
    binaryString += String(array4[i], BIN);
    binaryString += String(array5[i], BIN);
  }

  // Convert the binary string to hexadecimal and print it out
  String hexString = "";
  for (i = 0; i < binaryString.length(); i += 4) {
    String nibble = binaryString.substring(i, i + 4);
    hexString += String(nibble.toInt(), HEX);
  }

  Serial.print("Hexadecimal representation of binary values: 0x");
  Serial.println(hexString);
}
