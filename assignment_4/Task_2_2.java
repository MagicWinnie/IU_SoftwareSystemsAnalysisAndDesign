// Dmitriy Okoneshnikov
// d.okoneshnikov@innopolis.university

/*
* Single-responsibility principle:
        This principle is not broken in any of the classes.
* Open–closed principle:
        This principle is not broken in any of the classes.
* Liskov substitution principle:
        This principle is violated in Rectangle/Shape classes.
        We should make Rectangle and Square implement some general interface.
        Because a square does not really inherit the properties of a rectangle.
        It does not make sense to set a height or a width for a square.
        We should rather set its side.
* Interface segregation principle:
        This principle is not broken in any of the classes.
* Dependency inversion principle:
        This principle is not broken in any of the classes.
 */

interface Quadrilateral {
    double getArea();
    double getPerimeter();
}

class Rectangle implements Quadrilateral {
    private double length;
    private double width;

    public Rectangle(double length, double width) {
        this.length = length;
        this.width = width;
    }
    public void setLength(double length) {
        this.length = length;
    }
    public void setWidth(double width) {
        this.width = width;
    }
    public double getArea() {
        return length * width;
    }
    public double getPerimeter() {
        return 2 * (length + width);
    }
}

class Square implements Quadrilateral {
    private double side;
    public Square(double side) {
        this.side = side;
    }
    public void setSide(double side) {
        this.side = side;
    }
    public double getArea() {
        return side * side;
    }
    public double getPerimeter() {
        return 4 * side;
    }
}

class Calculator {
    public static double doModificationsAndComputeArea(Rectangle rectangle) {
        // Modify rectangles in some way using setters
        rectangle.setLength(5);
        rectangle.setWidth(3);

        // Returns the area of the modified rectangle
        return rectangle.getArea();
    }
    public static double doModificationsAndComputeArea(Square square) {
        // Modify squares in some way using setters
        square.setSide(3);

        // Returns the area of the modified square
        return square.getArea();
    }
}

public class Task_2_2 {
    public static void main(String[] args) {
        Rectangle rectangle = new Rectangle(3, 7);
        Square square = new Square(5);

        System.out.println("Area of original rectangle: " + rectangle.getArea());
        double areaRectangleAfter = Calculator.doModificationsAndComputeArea(rectangle);
        System.out.println("Area of rectangle after modifications: " + areaRectangleAfter);

        System.out.println("\nArea of original square: " + square.getArea());
        double areaSquareAfter = Calculator.doModificationsAndComputeArea(square);
        System.out.println("Area of square after modifications: " + areaSquareAfter);
    }
}
