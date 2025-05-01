package com.example.Telemetry;

import java.awt.Toolkit;
import java.awt.BorderLayout;
import java.awt.CardLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JProgressBar;
import javax.swing.JScrollPane;
import javax.swing.JSeparator;
import javax.swing.SwingConstants;
import javax.swing.SwingUtilities;

import org.jfree.data.xy.XYSeriesCollection;

import com.example.Diagram.TelemetryGraphFrame;

public class DataPanel extends JPanel {

    private final Map<String, TelemetryGraphFrame> graphFrames = new HashMap<>();
    private double prevAltitude = Double.NaN;
    private double prevTime = Double.NaN;

    private final CardLayout cardLayout = new CardLayout();
    private final JPanel cardPanel = new JPanel(cardLayout);
    private final JLabel workingTimeLabel = new JLabel("Working Time: 0 seconds", SwingConstants.CENTER);
    private final JLabel flightStageLabel = new JLabel("Flight Stage: Unknown", SwingConstants.CENTER);
    private final JLabel rssiLabel = new JLabel("RSSI (Signal Strength)", SwingConstants.CENTER);
    private final JProgressBar flightStageBar = new JProgressBar(0, 4);
    private final JProgressBar rssiBar = new JProgressBar(-100, 0);
    private final JLabel statusLabel = new JLabel("", SwingConstants.CENTER);
    private final JLabel countdownLabel = new JLabel("", SwingConstants.CENTER);
    private final Map<String, JLabel> labels = new HashMap<>();
    private boolean dataStarted = false;

    private static final String CSV_FILE = "telemetry_data.csv";
    private BufferedWriter csvWriter;

    private final String[][] fields = {
        {"Packet Number", ""},
        {"Temperature", "°C"},
        {"Pressure", "hPa"},
        {"Altitude (Pressure)", "m"},
        {"Humidity", "%"},
        {"GPS Fix", ""},
        {"Latitude", "°"},
        {"Longitude", "°"},
        {"Altitude (GPS)", "m"},
        {"Speed", "m/s"},
        {"Vertical Descend Speed", "m/s"},
        {"Angle", "°"},
        {"ax", "m/s^2"},
        {"ay", "m/s^2"},
        {"az", "m/s^2"},
        {"gx", "rad/s"},
        {"gy", "rad/s"},
        {"gz", "rad/s"},
        {"mx", "μT"},
        {"my", "μT"},
        {"mz", "μT"},
        {"Magheading", "°"},
        {"Yaw", "°"},
        {"Roll", "°"},
        {"Pitch", "°"}
    };

