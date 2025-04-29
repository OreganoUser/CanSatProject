// package com.example.Diagram;

// import gov.nasa.worldwind.BasicModel;
// import gov.nasa.worldwind.awt.WorldWindowGLCanvas;
// import gov.nasa.worldwind.geom.Position;
// import gov.nasa.worldwind.layers.RenderableLayer;
// import gov.nasa.worldwind.render.Path;
// import gov.nasa.worldwind.render.PointPlacemark;
// import gov.nasa.worldwind.render.PointPlacemarkAttributes;

// import javax.swing.JFrame;
// import java.awt.BorderLayout;
// import java.util.ArrayList;
// import java.util.List;

// public class MapFrame extends JFrame {
//     private final WorldWindowGLCanvas wwd;
//     private final RenderableLayer placemarkLayer;
//     private final RenderableLayer pathLayer;
//     private final List<Position> trackPositions = new ArrayList<>();

//     private PointPlacemark canMarker;
//     private PointPlacemark targetMarker;
//     private Path trackPath;

//     public MapFrame(double initLat, double initLon, double targetLat, double targetLon) {
//         super("Live CanSat Map View");
//         setDefaultCloseOperation(DISPOSE_ON_CLOSE);
//         setSize(800, 600);
//         setLayout(new BorderLayout());

//         // 1) WorldWind canvas & model
//         wwd = new WorldWindowGLCanvas();
//         wwd.setModel(new BasicModel());
//         add(wwd, BorderLayout.CENTER);

//         // 2) Layers
//         placemarkLayer = new RenderableLayer();
//         placemarkLayer.setName("Placemarks");
//         pathLayer      = new RenderableLayer();
//         pathLayer.setName("Track Path");
//         wwd.getModel().getLayers().add(placemarkLayer);
//         wwd.getModel().getLayers().add(pathLayer);

//         // 3) CanSat marker
//         Position initPos = Position.fromDegrees(initLat, initLon, 0);
//         canMarker = new PointPlacemark(initPos);
//         PointPlacemarkAttributes canAttrs = new PointPlacemarkAttributes();
//         canAttrs.setUsePointAsDefaultImage(true);
//         canMarker.setAttributes(canAttrs);
//         canMarker.setLabelText("CanSat");
//         placemarkLayer.addRenderable(canMarker);

//         // 4) Target marker
//         Position targetPos = Position.fromDegrees(targetLat, targetLon, 0);
//         targetMarker = new PointPlacemark(targetPos);
//         PointPlacemarkAttributes tgtAttrs = new PointPlacemarkAttributes();
//         tgtAttrs.setUsePointAsDefaultImage(true);
//         targetMarker.setAttributes(tgtAttrs);
//         targetMarker.setLabelText("Target");
//         placemarkLayer.addRenderable(targetMarker);

//         // 5) Track path
//         trackPath = new Path(trackPositions);
//         pathLayer.addRenderable(trackPath);

//         setLocationRelativeTo(null);
//         setVisible(true);
//     }

//     /** Move marker, extend path, redraw */
//     public void updatePosition(double lat, double lon) {
//         Position pos = Position.fromDegrees(lat, lon, 0);
//         canMarker.setPosition(pos);
//         trackPositions.add(pos);
//         trackPath.setPositions(trackPositions);
//         wwd.redraw();
//     }
// }
