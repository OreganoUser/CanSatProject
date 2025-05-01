// File: src/main/java/com/example/Diagram/TelemetryGraphFrame.java
package com.example.Diagram;

import java.text.DecimalFormat;
import javax.swing.JFrame;
import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.BasicStroke;
import java.awt.Color;
import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.NumberAxis;
import org.jfree.chart.plot.XYPlot;
import org.jfree.chart.renderer.xy.XYLineAndShapeRenderer;
import org.jfree.data.xy.XYSeriesCollection;
import org.jfree.chart.ChartUtils;
import java.io.File;
import java.io.IOException;

public class TelemetryGraphFrame extends JFrame {
    public TelemetryGraphFrame(String title, XYSeriesCollection dataset) {
        super(title);
        setDefaultCloseOperation(DISPOSE_ON_CLOSE);


        // Create the chart
        JFreeChart chart = ChartFactory.createXYLineChart(
            title,                  // chart title
            "Time (s)",             // x axis label
            title,                  // y axis label
            dataset
        );

        // Bold red line, no shapes
        XYPlot plot = chart.getXYPlot();
        XYLineAndShapeRenderer renderer = new XYLineAndShapeRenderer(true, false);
        renderer.setSeriesPaint(0, Color.RED);
        renderer.setSeriesStroke(0, new BasicStroke(2.0f));
        plot.setRenderer(renderer);

        // “Nice” rounded tick units
        NumberAxis domain = (NumberAxis) plot.getDomainAxis();
        domain.setStandardTickUnits(NumberAxis.createStandardTickUnits());
        NumberAxis range = (NumberAxis) plot.getRangeAxis();
        range.setStandardTickUnits(NumberAxis.createStandardTickUnits());

        // Auto‐range to fit data
        domain.setAutoRange(true);
        range.setAutoRange(true);
        range.setAutoRangeIncludesZero(false);

        // Plain decimal formatting (no scientific notation)
        DecimalFormat df = new DecimalFormat("0.##");
        domain.setNumberFormatOverride(df);
        range.setNumberFormatOverride(df);


        // Wrap in a ChartPanel
        ChartPanel chartPanel = new ChartPanel(chart);
        chartPanel.setPreferredSize(new Dimension(600, 400));
        add(chartPanel, BorderLayout.CENTER);

        pack();
        setLocationRelativeTo(null);
        setVisible(true);

        // Save chart as PNG
        try {
            String filename = title.replaceAll("\\s+", "_") + ".png";
            ChartUtils.saveChartAsPNG(new File(filename), chart, getWidth(), getHeight());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
