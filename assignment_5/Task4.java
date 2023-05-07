// Dmitriy Okoneshnikov
// d.okoneshnikov@innopolis.university

import java.io.File;
import java.io.FileNotFoundException;
import java.util.*;

class GroceryItem {
    private String name;
    private int quantity;

    public GroceryItem(String name, int quantity) {
        this.name = name;
        this.quantity = quantity;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getQuantity() {
        return quantity;
    }

    public void setQuantity(int quantity) {
        this.quantity = quantity;
    }

    @Override
    public String toString() {
        return name + ", " + quantity;
    }
}

class ShoppingList implements Iterable<GroceryItem> {
    private final List<GroceryItem> groceryItems = new ArrayList<>();

    public void add(GroceryItem newItem) {
        for (GroceryItem groceryItem : groceryItems) {
            if (groceryItem.getName().equals(newItem.getName())) {
                groceryItem.setQuantity(groceryItem.getQuantity() + newItem.getQuantity());
                return;
            }
        }
        groceryItems.add(newItem);
    }

    public void remove(GroceryItem toDelete) {
        for (GroceryItem groceryItem : groceryItems) {
            if (groceryItem.getName().equals(toDelete.getName())) {
                if (groceryItem.getQuantity() < toDelete.getQuantity()) {
                    System.out.println("Sorry, you can buy only " + groceryItem.getQuantity() + " " + groceryItem.getName());
                    groceryItems.remove(groceryItem);
                    return;
                } else if (groceryItem.getQuantity() == toDelete.getQuantity()) {
                    groceryItems.remove(groceryItem);
                    return;
                } else {
                    groceryItem.setQuantity(groceryItem.getQuantity() - toDelete.getQuantity());
                    return;
                }
            }
        }
        System.out.println("Sorry, " + toDelete.getName() + " are not available");
    }

    @Override
    public Iterator<GroceryItem> iterator() {
        return new ShoppingListIterator();
    }

    class ShoppingListIterator implements Iterator<GroceryItem> {
        private int index = 0;

        @Override
        public boolean hasNext() {
            return index < groceryItems.size();
        }

        @Override
        public GroceryItem next() {
            if (!hasNext()) {
                throw new NoSuchElementException();
            }
            GroceryItem item = groceryItems.get(index);
            index++;
            return item;
        }
    }
}

class Customer {
    protected final String path;
    protected ShoppingList shoppingList;

    public Customer(String path) {
        this.path = path;
    }

    public void readFile() {
        shoppingList = new ShoppingList();
        try {
            File myObj = new File(path);
            Scanner reader = new Scanner(myObj);
            while (reader.hasNextLine()) {
                String data = reader.nextLine();
                String[] split = data.split(", ");
                if (split.length == 2) {
                    shoppingList.add(new GroceryItem(split[0], Integer.parseInt(split[1])));
                }
            }
            reader.close();
        } catch (FileNotFoundException e) {
            System.out.println("File " + path + " does not exist!");
            System.exit(1);
        }
    }

    @Override
    public String toString() {
        if (shoppingList == null) {
            readFile();
        }
        StringBuilder result = new StringBuilder();
        for (GroceryItem groceryItem : shoppingList) {
            result.append(groceryItem.toString()).append('\n');
        }
        return result.toString().replaceFirst("\n$", "");
    }
}

class Grocery extends Customer {
    public Grocery(String path) {
        super(path);
    }

    public void buy(Customer customer) {
        if (shoppingList == null) {
            readFile();
        }
        if (customer.shoppingList == null) {
            customer.readFile();
        }
        for (GroceryItem groceryItem : customer.shoppingList) {
            shoppingList.remove(groceryItem);
        }
    }
}

public class Task4 {
    public static void main(String[] args) {
        String inputDirectory = "Task4_input_github";

        Grocery grocery = new Grocery(inputDirectory + "/shop.txt");
        System.out.println("Grocery shop list:");
        System.out.println(grocery);
        System.out.println();

        for (int i = 1; i <= 3; i++) {
            Customer customer = new Customer(inputDirectory + "/customer_" + i + ".txt");
            System.out.println("Customer " + i + " list:");
            System.out.println(customer);
            System.out.println();
            grocery.buy(customer);
            System.out.println();
            System.out.println("Updated grocery shop list:");
            System.out.println(grocery);
            System.out.println();
        }
    }
}
