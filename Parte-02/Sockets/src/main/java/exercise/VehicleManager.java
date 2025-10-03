package exercise;

import java.util.ArrayList;
import java.util.List;

public final class VehicleManager {
    private static VehicleManager instance;
    private int lastId=0;

    private VehicleManager() {
    }

    private List<Vehicle> vehicleList = new ArrayList<>();

    public static VehicleManager getInstance() {
        if (instance == null) {
            instance = new VehicleManager();
        }
        return instance;
    }

    public Vehicle createVehicle(String placa, String marca, String modelo, Integer ano, String cor, Integer quilometragem, Double valor){
        return new Vehicle(lastId++,placa, marca, modelo, ano, cor, quilometragem, valor);
    }

    public int add(Vehicle vehicle) {
        Vehicle v = createVehicle(vehicle.getPlaca(), vehicle.getMarca(), vehicle.getModelo(), vehicle.getAno(), vehicle.getCor(), vehicle.getQuilometragem(), vehicle.getValor());
        vehicleList.add(v);
        return v.getId();
    }

    public synchronized void remove(int id) {
        vehicleList.removeIf(v -> v.getId() == id);
    }

    //String placa, String marca, String modelo, Integer ano, String cor, Integer quilometragem, Double valor
    public List<Vehicle> search(Vehicle vehicle) {
        List<Vehicle> result = new ArrayList<>();
        for (Vehicle v : vehicleList) {
            boolean match = true;

            if (vehicle.getPlaca() != null && !v.getPlaca().equalsIgnoreCase(vehicle.getPlaca())) match = false;
            if (vehicle.getMarca() != null && !v.getMarca().equalsIgnoreCase(vehicle.getMarca())) match = false;
            if (vehicle.getModelo() != null && !v.getModelo().equalsIgnoreCase(vehicle.getModelo())) match = false;
            if (vehicle.getAno() != null && !v.getAno().equals(vehicle.getAno())) match = false;
            if (vehicle.getCor() != null && !v.getCor().equalsIgnoreCase(vehicle.getCor())) match = false;
            if (vehicle.getQuilometragem() != null && !v.getQuilometragem().equals(vehicle.getQuilometragem())) match = false;
            if (vehicle.getValor() != null && !v.getValor().equals(vehicle.getValor())) match = false;

            if (match) {
                result.add(v);
            }
        }
        return result;
    }

    public void printVehicles() {
        for (Vehicle v : vehicleList) {
            System.out.println(v.toString());
        }
    }


}
