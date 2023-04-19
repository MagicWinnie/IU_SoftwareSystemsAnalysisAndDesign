// Dmitriy Okoneshnikov
// d.okoneshnikov@innopolis.university

// Import libraries
import java.io.*;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

// Class for process data
class ProcessData {
    private final long PID;
    private double SHR;
    private double VIRT;
    private double CPU;
    private final String OwnerName;
    private final long NI;
    private final String permissions;

    // Set parameters from builder
    public ProcessData(ProcessDataBuilder builder) {
        this.PID = builder.getPID();
        this.SHR = builder.getSHR();
        this.VIRT = builder.getVIRT();
        this.CPU = builder.getCPU();
        this.OwnerName = builder.getOwnerName();
        this.NI = builder.getNI();
        this.permissions = builder.getPermissions();
    }

    public void setSHR(double SHR) {
        this.SHR = SHR;
    }

    public void setVIRT(double VIRT) {
        this.VIRT = VIRT;
    }

    public void setCPU(double CPU) {
        this.CPU = CPU;
    }

    public long getPID() {
        return PID;
    }

    public double getSHR() {
        return SHR;
    }

    public double getVIRT() {
        return VIRT;
    }

    public double getCPU() {
        return CPU;
    }

    public String getOwnerName() {
        return OwnerName;
    }

    public long getNI() {
        return NI;
    }

    public String getPermissions() {
        return permissions;
    }

    @Override
    public String toString() {
        return "ProcessData{" +
                "PID=" + PID +
                ", SHR=" + SHR +
                ", VIRT=" + VIRT +
                ", CPU=" + CPU +
                ", OwnerName='" + OwnerName + '\'' +
                ", NI=" + NI +
                ", permissions='" + permissions + '\'' +
                '}';
    }
}

// Class for builder pattern
class ProcessDataBuilder {
    private long PID;
    private double SHR;
    private double VIRT;
    private double CPU;
    private String OwnerName;
    private long NI;
    private String permissions;

    public ProcessDataBuilder setPID(long PID) {
        this.PID = PID;
        return this;
    }

    public ProcessDataBuilder setSHR(double SHR) {
        this.SHR = SHR;
        return this;
    }

    public ProcessDataBuilder setVIRT(double VIRT) {
        this.VIRT = VIRT;
        return this;
    }

    public ProcessDataBuilder setCPU(double CPU) {
        this.CPU = CPU;
        return this;
    }

    public ProcessDataBuilder setOwnerName(String OwnerName) {
        this.OwnerName = OwnerName;
        return this;
    }

    public ProcessDataBuilder setNI(long NI) {
        this.NI = NI;
        return this;
    }

    public ProcessDataBuilder setPermissions(String permissions) {
        this.permissions = permissions;
        return this;
    }

    public long getPID() {
        return PID;
    }

    public double getSHR() {
        return SHR;
    }

    public double getVIRT() {
        return VIRT;
    }

    public double getCPU() {
        return CPU;
    }

    public String getOwnerName() {
        return OwnerName;
    }

    public long getNI() {
        return NI;
    }

    public String getPermissions() {
        return permissions;
    }

    // Create ProcessData instance from builder
    public ProcessData build() {
        return new ProcessData(this);
    }
}

// Logger class
class Logging {
    // Instance object for singleton
    private static Logging instance;
    private static File file;
    // Date format for logging
    private static DateFormat dateFormat = new SimpleDateFormat("dd/MM/yyyy HH:mm:ss.SSS");

    private Logging() {}

    public static Logging getInstance() {
        // If the static instance does not exist
        // Then create new instance
        if (instance == null) {
            instance = new Logging();
            file = new File("Task_3.log");
        }
        return instance;
    }

    // Write one process to file, erasing previous data
    public void writeToFile(String prefix, ProcessData data) throws IOException {
        // If file does not exist then create a new one
        if (!file.exists()) {
            file.createNewFile();
        }

        // Create writers
        FileWriter fileWriter = new FileWriter(file);
        BufferedWriter bufferedWriter = new BufferedWriter(fileWriter);

        // Write with date
        Date date = new Date();
        bufferedWriter.write("[" + dateFormat.format(date) + "]: " + prefix + " " + data.toString() + "\n");

        // Close writers
        bufferedWriter.close();
        fileWriter.close();
    }

