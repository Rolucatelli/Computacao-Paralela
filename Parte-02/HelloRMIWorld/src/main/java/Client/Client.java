package Client;

import Global.Hello;

import java.rmi.Naming;

public class Client {
    public static void main(String[] args) {
        try {
//            Registry registry = LocateRegistry.getRegistry(null, 1099);
//            Hello stub = (Hello) registry.lookup("Hello");
            Hello stub = (Hello) Naming.lookup("rmi://localhost/Hello");
            String response = stub.sayHello("Rodrigo");
            System.out.println("Response: " + response);
        } catch (Exception e) {
            System.err.println("Client exception: " + e.toString());
            e.printStackTrace();
        }
    }
}
