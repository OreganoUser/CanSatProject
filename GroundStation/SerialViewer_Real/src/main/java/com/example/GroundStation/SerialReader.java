package com.example.GroundStation;

import java.io.BufferedReader;
import java.io.InputStreamReader;

import com.example.Telemetry.DataPanel;
import com.fazecast.jSerialComm.SerialPort;

public class SerialReader extends Thread {
    private final DataPanel dataPanel;
    private boolean firstDataReceived = false;
    private boolean running = true;

    public SerialReader(DataPanel panel) {
        this.dataPanel = panel;
    }

    @Override
    public void run() {
        while (running) {
            SerialPort comPort = findPicoPort();

            if (comPort == null) {
                System.out.println("Waiting for device to be connected...");
                dataPanel.showStatusMessage("Waiting for device to be connected...");
                pause(1000);
                continue;
            }

            System.out.println("Connecting to " + comPort.getSystemPortName());
            dataPanel.showStatusMessage("Connecting...");
            comPort.setBaudRate(115200);
            // 2s blocking read timeout so we break out if no data arrives
            comPort.setComPortTimeouts(
                SerialPort.TIMEOUT_READ_BLOCKING,
                2000,   // read timeout in ms
                0
            );

            if (!comPort.openPort()) {
                System.err.println("Failed to open port. Retrying...");
                pause(1000);
                continue;
            }

            // Reset flag so status clears on first incoming line
            firstDataReceived = false;
            dataPanel.showStatusMessage("No data received yet!");
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
                System.out.println("SerialReader: read loop ended");
            } catch (Exception e) {
                System.err.println("Serial read error: " + e.getMessage());
            } finally {
                if (comPort.isOpen()) {
                    comPort.closePort();
                    System.out.println("Port closed, will retry connection");
                }
            }

            // Brief pause before attempting to reconnect
            pause(1000);
        }
    }


    private SerialPort findPicoPort() {
        for (SerialPort port : SerialPort.getCommPorts()) {
            if (port.getSystemPortName().contains("usbmodem")) {
                return port;
            }
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