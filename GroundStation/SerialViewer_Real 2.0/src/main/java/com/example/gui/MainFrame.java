package com.example.gui;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.Toolkit;

import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.SwingConstants;

import com.example.GroundStation.SerialReader;
import com.example.Telemetry.DataPanel;

public class MainFrame extends JFrame {
    private final DataPanel dataPanel = new DataPanel();

    public MainFrame() {
        // Set dynamic size based on screen
        Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
        int screenWidth = screenSize.width;
        int screenHeight = screenSize.height;

        int width = (int) (screenWidth * 2 / 3.0);
        int height = (int) (screenHeight);
        setSize(width, height);
        setLocationRelativeTo(null);
        setTitle("Telemetry Viewer");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BorderLayout());

        // ========== Header ==========
        JPanel headerPanel = new JPanel();
        headerPanel.setLayout(new BoxLayout(headerPanel, BoxLayout.X_AXIS));
        headerPanel.setPreferredSize(new Dimension(width, (int) (height / 8)));
        add(headerPanel, BorderLayout.NORTH);

        // Serial Viewer Box
        JLabel serialViewerLabel = new JLabel(" Serial Viewer ");
        serialViewerLabel.setOpaque(true);
        serialViewerLabel.setBackground(new Color(200, 200, 200));
        serialViewerLabel.setForeground(new Color(0, 0, 0));
        serialViewerLabel.setBorder(BorderFactory.createLineBorder(Color.BLACK));
        serialViewerLabel.setFont(new Font("Times New Roman", Font.BOLD, 20));
        serialViewerLabel.setHorizontalAlignment(SwingConstants.CENTER);
        serialViewerLabel.setPreferredSize(new Dimension((int) (width / 8), (int) (height / 8)));

        JPanel serialViewerContainer = new JPanel(new BorderLayout());
        serialViewerContainer.setPreferredSize(new Dimension((int) (width / 8), (int) (height / 8)));
        serialViewerContainer.setMaximumSize(new Dimension((int) (width / 8), (int) (height / 8)));
        serialViewerContainer.setMinimumSize(new Dimension((int) (width / 8), (int) (height / 8)));
        serialViewerContainer.setAlignmentY(SwingConstants.TOP);
        serialViewerContainer.setLayout(new FlowLayout(FlowLayout.CENTER, 0, 0));
        serialViewerContainer.setOpaque(false);
        serialViewerContainer.add(serialViewerLabel);

        // CanSat Header
        JPanel projectPanel = new JPanel();
        projectPanel.setBackground(new Color(200, 200, 200));
        projectPanel.setBorder(BorderFactory.createLineBorder(Color.BLACK));
        projectPanel.setLayout(new BoxLayout(projectPanel, BoxLayout.Y_AXIS));
        projectPanel.setPreferredSize(new Dimension((int) (width / 8) * 7, (int) (height / 8)));
        projectPanel.setMaximumSize(new Dimension((int) (width / 8) * 7, (int) (height / 8)));
        projectPanel.setMinimumSize(new Dimension((int) (width / 8) * 7, (int) (height / 8)));
        projectPanel.setAlignmentY(SwingConstants.RIGHT);

        JLabel titleLabel = new JLabel("CanSat - Sensor-sational Science", SwingConstants.CENTER);
        titleLabel.setFont(new Font("Times New Roman", Font.BOLD, 25));
        titleLabel.setAlignmentX(Component.CENTER_ALIGNMENT);

        JLabel namesLabel = new JLabel("SABATINI Paul | KOPS Lou | BOCK Louis | DURBACA Leila | SAND Yannik | LEJEUNE Charel", SwingConstants.CENTER);
        namesLabel.setFont(new Font("Times New Roman", Font.PLAIN, 15));
        namesLabel.setAlignmentX(Component.CENTER_ALIGNMENT);

        projectPanel.add(Box.createVerticalGlue());
        projectPanel.add(titleLabel);
        projectPanel.add(namesLabel);
        projectPanel.add(Box.createVerticalGlue());

        headerPanel.add(serialViewerContainer);
        headerPanel.add(projectPanel);

        // ========== Main Content (DataPanel) ==========
        add(dataPanel, BorderLayout.CENTER);

        // right after you add your DataPanel and before starting the reader:
        LogFrame logFrame = new LogFrame(this);
        new SerialReader(dataPanel, logFrame).start();
    }
}
