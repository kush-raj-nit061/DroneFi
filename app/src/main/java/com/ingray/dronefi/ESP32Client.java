package com.ingray.dronefi;
import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;

public class ESP32Client {

    private static final String ESP32_IP = "192.168.4.1"; // Replace with your ESP32 IP address
    private static final int ESP32_PORT = 80;

    public void sendControlCommand(String data) {
        new Thread(() -> {
            try {
                // Create a socket connection to the ESP32
                Socket socket = new Socket(ESP32_IP, ESP32_PORT);

                // Get the output stream of the socket
                OutputStream outputStream = socket.getOutputStream();

                // Send data to the ESP32
                outputStream.write(data.getBytes());
                outputStream.flush();

                // Close the output stream and socket
                outputStream.close();
                socket.close();
            } catch (UnknownHostException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }).start();
    }
    public void throttleControl(int value) {
        sendControlCommand("throttle:" + value);
    }

    public void pitchControl(int value) {
        sendControlCommand("pitch:" + value);
    }

    public void rollControl(int value) {
        sendControlCommand("roll:" + value);
    }

    public void yawControl(int value) {
        sendControlCommand("yaw:" + value);
    }

    public void takeoff() {
        sendControlCommand("takeoff");
    }

    public void land() {
        sendControlCommand("land");
    }

    public void emergencyStop() {
        sendControlCommand("emergency_stop");
    }
}