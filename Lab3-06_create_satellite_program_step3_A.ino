#include "src/HeptaSat.h"

HeptaCdh cdh;
HeptaEps eps;

const float temperature = 25.0; // Temperature in degrees Celsius

bool is_cmd_received(void);
char get_cmd(void);

void setup() {
  cdh.begin();
  eps.init();
  cdh.wait_for_serial();
  cdh.wait_for_sd();
}

void loop() {
  cdh.println("------------------------------");
  cdh.printf("Satellite Time: %.2f seconds\n", millis() / 1000.0); // Print time in seconds
  cdh.printf("Battery Voltage: %.2f V\n", eps.get_battery_voltage());
  cdh.printf("Temperature: %.2f °C\n", temperature);
  cdh.println("------------------------------");

  if (is_cmd_received()) {
    char cmd = get_cmd();

    if (cmd != '\0') {
      cdh.printf("Received command: %c\n", cmd);

      // Process the command and respond accordingly
      switch (cmd) {
        case 'a': {
          for(uint8_t i = 0; i < 10; i++) {
            cdh.println("Hello Hepta sat lite");
            delay(1000);
          }
          break;
        }

        case 'b': {
          File file = cdh.create_file("test.txt");
          if (file) {
            for(uint8_t i = 0; i < 10; i++) {
              float battery_voltage = eps.get_battery_voltage();
              cdh.printf_file(file, "Battery Voltage: %f V\r\n", battery_voltage);
              delay(1000);
            }
            file.close();

            file = cdh.open_file("test.txt");
            while (file && file.available()) {
              cdh.write(cdh.read_file(file));
            }
            file.close();
          } else {
            cdh.printf("Failed to create file on SD card.\r\n");
            break;
          }
          break;
        }

        default:
          cdh.println("Unknown command");
          break;
      }
    }
  }
  delay(1000);
}

bool is_cmd_received(void) {
  // Check if there is any data available in the serial buffer
  return Serial.available() > 0;
}

char get_cmd(void) {
  // Read one command character and ignore line endings from Serial Monitor.
  while (is_cmd_received()) {
    char received_char = Serial.read();
    if (received_char != '\r' && received_char != '\n') {
      return received_char;
    }
  }
  return '\0'; // Return null character if no data is available
}
