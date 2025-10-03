package exercise;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;
import java.util.List;

public class Client {
    public static void main(String[] args) throws IOException, ClassNotFoundException {
        createVehicle(new Vehicle("EIJ-8926", "Peugeot", "Passion 208", 2008, "Preto", 100000, 30000.0 ));
        createVehicle(new Vehicle("FKS-3271", "Toyota", "Corolla", 2015, "Prata", 85000, 65000.0 ));
        createVehicle(new Vehicle("GHT-9823", "Honda", "Civic", 2018, "Branco", 60000, 88000.0 ));
        createVehicle(new Vehicle("JPL-4567", "Volkswagen", "Gol", 2012, "Vermelho", 120000, 28000.0 ));
        createVehicle(new Vehicle("MNO-7134", "Chevrolet", "Onix", 2020, "Azul", 40000, 72000.0 ));
        createVehicle(new Vehicle("QRS-8421", "Ford", "Ka", 2017, "Preto", 75000, 35000.0 ));
        createVehicle(new Vehicle("TUV-1597", "Hyundai", "HB20", 2019, "Prata", 50000, 68000.0 ));
        createVehicle(new Vehicle("WXY-2649", "Fiat", "Argo", 2021, "Branco", 30000, 78000.0 ));
        createVehicle(new Vehicle("ZAB-3758", "Renault", "Sandero", 2016, "Cinza", 90000, 42000.0 ));
        createVehicle(new Vehicle("CDE-4862", "Nissan", "Kicks", 2022, "Azul", 15000, 105000.0 ));
        createVehicle(new Vehicle("FGH-5973", "Jeep", "Renegade", 2019, "Verde", 55000, 95000.0 ));
        List<Vehicle> vehicles = searchVehicle(new Vehicle( null,null,null,2019,null,null,null));
        vehicles.forEach( v -> {
            try {
                deleteVehicle(v);
            } catch (IOException e) {
                throw new RuntimeException(e);
            } catch (ClassNotFoundException e) {
                throw new RuntimeException(e);
            }
        });
        vehicles = searchVehicle(new Vehicle( null,null,null,null,null,null,null));


    }

    private static void createVehicle(Vehicle vehicle) throws IOException, ClassNotFoundException {
        Socket socket = new Socket("127.0.0.1", 54321);
        ObjectOutputStream saida = new ObjectOutputStream(socket.getOutputStream());

        saida.writeObject(VehicleOperations.CREATE);
        saida.writeObject(vehicle);
        ObjectInputStream entrada = new ObjectInputStream(socket.getInputStream());
        System.out.println(entrada.readObject());
        saida.close();

        entrada.close();
        socket.close();
    }

    private static List<Vehicle> searchVehicle(Vehicle vehicle) throws IOException, ClassNotFoundException {
        Socket socket = new Socket("127.0.0.1", 54321);
        ObjectOutputStream saida = new ObjectOutputStream(socket.getOutputStream());

        saida.writeObject(VehicleOperations.SEARCH);
        saida.writeObject(vehicle);
        ObjectInputStream entrada = new ObjectInputStream(socket.getInputStream());
        List<Vehicle> result = (List<Vehicle>) entrada.readObject();
        System.out.println(result);
        saida.close();

        entrada.close();
        socket.close();

        return result;
    }

    private static void deleteVehicle(Vehicle vehicle) throws IOException, ClassNotFoundException {
        Socket socket = new Socket("127.0.0.1", 54321);
        ObjectOutputStream saida = new ObjectOutputStream(socket.getOutputStream());

        saida.writeObject(VehicleOperations.DELETE);
        saida.writeObject(vehicle);
        ObjectInputStream entrada = new ObjectInputStream(socket.getInputStream());
        System.out.println(entrada.readObject());
        saida.close();

        entrada.close();
        socket.close();
    }
}
