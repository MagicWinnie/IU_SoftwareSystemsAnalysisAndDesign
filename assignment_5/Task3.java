// Dmitriy Okoneshnikov
// d.okoneshnikov@innopolis.university

// * Flyweight:
// I am storing the existing instances of shapes in a hash map and access them by
// their type and colors, because the same colors are frequently used over and over.
// Also, the amount of colors is "finite" (3 integer values from 0 to 255), whereas
// the coordinates and thickness can be assigned to "infinite" (double values without
// custom restrictions) number of values. Storing shapes by these floating point values
// is pointless as it won't decrease the amount of objects we have.
// We could also store these shapes only by their type, so all the other values
// can be assigned later. This would result in an even better space optimization.
// * Visitor:
// I have an interface for visitors, so other visitors can be easily created.
// An ExportVisitor is implementing the interface and has visit methods for each
// shape type (these methods just print "exporting...").
// The export method in the base class Shape gets the Visitor in the parameter
// and calls the visit with current shape. This way new types of visitors and new
// shapes can be easily added.

import java.util.HashMap;
import java.util.Map;

class Color {
    private final int r;
    private final int g;
    private final int b;

    public Color(int r, int g, int b) {
        this.r = r;
        this.g = g;
        this.b = b;
    }

    public int getR() {
        return r;
    }

    public int getG() {
        return g;
    }

    public int getB() {
        return b;
    }

    @Override
    public String toString() {
        return "Color{" + r + "," + g + "," + b + '}';
    }
}

interface Visitor {
    void visit(Shape shape);
    void visit(Rectangle rectangle);
    void visit(Circle circle);
    void visit(Triangle triangle);
    void visit(Line line);
}

class ExportVisitor implements Visitor {
    @Override
    public void visit(Shape shape) {
        System.out.println("Exporting shape: " + shape.toString());
    }

    @Override
    public void visit(Rectangle rectangle) {
        System.out.println("Exporting rectangle: " + rectangle.toString());
    }

    @Override
    public void visit(Circle circle) {
        System.out.println("Exporting circle: " + circle.toString());
    }

    @Override
    public void visit(Triangle triangle) {
        System.out.println("Exporting triangle: " + triangle.toString());
    }

    @Override
    public void visit(Line line) {
        System.out.println("Exporting line: " + line.toString());
    }
}

abstract class Shape {
    protected final Color fillColor;
    protected final Color borderColor;
    protected double borderThickness;
    protected double coordinateX;
    protected double coordinateY;

    public Shape(Color fillColor, Color borderColor) {
        this.fillColor = fillColor;
        this.borderColor = borderColor;
    }

    void draw(double x, double y) {
        this.coordinateX = x;
        this.coordinateY = y;
        System.out.println("Drawing '" + getClass().getName() + "' at " + x + ", " + y);
    }

    void export(Visitor visitor) {
        visitor.visit(this);
    }

    public void setParams(double borderThickness) {
        this.borderThickness = borderThickness;
    }

    public void setBorderThickness(double borderThickness) {
        this.borderThickness = borderThickness;
    }

    public Color getFillColor() {
        return fillColor;
    }

    public Color getBorderColor() {
        return borderColor;
    }

    public double getBorderThickness() {
        return borderThickness;
    }

    public double getCoordinateX() {
        return coordinateX;
    }

    public double getCoordinateY() {
        return coordinateY;
    }

    @Override
    public String toString() {
        return "Shape{" +
                "fillColor=" + fillColor +
                ", borderColor=" + borderColor +
                ", borderThickness=" + borderThickness +
                ", coordinateX=" + coordinateX +
                ", coordinateY=" + coordinateY +
                ", hashCode=" + hashCode() +
                '}';
    }
}

class Rectangle extends Shape {
    private double length;
    private double width;

    public Rectangle(Color fillColor, Color borderColor) {
        super(fillColor, borderColor);
    }

    public void setParams(double borderThickness,
                          double length,
                          double width) {
        super.setParams(borderThickness);
        this.length = length;
        this.width = width;
    }

    public void setLength(double length) {
        this.length = length;
    }

    public void setWidth(double width) {
        this.width = width;
    }

    public double getLength() {
        return length;
    }

    public double getWidth() {
        return width;
    }

    @Override
    public String toString() {
        return "Rectangle{" +
                "length=" + length +
                ", width=" + width +
                ", fillColor=" + fillColor +
                ", borderColor=" + borderColor +
                ", borderThickness=" + borderThickness +
                ", coordinateX=" + coordinateX +
                ", coordinateY=" + coordinateY +
                ", hashCode=" + hashCode() +
                '}';
    }
}

class Circle extends Shape {
    private double radius;

    public Circle(Color fillColor, Color borderColor) {
        super(fillColor, borderColor);
    }

    public void setParams(double borderThickness,
                          double radius) {
        super.setParams(borderThickness);
        this.radius = radius;
    }

    public void setRadius(double radius) {
        this.radius = radius;
    }

    public double getRadius() {
        return radius;
    }

    @Override
    public String toString() {
        return "Circle{" +
                "radius=" + radius +
                ", fillColor=" + fillColor +
                ", borderColor=" + borderColor +
                ", borderThickness=" + borderThickness +
                ", coordinateX=" + coordinateX +
                ", coordinateY=" + coordinateY +
                ", hashCode=" + hashCode() +
                '}';
    }
}

