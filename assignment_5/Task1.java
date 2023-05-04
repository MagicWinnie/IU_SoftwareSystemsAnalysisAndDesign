// Dmitriy Okoneshnikov
// d.okoneshnikov@innopolis.university

import java.util.ArrayList;
import java.util.List;

interface IMenu {
    void print();

    String getName();

    double getPrice();
}

abstract class MenuItemDecorator implements IMenu {
    protected IMenu menuItem;

    public MenuItemDecorator(IMenu menuItem) {
        this.menuItem = menuItem;
    }

    @Override
    public void print() {
        menuItem.print();
    }

    @Override
    public String getName() {
        return menuItem.getName();
    }

    @Override
    public double getPrice() {
        return menuItem.getPrice();
    }
}

class SpicyDecorator extends MenuItemDecorator {

    public SpicyDecorator(IMenu menuItem) {
        super(menuItem);
    }

    @Override
    public void print() {
        super.print();
        System.out.println("\t\t-- This item is spicy (+ $2)");
    }

    @Override
    public String getName() {
        return super.getName();
    }

    @Override
    public double getPrice() {
        return super.getPrice() + 2;
    }
}

class VegetarianDecorator extends MenuItemDecorator {

    public VegetarianDecorator(IMenu menuItem) {
        super(menuItem);
    }

    @Override
    public void print() {
        super.print();
        System.out.println("\t\t-- This item is vegetarian (+ $4)");
    }

    @Override
    public String getName() {
        return super.getName();
    }

    @Override
    public double getPrice() {
        return super.getPrice() + 4;
    }
}

class CompositeMenu implements IMenu {
    private final String name;
    private final List<IMenu> menuItems = new ArrayList<>();

    public CompositeMenu(String name) {
        this.name = name;
    }

    public void addMenuItem(IMenu menuItem) {
        menuItems.add(menuItem);
    }

    @Override
    public void print() {
        System.out.println(getName() + " [ $" + getPrice() + " ]");
        System.out.println("-------------------------");
        for (IMenu menuItem : menuItems) {
            if (menuItem instanceof CompositeMenu) {
                System.out.println();
            }
            menuItem.print();
        }
    }

    @Override
    public String getName() {
        return name;
    }

    @Override
    public double getPrice() {
        double price = 0;
        for (IMenu menuItem : menuItems) {
            price += menuItem.getPrice();
        }
        return price;
    }
}

class MenuItem implements IMenu {
    private final String name;
    private final double price;

    public MenuItem(String name, double price) {
        this.name = name;
        this.price = price;
    }

    @Override
    public void print() {
        System.out.println('\t' + name + ", $" + price);
    }

    @Override
    public String getName() {
        return name;
    }

    @Override
    public double getPrice() {
        return price;
    }
}

public class Task1 {
    public static void main(String[] args) {
        CompositeMenu appetizerMenu = new CompositeMenu("Appetizer Menu");
        appetizerMenu.addMenuItem(new VegetarianDecorator(new MenuItem("Garlic bread", 5.5)));
        appetizerMenu.addMenuItem(new SpicyDecorator(new MenuItem("Chicken wings", 12.5)));
        appetizerMenu.addMenuItem(new VegetarianDecorator(new SpicyDecorator(new MenuItem("Tomato soup", 10.5))));

        CompositeMenu dessertMenu = new CompositeMenu("Dessert Menu");
        dessertMenu.addMenuItem(new MenuItem("Pie", 4.5));
        dessertMenu.addMenuItem(new SpicyDecorator(new MenuItem("Pie", 4.5)));
        dessertMenu.addMenuItem(new MenuItem("Ice cream", 3.0));

        CompositeMenu mainMenu = new CompositeMenu("Main Menu");
        mainMenu.addMenuItem(appetizerMenu);
        mainMenu.addMenuItem(dessertMenu);
        mainMenu.print();
    }
}
