// Dmitriy Okoneshnikov
// d.okoneshnikov@innopolis.university

import java.util.ArrayList;
import java.util.List;

class Channel {
    private final String name;
    private final List<User> subscribers = new ArrayList<>();

    public Channel(String name) {
        this.name = name;
    }

    void subscribe(User user) {
        this.subscribers.add(user);
    }

    void unsubscribe(User user) {
        this.subscribers.remove(user);
    }

    void publish(Content content) {
        for (User user : subscribers) {
            String notification = "Channel " + name + " " + content.getNotification();
            user.update(notification);
        }
    }

    public String getName() {
        return name;
    }
}

class User {
    private final String name;

    public User(String name) {
        this.name = name;
    }

    public void update(String notification) {
        System.out.println(name + ": " + notification);
    }

    public String getName() {
        return name;
    }
}

interface Content {
    String getNotification();
}

class Video implements Content {
    private final String name;

    public Video(String name) {
        this.name = name;
    }

    @Override
    public String getNotification() {
        return "published new Video: " + name;
    }

    public String getName() {
        return name;
    }
}

class Shorts implements Content {
    private final String name;

    public Shorts(String name) {
        this.name = name;
    }

    @Override
    public String getNotification() {
        return "published new Shorts: " + name;
    }

    public String getName() {
        return name;
    }
}

class LiveStream implements Content {
    private final String name;

    public LiveStream(String name) {
        this.name = name;
    }

    @Override
    public String getNotification() {
        return "is going live: " + name;
    }

    public String getName() {
        return name;
    }
}

public class Task2 {
    public static void main(String[] args) {
        // Create some users
        User user1 = new User("Alice");
        User user2 = new User("Bob");
        // Create some channels
        Channel channel1 = new Channel("Discovery");
        Channel channel2 = new Channel("Pewdiepie");
        Channel channel3 = new Channel("Innopolis");
        // Subscribe users to channels
        channel1.subscribe(user1);
        channel2.subscribe(user2);
        channel3.subscribe(user1);
        channel3.subscribe(user2);
        // Publish some content
        channel1.publish(new Video("Animals in Australia"));
        channel2.publish(new Shorts("Minicraft"));
        channel3.publish(new LiveStream("Report from rainforest"));
        // Unsubscribe a user from a channel
        channel3.unsubscribe(user1);
        // Publish more content
        channel1.publish(new Video("Scuba diving in Great Barrier Reef"));
        channel2.publish(new Shorts("Fortnite montage"));
        channel3.publish(new LiveStream("Jungle survival tips"));
    }
}
