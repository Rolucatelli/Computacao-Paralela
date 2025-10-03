package exercise;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.List;

public class Server {
    public static void main(String[] args) throws IOException, ClassNotFoundException {
        ServerSocket serverSocket = new ServerSocket(54321);

        System.out.println("A porta 54321 foi aberta!");
        System.out.println("Servidor esperando receber objeto de cliente...");
        VehicleManager vehicleManager = VehicleManager.getInstance();

        while (true) {
            Socket socket = serverSocket.accept();
            System.out.println("Cliente " + socket.getInetAddress().getHostAddress() + " conectado");

            ObjectInputStream in = new ObjectInputStream(socket.getInputStream());
            ObjectOutputStream out = new ObjectOutputStream(socket.getOutputStream());
            VehicleOperations operation = (VehicleOperations) in.readObject();
            System.out.println(operation);
            Vehicle vehicle = (Vehicle) in.readObject();


            switch (operation) {
                case CREATE:
                    int id = vehicleManager.add(vehicle);

                    out.writeObject("Veiculo " + id + " adicionado com sucesso!");
                    break;
                case SEARCH:
                    List<Vehicle> list = vehicleManager.search(vehicle);
                    list.forEach(System.out::println);
                    out.writeObject(list);
                    break;
                case DELETE:
                    vehicleManager.remove(vehicle.getId());
                    out.writeObject("Veiculo removido com sucesso!");
                    break;
                case UPDATE:
                    vehicleManager.printVehicles();
                    break;
            }

            in.close();
            out.close();
            socket.close();

        }
    }


}
