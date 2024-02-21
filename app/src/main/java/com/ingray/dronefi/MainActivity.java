package com.ingray.dronefi;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    private ESP32Client esp32Client;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        esp32Client = new ESP32Client();

        // Initialize UI components
        Button takeoffButton = findViewById(R.id.takeoffButton);
        Button landButton = findViewById(R.id.landButton);
        Button emergencyStopButton = findViewById(R.id.emergencyStopButton);

        // Set onClick listeners for buttons
        takeoffButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // Send takeoff command to ESP32
                esp32Client.sendControlCommand("takeoff");
            }
        });

        landButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // Send land command to ESP32
                esp32Client.sendControlCommand("land");
            }
        });

        emergencyStopButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // Send emergency stop command to ESP32
                esp32Client.sendControlCommand("emergency_stop");
            }
        });

        // Add listeners for other controls (e.g., SeekBars) and send corresponding control commands
        SeekBar throttleSeekBar = findViewById(R.id.throttleSeekBar);
        SeekBar pitchSeekBar = findViewById(R.id.pitchSeekBar);
        SeekBar rollSeekBar = findViewById(R.id.rollSeekBar);
        SeekBar yawSeekBar = findViewById(R.id.yawSeekBar);

        // Set SeekBar listeners
        throttleSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                // Send throttle control command to ESP32
                esp32Client.sendControlCommand("throttle:" + progress);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {}

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {}
        });

        pitchSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                // Send pitch control command to ESP32
                esp32Client.sendControlCommand("pitch:" + progress);

            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {}

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {}
        });

        rollSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                // Send roll control command to ESP32
                esp32Client.sendControlCommand("roll:" + progress);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {}

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {}
        });

        yawSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                // Send yaw control command to ESP32
                esp32Client.sendControlCommand("yaw:" + progress);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {}

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {}
        });

    }
}