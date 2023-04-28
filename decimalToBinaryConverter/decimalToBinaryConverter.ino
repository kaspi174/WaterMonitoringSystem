void setup() {
  Serial.begin(9600);  // Initialize serial communication at 9600 bits per second
}

void loop() {
  int i, value;
  int array1[5], array2[5], array3[5], array4[5], array5[5];

  // Prompt user to enter 5 values
  Serial.println("Enter 5 values:");

  // Read in each value and store it in the corresponding array
  for (i = 0; i < 5; i++) {
    while (Serial.available() == 0) {
      // Wait for input
    }
    value = Serial.parseInt();
    array1[i] = value;

    while (Serial.available() == 0) {
      // Wait for input
    }
    value = Serial.parseInt();
    array2[i] = value;

    while (Serial.available() == 0) {
      // Wait for input
    }
    value = Serial.parseInt();
    array3[i] = value;

    while (Serial.available() == 0) {
      // Wait for input
    }
    value = Serial.parseInt();
    array4[i] = value;

    while (Serial.available() == 0) {
      // Wait for input
    }
    value = Serial.parseInt();
    array5[i] = value;
  }

  // Convert each value to binary and print it out
  for (i = 0; i < 5; i++) {
    Serial.print("Binary representation of ");
    Serial.print(array1[i]);
    Serial.print(": ");
    Serial.println(String(array1[i], BIN));

    Serial.print("Binary representation of ");
    Serial.print(array2[i]);
    Serial.print(": ");
    Serial.println(String(array2[i], BIN));

    Serial.print("Binary representation of ");
    Serial.print(array3[i]);
    Serial.print(": ");
    Serial.println(String(array3[i], BIN));

    Serial.print("Binary representation of ");
    Serial.print(array4[i]);
    Serial.print(": ");
    Serial.println(String(array4[i], BIN));

    Serial.print("Binary representation of ");
    Serial.print(array5[i]);
    Serial.print(": ");
    Serial.println(String(array5[i], BIN));

    Serial.println();
  }
}