    // Print all processes from log file
    void readFromFile() throws IOException {
        // Create readers
        FileReader fileReader = new FileReader(file);
        BufferedReader bufferedReader = new BufferedReader(fileReader);

        // Print all lines
        while (true) {
            String line = bufferedReader.readLine();
            if (line == null) {
                break;
            }
            System.out.println(line);
        }

        // Close readers
        bufferedReader.close();
        fileReader.close();
    }

    // Append one process to file
    void appendToFile(String prefix, ProcessData data) throws IOException {
        // If file does not exist then create a new one
        if (!file.exists()) {
            file.createNewFile();
        }

        // Create writers
        FileWriter fileWriter = new FileWriter(file, true);
        BufferedWriter bufferedWriter = new BufferedWriter(fileWriter);

        // Append with date
        Date date = new Date();
        bufferedWriter.write("[" + dateFormat.format(date) + "]: " + prefix + " " + data.toString() + "\n");

        // Close writers
        bufferedWriter.close();
        fileWriter.close();
    }

    public void deleteFile() {
        // If file exists then delete it
        if (file.exists()) {
            file.delete();
        }
    }
}

// Base abstract clas for process monitor
abstract class Monitoring {
    public Logging logging;
    public Monitoring() {
        // Create logging object
        logging = Logging.getInstance();
    }
    public void logData(String prefix, ProcessData data) {
        try {
            logging.appendToFile(prefix, data);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
    public void visualizeData() {
        try {
            logging.readFromFile();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
}

// System process monitor
class SystemProcessesMonitoring extends Monitoring {
    public SystemProcessesMonitoring() {
        super();
    }
    public void logData(ProcessData data) {
        // If the process has necessary permissions
        if (!data.getPermissions().equals("All Permissions")) {
            return;
        }
        // Update values
        data.setCPU(Math.random() * 100.0D);
        data.setSHR(Math.random() * 100.0D);
        data.setVIRT(Math.random() * 100.0D);
        // Run base class method
        super.logData("System", data);
    }
    public void visualizeData() {
        System.out.println("Visualize data in system:");
        super.visualizeData();
    }
}

// User process monitor
class UserProcessesMonitoring extends Monitoring {
    public UserProcessesMonitoring() {
        super();
    }
    public void logData(ProcessData data) {
        // If the process has necessary permissions
        if (!data.getPermissions().equals("User Permissions")) {
            return;
        }
        // Update values
        data.setCPU(Math.random() * 100.0D);
        data.setSHR(Math.random() * 100.0D);
        data.setVIRT(Math.random() * 100.0D);
        // Run base class method
        super.logData("User", data);
    }
    public void visualizeData() {
        System.out.println("Visualize data in user:");
        // Run base class method
        super.visualizeData();
    }
}

// Factory for monitor classes
class MonitoringFactory {
    public static Monitoring getMonitoring(String monitoringType){
        switch (monitoringType) {
            case "SYSTEM":
                return new SystemProcessesMonitoring();
            case "USER":
                return new UserProcessesMonitoring();
            default:
                return null;
        }
    }
}

public class Task_3 {
    public static void main(String[] args) {
        // Create user process
        ProcessData processDataUser = new ProcessDataBuilder()
                .setPID(666)
                .setSHR(3.14)
                .setVIRT(2.71)
                .setCPU(9.81)
                .setOwnerName("Dmitriy Okoneshnikov")
                .setNI(8)
                .setPermissions("User Permissions")
                .build();
        // Create system process
        ProcessData processDataSystem = new ProcessDataBuilder()
                .setPID(123)
                .setSHR(1.1)
                .setVIRT(2.2)
                .setCPU(3.3)
                .setOwnerName("System")
                .setNI(4)
                .setPermissions("All Permissions")
                .build();

        // Create user process monitor using factory
        UserProcessesMonitoring userProcessesMonitoring = (UserProcessesMonitoring)MonitoringFactory.getMonitoring("USER");
        // Log 5 times
        for (int i = 0; i < 5; i++) {
            userProcessesMonitoring.logData(processDataUser);
        }
        // Print log file
        userProcessesMonitoring.visualizeData();

        // Create system process monitor using factory
        SystemProcessesMonitoring systemProcessesMonitoring = (SystemProcessesMonitoring)MonitoringFactory.getMonitoring("SYSTEM");
        // Log 5 times
        for (int i = 0; i < 5; i++) {
            systemProcessesMonitoring.logData(processDataSystem);
        }
        // Print log file
        systemProcessesMonitoring.visualizeData();

        // Delete log file
        systemProcessesMonitoring.logging.deleteFile();
    }
}
