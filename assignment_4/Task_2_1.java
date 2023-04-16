// Dmitriy Okoneshnikov
// d.okoneshnikov@innopolis.university

/*
* Single-responsibility principle:
        This principle is violated in Employee class.
        There is no need to know the hourly rate for salaried employee and vice versa.
        Let us move the hourlyRate and hoursWorked from Employee to HourlyEmployee and
        move the salary from Employee to SalariedEmployee.
        So the base class stores only the name of the employee.
* Open–closed principle:
        This principle is violated in Payroll class.
        There is no need in two methods that work with HourlyEmployee and SalariedEmployee separately.
        This will restrict us from adding other types of payments.
        These methods call the same calculatePay, so it would be logical two replace them
        with one method that accepts the base class Employee.
* Liskov substitution principle:
        This principle is not broken in any of the classes.
* Interface segregation principle:
        This principle is not broken in any of the classes.
* Dependency inversion principle:
        This principle is not broken in any of the classes.
 */

abstract class Employee {
    private String name;

    public Employee(String name) {
        this.name = name;
    }

    public String getName() {
        return name;
    }

    abstract public double calculatePay();
}

class HourlyEmployee extends Employee {
    private double hourlyRate;
    private double hoursWorked;

    public HourlyEmployee(String name, double hourlyRate, double hoursWorked) {
        super(name);
        this.hourlyRate = hourlyRate;
        this.hoursWorked = hoursWorked;
    }

    public double calculatePay() {
        return hourlyRate * hoursWorked;
    }
}

class SalariedEmployee extends Employee {
    private double salary;

    public SalariedEmployee(String name, double salary) {
        super(name);
        this.salary = salary;
    }

    public double calculatePay() {
        return salary;
    }
}

class Payroll {
    public double calculatePay(Employee employee) {
        return employee.calculatePay();
    }
}

public class Task_2_1 {
    public static void main(String[] args) {
        SalariedEmployee salariedEmployee = new SalariedEmployee("Alice", 1234.56);
        HourlyEmployee hourlyEmployee = new HourlyEmployee("Bob", 12.3456, 8);

        Payroll payroll = new Payroll();
        System.out.println(payroll.calculatePay(salariedEmployee));
        System.out.println(payroll.calculatePay(hourlyEmployee));
    }
}
