// File: src/main/java/com/example/Telemetry/SharedTelemetryModel.java
package com.example.Telemetry;

import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;

public class SharedTelemetryModel {
    // GPS‐derived altitude vs time
    public static final XYSeries altitudeSeries =
        new XYSeries("GPS Altitude (m)", true);
    // Pressure‐derived altitude vs time
    public static final XYSeries pressureAltitudeSeries =
        new XYSeries("Pressure Altitude (m)", true);
    // Temperature vs time
    public static final XYSeries temperatureSeries =
        new XYSeries("Temperature (°C)", true);
    // Horizontal speed vs time
    public static final XYSeries horizontalSpeedSeries =
        new XYSeries("Horizontal Speed (m/s)", true);
    // Vertical speed vs time
    public static final XYSeries verticalSpeedSeries =
        new XYSeries("Vertical Speed (m/s)", true);

    public static void addAltitude(double t, double v) {
        altitudeSeries.add(t, v);
    }

    public static void addPressureAltitude(double t, double v) {
        pressureAltitudeSeries.add(t, v);
    }

    public static void addTemperature(double t, double v) {
        temperatureSeries.add(t, v);
    }

    public static void addHorizontalSpeed(double t, double v) {
        horizontalSpeedSeries.add(t, v);
    }

    public static void addVerticalSpeed(double t, double v) {
        verticalSpeedSeries.add(t, v);
    }

    /** Returns a dataset containing exactly the given series */
    public static XYSeriesCollection getDataset(XYSeries s) {
        XYSeriesCollection ds = new XYSeriesCollection();
        ds.addSeries(s);
        return ds;
    }
}