    public DataPanel() {
        setLayout(new BorderLayout());
        initStartupScreen();
        initTelemetryScreen();

        // CSV setup
        try {
            File csv = new File(CSV_FILE);
            boolean isNew = csv.createNewFile();
            csvWriter = new BufferedWriter(new FileWriter(csv, true));
            if (isNew) {
                csvWriter.write(
                        "PacketNumber,Timestamp,Temperature,Pressure,"
                        + "AltitudePressure,Humidity,GPSFix,Latitude,Longitude,"
                        + "AltitudeGPS,SpeedKnots,Angle,ax,ay,az,"
                        + "gx,gy,gz,mx,my,mz,Magheading,yaw,roll,pitch,flightStage,RSSI"
                );
                csvWriter.newLine();
                csvWriter.flush();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        add(cardPanel, BorderLayout.CENTER);
        showStatusMessage("No Device Connected!");
    }

    private void initStartupScreen() {
        JPanel startupPanel = new JPanel(new GridBagLayout());
        startupPanel.setBackground(new Color(200, 200, 200));
        startupPanel.setBorder(BorderFactory.createLineBorder(Color.BLACK));
        statusLabel.setForeground(Color.RED);
        statusLabel.setFont(new Font("Arial", Font.BOLD, 24));
        countdownLabel.setFont(new Font("Arial", Font.PLAIN, 18));
        Box box = Box.createVerticalBox();
        box.add(statusLabel);
        box.add(Box.createVerticalStrut(10));
        box.add(countdownLabel);
        startupPanel.add(box);
        cardPanel.add(startupPanel, "startup");
    }

    private void initTelemetryScreen() {
        JPanel root = new JPanel(new BorderLayout());
        root.setBackground(new Color(245, 245, 245));

        // Top overview + status
        JPanel overview = new JPanel();
        overview.setLayout(new BoxLayout(overview, BoxLayout.Y_AXIS));
        overview.setBackground(new Color(245, 245, 245));
        overview.setBorder(BorderFactory.createEmptyBorder(10, 20, 10, 20));

        JButton showAll = new JButton("Show All");
        showAll.setAlignmentX(Component.LEFT_ALIGNMENT);
        showAll.addActionListener(e -> openAllGraphs());
        overview.add(showAll);
        overview.add(Box.createVerticalStrut(10));

        JLabel overLabel = new JLabel("Overview", SwingConstants.CENTER);
        overLabel.setFont(new Font("Arial Black", Font.BOLD, 40));
        overLabel.setAlignmentX(Component.CENTER_ALIGNMENT);
        overview.add(overLabel);
        overview.add(new JSeparator(SwingConstants.HORIZONTAL));
        overview.add(Box.createVerticalStrut(10));

        workingTimeLabel.setFont(new Font("Arial", Font.BOLD, 30));
        workingTimeLabel.setAlignmentX(Component.CENTER_ALIGNMENT);
        overview.add(workingTimeLabel);

        // Status
        JPanel status = new JPanel();
        status.setLayout(new BoxLayout(status, BoxLayout.Y_AXIS));
        status.setBackground(new Color(245, 245, 245));
        status.setBorder(BorderFactory.createEmptyBorder(10, 20, 10, 20));

        flightStageLabel.setFont(new Font("Arial", Font.BOLD, 14));
        flightStageLabel.setAlignmentX(Component.LEFT_ALIGNMENT);
        status.add(flightStageLabel);
        Dimension bs = new Dimension(200, 25);
        flightStageBar.setPreferredSize(bs);
        flightStageBar.setMaximumSize(new Dimension(Integer.MAX_VALUE, bs.height));
        status.add(flightStageBar);
        status.add(Box.createVerticalStrut(10));

        rssiLabel.setFont(new Font("Arial", Font.BOLD, 14));
        rssiLabel.setAlignmentX(Component.LEFT_ALIGNMENT);
        status.add(rssiLabel);
        rssiBar.setPreferredSize(bs);
        rssiBar.setMaximumSize(new Dimension(Integer.MAX_VALUE, bs.height));
        status.add(rssiBar);

        JPanel top = new JPanel();
        top.setLayout(new BoxLayout(top, BoxLayout.Y_AXIS));
        top.setBackground(new Color(245, 245, 245));
        top.add(overview);
        top.add(status);
        root.add(top, BorderLayout.NORTH);

        // Grid of values
        JPanel grid = new JPanel(new GridLayout(0, 3, 15, 15));
        grid.setBackground(new Color(240, 240, 240));
        grid.setBorder(BorderFactory.createEmptyBorder(20, 20, 20, 20));
        for (String[] field : fields) {
            String key = field[0], unit = field[1];
            JLabel lbl = new JLabel(key + ":", SwingConstants.CENTER);
            lbl.setFont(new Font("Arial", Font.BOLD, 16));
            JLabel val = new JLabel("N/A " + unit, SwingConstants.CENTER);
            val.setFont(new Font("Arial", Font.PLAIN, 14));
            JPanel cell = new JPanel(new BorderLayout());
            cell.setBackground(new Color(220, 220, 220));
            cell.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
            cell.add(lbl, BorderLayout.NORTH);
            cell.add(val, BorderLayout.CENTER);
            addGraphButtonIfNeeded(cell, key);
            labels.put(key, val);
            grid.add(cell);
        }
        root.add(new JScrollPane(grid), BorderLayout.CENTER);
        cardPanel.add(root, "overview");
        add(cardPanel, BorderLayout.CENTER);
    }

    private void openAllGraphs() {
        String[] keys = {"Temperature", "Altitude (Pressure)", "Altitude (GPS)", "Speed", "Vertical Descend Speed"};
        for (String k : keys) {
            XYSeriesCollection ds;
            switch (k) {
                case "Temperature":
                    ds = SharedTelemetryModel.getDataset(SharedTelemetryModel.temperatureSeries);
                    break;
                case "Altitude (Pressure)":
                    ds = SharedTelemetryModel.getDataset(SharedTelemetryModel.pressureAltitudeSeries);
                    break;
                case "Altitude (GPS)":
                    ds = SharedTelemetryModel.getDataset(SharedTelemetryModel.altitudeSeries);
                    break;
                case "Speed":
                    ds = SharedTelemetryModel.getDataset(SharedTelemetryModel.horizontalSpeedSeries);
                    break;
                case "Vertical Descend Speed":
                    ds = SharedTelemetryModel.getDataset(SharedTelemetryModel.verticalSpeedSeries);
                    break;
                default:
                    continue;
            }
            TelemetryGraphFrame f = graphFrames.get(k);
            if (f == null || !f.isDisplayable()) {
                f = new TelemetryGraphFrame(k + " vs Time", ds);
                graphFrames.put(k, f);
            } else {
                f.setVisible(true);
                f.toFront();
            }
        }

        arrangeGraphFrames();
    }

    /**
     * Evenly tiles all visible TelemetryGraphFrame windows
     * in a rows×cols grid across the entire screen.
     */

    private void arrangeGraphFrames() {
        // count live frames
        java.util.List<TelemetryGraphFrame> frames = new java.util.ArrayList<>();
        for (TelemetryGraphFrame f : graphFrames.values()) {
            if (f.isDisplayable()) frames.add(f);
        }
        int n = frames.size();
        if (n == 0) return;

        // compute rows & cols (near square)
        int rows = (int) Math.ceil(Math.sqrt(n));
        int cols = (int) Math.ceil((double) n / rows);

        // screen size
        Dimension screen = Toolkit.getDefaultToolkit().getScreenSize();
        int fw = screen.width  / cols;
        int fh = screen.height / rows;

        // position each frame
        for (int i = 0; i < frames.size(); i++) {
            int r = i / cols;
            int c = i % cols;
            TelemetryGraphFrame f = frames.get(i);
            f.setSize(fw, fh);
            f.setLocation(c * fw, r * fh);
        }
    }


    private void addGraphButtonIfNeeded(JPanel cell, String key) {
        if (key.equals("Temperature") || key.equals("Altitude (Pressure)") || key.equals("Altitude (GPS)") || key.equals("Speed") || key.equals("Vertical Descend Speed")) {
            JButton btn = new JButton("Graph");
            btn.addActionListener(e -> {
                XYSeriesCollection ds;
                switch (key) {
                    case "Temperature":
                        ds = SharedTelemetryModel.getDataset(SharedTelemetryModel.temperatureSeries);
                        break;
                    case "Altitude (Pressure)":
                        ds = SharedTelemetryModel.getDataset(SharedTelemetryModel.pressureAltitudeSeries);
                        break;
                    case "Altitude (GPS)":
                        ds = SharedTelemetryModel.getDataset(SharedTelemetryModel.altitudeSeries);
                        break;
                    case "Speed":
                        ds = SharedTelemetryModel.getDataset(SharedTelemetryModel.horizontalSpeedSeries);
                        break;
                    case "Vertical Descend Speed":
                        ds = SharedTelemetryModel.getDataset(SharedTelemetryModel.verticalSpeedSeries);
                        break;
                    default:
                        return;
                }
                TelemetryGraphFrame f = graphFrames.get(key);
                if (f == null || !f.isDisplayable()) {
                    f = new TelemetryGraphFrame(key + " vs Time", ds);
                    graphFrames.put(key, f);
                } else {
                    f.setVisible(true);
                    f.toFront();
                }
            });
            cell.add(btn, BorderLayout.SOUTH);
        }
    }

    public void showStatusMessage(String msg) {
        SwingUtilities.invokeLater(() -> {
            statusLabel.setText(msg);
            cardLayout.show(cardPanel, "startup");
        });
    }

    public void clearStatusMessage() {
        SwingUtilities.invokeLater(() -> cardLayout.show(cardPanel, "overview"));
    }

    public void handleSerialLine(String line) {
        String[] parts = line.split(",");
        if (parts.length < 27) {
            return;
        }
        try {
            csvWriter.write(line);
            csvWriter.newLine();
            csvWriter.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }
        if (!dataStarted) {
            dataStarted = true;
            clearStatusMessage();
        }

        int packetNumber = Integer.parseInt(parts[0]);
        double seconds = Double.parseDouble(parts[1]);
        float temp = Float.parseFloat(parts[2]);
        float pressure = Float.parseFloat(parts[3]);
        float altPressure = Float.parseFloat(parts[4]);
        int humidity = Integer.parseInt(parts[5]);
        int gpsFix = Integer.parseInt(parts[6]);
        float latitude = Float.parseFloat(parts[7]);
        float longitude = Float.parseFloat(parts[8]);
        float altGPS = Float.parseFloat(parts[9]);
        float speedKnots = Float.parseFloat(parts[10]);
        int angle = Integer.parseInt(parts[11]);
        float ax = Float.parseFloat(parts[12]), ay = Float.parseFloat(parts[13]), az = Float.parseFloat(parts[14]);
        float gx = Float.parseFloat(parts[15]), gy = Float.parseFloat(parts[16]), gz = Float.parseFloat(parts[17]);
        float mx = Float.parseFloat(parts[18]), my = Float.parseFloat(parts[19]), mz = Float.parseFloat(parts[20]);
        int magHeading = Integer.parseInt(parts[21]);
        float yaw = Float.parseFloat(parts[22]), roll = Float.parseFloat(parts[23]), pitch = Float.parseFloat(parts[24]);
        int flightStage = Integer.parseInt(parts[25]), rssi = Integer.parseInt(parts[26]);

        // time label
        if (seconds < 60) {
            workingTimeLabel.setText(String.format("Working Time: %.0f sec", seconds));
        } else {
            workingTimeLabel.setText(String.format("Working Time: %d min %d sec", (int) seconds / 60, (int) seconds % 60));
        }
        
        labels.get("Packet Number").setText("Nr." + packetNumber);
        labels.get("Temperature").setText(String.format("%.2f °C", temp));
        labels.get("Pressure").setText(String.format("%.2f hPa", pressure));
        labels.get("Altitude (Pressure)").setText(String.format("%.2f m", altPressure));
        labels.get("Humidity").setText(humidity + " %");
        updateGpsFixLabel(gpsFix == 1);
        labels.get("Latitude").setText(String.format("%.2f°", latitude));
        labels.get("Longitude").setText(String.format("%.2f°", longitude));
        // add series
        SharedTelemetryModel.addPressureAltitude(seconds, altPressure);
        SharedTelemetryModel.addTemperature(seconds, temp);
        SharedTelemetryModel.addAltitude(seconds, altGPS);
        if (!Double.isNaN(prevAltitude) && seconds - prevTime > 0) {
            double v = (prevAltitude - altGPS) / (seconds - prevTime);
            SharedTelemetryModel.addVerticalSpeed(seconds, v);
            labels.get("Vertical Descend Speed").setText(String.format("%.2f m/s", v));
        }
        prevAltitude = altGPS;
        prevTime = seconds;
        labels.get("Altitude (GPS)").setText(String.format("%.2f m", altGPS));
        double speedMs = speedKnots * 0.514444;
        SharedTelemetryModel.addHorizontalSpeed(seconds, speedMs);
        labels.get("Speed").setText(convertKnotsToMS(Float.toString(speedKnots)));
        labels.get("Angle").setText(angle + " °");
        labels.get("ax").setText(String.format("%.2f m/s^2", ax));
        labels.get("ay").setText(String.format("%.2f m/s^2", ay));
        labels.get("az").setText(String.format("%.2f m/s^2", az));
        labels.get("gx").setText(String.format("%.2f rad/s", gx));
        labels.get("gy").setText(String.format("%.2f rad/s", gy));
        labels.get("gz").setText(String.format("%.2f rad/s", gz));
        labels.get("mx").setText(String.format("%.2f μT", mx));
        labels.get("my").setText(String.format("%.2f μT", my));
        labels.get("mz").setText(String.format("%.2f μT", mz));
        labels.get("Magheading").setText(magHeading + " °");
        labels.get("Yaw").setText(String.format("%.2f °", yaw));
        labels.get("Roll").setText(String.format("%.2f °", roll));
        labels.get("Pitch").setText(String.format("%.2f °", pitch));
        flightStageBar.setValue(flightStage);
        switch (flightStage) {
            case -1:
                flightStageLabel.setText("Flight Stage: Uninitialized");
                break;
            case 0:
                flightStageLabel.setText("Flight Stage: Awaiting Launch");
                break;
            case 1:
                flightStageLabel.setText("Flight Stage: Launch Detected");
                break;
            case 2:
                flightStageLabel.setText("Flight Stage: Rocket Peak Reached");
                break;
            case 3:
                flightStageLabel.setText("Flight Stage: CanSat Falling");
                break;
            case 4:
                flightStageLabel.setText("Flight Stage: Near Ground");
                break;
            default:
                flightStageLabel.setText("Flight Stage: Unknown");
        }
        rssiBar.setValue(rssi);
        rssiLabel.setText("RSSI: " + rssi + " dBm");
        int g = (int) map(rssi, -100, 0, 0, 255);
        g = Math.max(0, Math.min(255, g));
        int r = 255 - g;
        r = Math.max(0, Math.min(255, r));
        rssiBar.setForeground(new Color(r, g, 0));
    }

    private String convertKnotsToMS(String knotsStr) {
        try {
            double knots = Double.parseDouble(knotsStr);
            double ms = knots * 0.514444;
            return String.format("%.2f m/s", ms);
        } catch (NumberFormatException e) {
            return "N/A";
        }
    }

    private void updateGpsFixLabel(boolean hasFix) {
        JLabel gpsLabel = labels.get("GPS Fix");
        gpsLabel.setText(hasFix ? "Yes" : "No");
        gpsLabel.setOpaque(true);
        gpsLabel.setBackground(hasFix ? Color.GREEN : Color.RED);
        gpsLabel.setForeground(Color.BLACK);
    }

    private double map(double value, double fromLow, double fromHigh, double toLow, double toHigh) {
        return toLow + (value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow);
    }
}
