#include <WiFi.h>

const char* ssid = "YourSSID";       // Replace with your Wi-Fi network SSID
const char* password = "YourPassword";   // Replace with your Wi-Fi network password
const int serverPort = 80;

WiFiServer server(serverPort);

// Pin configuration of motor and function to control motor
const int motor1Pin = 5; // Front-left motor
const int motor2Pin = 6; // Front-right motor
const int motor3Pin = 7; // Rear-left motor
const int motor4Pin = 8; // Rear-right motor

// Define the maximum and minimum throttle values
const int maxThrottle = 255; // Maximum throttle value (full speed)
const int minThrottle = 0;   // Minimum throttle value (stopped)

// Define PID parameters for each axis
double kp_pitch = 1.0;
double ki_pitch = 0.0;
double kd_pitch = 0.0;

// Define variables for PID control (error, integral, derivative)
double error_pitch, prev_error_pitch = 0, integral_pitch = 0, derivative_pitch;

// PID Controller function for pitch axis
double pidControllerPitch(double setpoint, double actual_value) {
    // Calculate error
    error_pitch = setpoint - actual_value;

    // Calculate integral
    integral_pitch += error_pitch;

    // Calculate derivative
    derivative_pitch = error_pitch - prev_error_pitch;
    prev_error_pitch = error_pitch;

    // Calculate PID control signal
    double pid_output = (kp_pitch * error_pitch) + (ki_pitch * integral_pitch) + (kd_pitch * derivative_pitch);

    return pid_output;
}

void adjustMotorSpeeds(int throttle, int pitch, int yaw, int roll) {
    // Calculate PID control signals for pitch axis
    double pid_pitch = pidControllerPitch(0, pitch); // Setpoint for pitch is 0 (level)

    // Adjust the speed of each motor based on the input values and PID control signals
    int motor1Speed = throttle - pid_pitch + yaw - roll;  // Front-left motor
    int motor2Speed = throttle + pid_pitch + yaw + roll;  // Front-right motor
    int motor3Speed = throttle + pid_pitch - yaw - roll;  // Rear-left motor
    int motor4Speed = throttle - pid_pitch - yaw + roll;  // Rear-right motor

    // Ensure motor speeds are within valid range
    motor1Speed = constrain(motor1Speed, minThrottle, maxThrottle);
    motor2Speed = constrain(motor2Speed, minThrottle, maxThrottle);
    motor3Speed = constrain(motor3Speed, minThrottle, maxThrottle);
    motor4Speed = constrain(motor4Speed, minThrottle, maxThrottle);

    // Adjust motor speeds using PWM
    analogWrite(motor1Pin, motor1Speed);
    analogWrite(motor2Pin, motor2Speed);
    analogWrite(motor3Pin, motor3Speed);
    analogWrite(motor4Pin, motor4Speed);
}

void takeoff() {
    // Set throttle to a predefined value for takeoff
    int takeoffThrottle = 150; // Example value, adjust as needed
    adjustMotorSpeeds(takeoffThrottle, 0, 0, 0);
}

void land() {
    // Set throttle to minimum value to land
    adjustMotorSpeeds(minThrottle, 0, 0, 0);
}

void emergencyStop() {
    // Set all motor speeds to minimum value to stop the drone immediately
    adjustMotorSpeeds(minThrottle, 0, 0, 0);
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    // Connect to Wi-Fi network
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected to WiFi");

    // Start the server
    server.begin();
    Serial.println("Server started");

    // Initialize motor control pins
    pinMode(motor1Pin, OUTPUT);
    pinMode(motor2Pin, OUTPUT);
    pinMode(motor3Pin, OUTPUT);
    pinMode(motor4Pin, OUTPUT);
}

void processControlCommand(const String& command) {
    // Split the command into parts based on delimiter (e.g., ':')
    int delimiterIndex = command.indexOf(':');
    if (delimiterIndex == -1) {
        // Invalid command format
        Serial.println("Invalid command format");
        return;
    }

    // Extract the control type and value from the command
    String controlType = command.substring(0, delimiterIndex);
    int controlValue = command.substring(delimiterIndex + 1).toInt();

    // Process the control command based on its type
    if (controlType.equals("throttle")) {
        // Adjust the drone's throttle
        adjustMotorSpeeds(controlValue, 0, 0, 0); // Only throttle adjustment
    } else if (controlType.equals("pitch")) {
        // Adjust the drone's pitch
        adjustMotorSpeeds(0, controlValue, 0, 0); // Only pitch adjustment
    } else if (controlType.equals("yaw")) {
        // Adjust the drone's yaw
        adjustMotorSpeeds(0, 0, controlValue, 0); // Only yaw adjustment
    } else if (controlType.equals("roll")) {
        // Adjust the drone's roll
        adjustMotorSpeeds(0, 0, 0, controlValue); // Only roll adjustment
    } else if (controlType.equals("takeoff")) {
        // Initiate drone takeoff
        // Implement takeoff() function here
        takeoff();
    } else if (controlType.equals("land")) {
        // Initiate drone landing
        // Implement land() function here
        land();
    } else if (controlType.equals("emergency_stop")) {
        // Perform emergency stop
        // Implement emergencyStop() function here
        emergencyStop();
    } else {
        // Unknown control type
        Serial.println("Unknown control type: " + controlType);
    }
}

void loop() {
    // Check if a client has connected
    WiFiClient client = server.available();
    if (!client) {
        return;
    }

    // Wait until the client sends some data
    while (!client.available()) {
        delay(1);
    }

    // Read the first line of the request
    String request = client.readStringUntil('\r');
    Serial.println(request);

    // Parse the command from the request
    if (request.indexOf("GET /control") != -1) {
        // Extract the control command from the request
        int commandIndex = request.indexOf("/control/") + 9; // Length of "/control/" is 9
        String controlCommand = request.substring(commandIndex);
        Serial.println("Received control command: " + controlCommand);

        // Process the control command
        processControlCommand(controlCommand);
    }

    // Respond to the client
    String response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: text/plain\r\n\r\n";
    response += "Command received successfully";
    client.print(response);

    // Wait a little before closing the connection
    delay(10);
    client.stop();
}