class Triangle extends Shape {
    private double side1;
    private double side2;
    private double side3;

    public Triangle(Color fillColor, Color borderColor) {
        super(fillColor, borderColor);
    }

    public void setParams(double borderThickness,
                          double side1,
                          double side2,
                          double side3) {
        super.setParams(borderThickness);
        this.side1 = side1;
        this.side2 = side2;
        this.side3 = side3;
    }

    public void setSide1(double side1) {
        this.side1 = side1;
    }

    public void setSide2(double side2) {
        this.side2 = side2;
    }

    public void setSide3(double side3) {
        this.side3 = side3;
    }

    public double getSide1() {
        return side1;
    }

    public double getSide2() {
        return side2;
    }

    public double getSide3() {
        return side3;
    }

    @Override
    public String toString() {
        return "Triangle{" +
                "side1=" + side1 +
                ", side2=" + side2 +
                ", side3=" + side3 +
                ", fillColor=" + fillColor +
                ", borderColor=" + borderColor +
                ", borderThickness=" + borderThickness +
                ", coordinateX=" + coordinateX +
                ", coordinateY=" + coordinateY +
                ", hashCode=" + hashCode() +
                '}';
    }
}

class Line extends Shape {
    private double lineLength;

    public Line(Color fillColor, Color borderColor) {
        super(fillColor, borderColor);
    }

    public void setParams(double borderThickness,
                          double lineLength) {
        super.setParams(borderThickness);
        this.lineLength = lineLength;
    }

    public void setLineLength(double lineLength) {
        this.lineLength = lineLength;
    }

    public double getLineLength() {
        return lineLength;
    }

    @Override
    public String toString() {
        return "Line{" +
                "lineLength=" + lineLength +
                ", fillColor=" + fillColor +
                ", borderColor=" + borderColor +
                ", borderThickness=" + borderThickness +
                ", coordinateX=" + coordinateX +
                ", coordinateY=" + coordinateY +
                ", hashCode=" + hashCode() +
                '}';
    }
}

class ShapeFactory {
    static Map<String, Shape> shapes = new HashMap<>();

    public static Shape getShape(String type,
                                 Color fillColor,
                                 Color borderColor) {
        System.out.println("Trying to get an existing '" + type + "'");
        String key = type + "|" + fillColor.toString() + "|" + borderColor.toString();
        Shape result = shapes.get(key);
        if (result == null) {
            System.out.println("Creating a new '" + type + "'");
            switch (type) {
                case "Rectangle":
                    result = new Rectangle(fillColor, borderColor);
                    break;
                case "Circle":
                    result = new Circle(fillColor, borderColor);
                    break;
                case "Triangle":
                    result = new Triangle(fillColor, borderColor);
                    break;
                case "Line":
                    result = new Line(fillColor, borderColor);
                    break;
                default:
                    System.out.println("Shape '" + type + "' is not implemented!");
                    System.exit(1);
            }
            shapes.put(key, result);
        } else {
            System.out.println("Using an already existing '" + type + "'");
        }
        return result;
    }
}

public class Task3 {
    public static void main(String[] args) {
        ExportVisitor exportVisitor = new ExportVisitor();

        System.out.println("---------------------------");

        Rectangle rectangle1 = (Rectangle) ShapeFactory.getShape("Rectangle", new Color(11, 22, 33), new Color(33, 22, 11));
        rectangle1.setParams(1.2,2.3,3.4);
        rectangle1.draw(4.5, 5.6);
        rectangle1.export(exportVisitor);

        System.out.println("---------------------------");

        Rectangle rectangle2 = (Rectangle) ShapeFactory.getShape("Rectangle", new Color(11, 22, 33), new Color(33, 22, 11));
        rectangle2.setParams(1,2,3);
        rectangle2.draw(4, 5);
        rectangle2.export(exportVisitor);

        System.out.println("---------------------------");

        Rectangle rectangle3 = (Rectangle) ShapeFactory.getShape("Rectangle", new Color(102, 103, 104), new Color(33, 22, 11));
        rectangle3.setParams(12,23,34);
        rectangle3.draw(45, 56);
        rectangle3.export(exportVisitor);

        System.out.println("---------------------------");

        Circle circle1 = (Circle) ShapeFactory.getShape("Circle", new Color(0, 0, 255), new Color(0, 255, 0));
        circle1.setParams(0.1,0.2);
        circle1.draw(0.3, 0.4);
        circle1.export(exportVisitor);

        System.out.println("---------------------------");

        Circle circle2 = (Circle) ShapeFactory.getShape("Circle", new Color(0, 0, 255), new Color(0, 255, 0));
        circle2.setParams(318,319);
        circle2.draw(320, 321);
        circle2.export(exportVisitor);

        System.out.println("---------------------------");

        Triangle triangle = (Triangle) ShapeFactory.getShape("Triangle", new Color(0, 128, 255), new Color(255, 128, 0));
        triangle.setParams(707,717,727,737);
        triangle.draw(747, 757);
        triangle.export(exportVisitor);

        System.out.println("---------------------------");

        Line line = (Line) ShapeFactory.getShape("Line", new Color(10, 20, 30), new Color(30, 20, 10));
        line.setParams(767,777);
        line.draw(787, 797);
        line.export(exportVisitor);

        System.out.println("---------------------------");
    }
}
