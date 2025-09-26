package exercise;

import java.util.ArrayList;
import java.util.List;

public final class VehicleManager {
    private static VehicleManager instance;

    private VehicleManager() {
    }

    private List<Vehicle> vehicleList = new ArrayList<>();

    public static VehicleManager getInstance() {
        if (instance == null) {
            instance = new VehicleManager();
        }
        return instance;
    }

    public void add(Vehicle vehicle) {
        vehicleList.add(vehicle);
    }

    public void remove(Vehicle vehicle) {
        vehicleList.remove(vehicle);
    }

    public List<Vehicle> search(String placa, String marca, String modelo, Integer ano, String cor, Integer kilometragem, Double valor) {
        List<Vehicle> result = new ArrayList<>();
        for (Vehicle v : vehicleList) {
            boolean match = true;

            if (placa != null && !v.getPlaca().equalsIgnoreCase(placa)) match = false;
            if (marca != null && !v.getMarca().equalsIgnoreCase(marca)) match = false;
            if (modelo != null && !v.getModelo().equalsIgnoreCase(modelo)) match = false;
            if (ano != null && v.getAno() != ano) match = false;
            if (cor != null && !v.getCor().equalsIgnoreCase(cor)) match = false;
            if (kilometragem != null && v.getKilometragem() != kilometragem) match = false;
            if (valor != null && v.getValor() != valor) match = false;

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
