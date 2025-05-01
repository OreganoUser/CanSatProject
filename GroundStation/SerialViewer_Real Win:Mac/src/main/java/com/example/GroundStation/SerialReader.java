package com.example.GroundStation;

import com.example.gui.LogFrame;
import java.io.BufferedReader;
import java.io.InputStreamReader;

import com.example.Telemetry.DataPanel;
import com.fazecast.jSerialComm.SerialPort;

public class SerialReader extends Thread {
    private final DataPanel dataPanel;
    private final LogFrame logFrame;
    private boolean firstDataReceived = false;
    private boolean running = true;

    public SerialReader(DataPanel panel, LogFrame logFrame) {
        this.dataPanel = panel;
        this.logFrame = logFrame;
    }

    @Override
    public void run() {
        while (running) {
            SerialPort comPort = findPicoPort();

            if (comPort == null) {
                logFrame.log("[INFO]: Waiting for device to be connected...");
                dataPanel.showStatusMessage("Waiting for device to be connected...");
                pause(1000);
                continue;
            }

            logFrame.log("[INFO]: Connecting to " + comPort.getSystemPortName());
            dataPanel.showStatusMessage("Connecting...");
            comPort.setBaudRate(115200);
            // 2s blocking read timeout so we break out if no data arrives
            comPort.setComPortTimeouts(
                SerialPort.TIMEOUT_READ_BLOCKING,
                2000,   // read timeout in ms
                0
            );

            if (!comPort.openPort()) {
                logFrame.log("[ERROR]: Failed to open port. Retrying...");
                pause(1000);
                continue;
            }

            // Reset flag so status clears on first incoming line
            firstDataReceived = false;
            logFrame.log("[INFO]: No data received yet!");
            dataPanel.showStatusMessage("No data received yet!");
            logFrame.log("[INFO]: Port opened, waiting for data...");
            System.out.println("Port opened, waiting for data...");

            try (BufferedReader reader = 
                    new BufferedReader(new InputStreamReader(comPort.getInputStream()))) {

                String line;
                // Read until null (port closed or error) or running stops
                while (running && comPort.isOpen() && (line = reader.readLine()) != null) {
                    line = line.trim();
                    if (line.isEmpty()) {
                        continue;
                    }
                    if (!firstDataReceived) {
                        firstDataReceived = true;
                        dataPanel.clearStatusMessage();
                    }
                    dataPanel.handleSerialLine(line);
                }
                logFrame.log("[INFO]: SerialReader: read loop ended.");
            } catch (Exception e) {
                logFrame.log("[ERROR]: Serial read error: " + e.getMessage());
            } finally {
                if (comPort.isOpen()) {
                    comPort.closePort();
                    logFrame.log("[INFO]: Port closed, will retry connection.");
                }
            }

            // Brief pause before attempting to reconnect
            pause(1000);
        }
    }


    private SerialPort findPicoPort() {
        String os = System.getProperty("os.name").toLowerCase();
    
        for (SerialPort port : SerialPort.getCommPorts()) {
            String name = port.getSystemPortName().toLowerCase();
    
            if (os.contains("mac")) {
                // macOS: Pico shows up as tty.usbmodem*
                if (name.contains("usbmodem") || name.contains("tty.usbmodem"))
                    return port;
            }
            else if (os.contains("win")) {
                // Windows: Pico appears as COMx
                if (name.startsWith("com"))
                    return port;
            }
            // (optional) else if Linux: look for ttyACM*
        }
    
        return null;
    }

    private void pause(long ms) {
        try {
            Thread.sleep(ms);
        } catch (InterruptedException ignored) {}
    } 

    public void stopReader() {
        running = false;
    }
}