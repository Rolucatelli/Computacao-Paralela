package Server;

import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

public class Server {
    public static void main(String[] args) throws RemoteException {
        try {
            HelloImpl obj = new HelloImpl();
            Registry registry = LocateRegistry.createRegistry(1099);
//            registry.rebind("Hello", obj);
            Naming.rebind("rmi://localhost/Hello", obj);
            System.out.println("Server ready");
        } catch (Exception e) {
            System.err.println("Server exception: " + e.toString());
            e.printStackTrace();
        }
    }
}
