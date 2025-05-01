package com.example.gui;

import javax.swing.*;
import java.awt.*;

public class LogFrame extends JFrame {
    private final DefaultListModel<String> listModel = new DefaultListModel<>();

    public LogFrame(JFrame parent) {
        super("Serial Log");
        setLayout(new BorderLayout());

        // JList to display each log line
        JList<String> logList = new JList<>(listModel);
        add(new JScrollPane(logList), BorderLayout.CENTER);

        // size: full height of parent, 1/3 width
        Dimension parentSize = parent.getSize();
        setSize(parentSize.width/3, parentSize.height);

        // position just to the right of parent
        Point loc = parent.getLocation();
        setLocation(loc.x + parentSize.width, loc.y);

        setVisible(true);
    }

    /** call this from any thread to append a line */
    public void log(String message) {
        SwingUtilities.invokeLater(() -> {
            listModel.addElement(message);
        });
    }
}
