package main;

public class Main {
    public static int myNumber = 10;

    public static void hookMe() {
        System.out.println("not hooked");
    }
   
    public static void doStuff() {
        while (true) {
            hookMe();
            System.out.println("My number is: " + myNumber);
            try {
                Thread.sleep(1000);
            } catch (Exception e) {
                System.out.println("bruh how???");
                break;
            }
        }
    }
    
    public static void main(String[] args) {
        System.out.println("My Java Program");
        
        if (args.length >= 1) {
            // Load library directly from Java process (good for testing with JNI_OnLoad)
            System.out.println("Library path: " + args[0]);
            System.load(args[0]);
        }

        Main.doStuff();
    }
}
