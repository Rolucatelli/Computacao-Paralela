package exercise;

import java.io.Serializable;

public class Vehicle implements Serializable {
    private String placa;
    private String marca;
    private String modelo;
    private int ano;
    private String cor;
    private int kilometragem;
    private double valor;


    public Vehicle(String placa, String marca, String modelo, int ano, String cor, int kilometragem, double valor) {
        this.placa = placa;
        this.marca = marca;
        this.modelo = modelo;
        this.ano = ano;
        this.cor = cor;
        this.kilometragem = kilometragem;
        this.valor = valor;
    }

    public String getPlaca() {
        return placa;
    }

    public String getMarca() {
        return marca;
    }

    public String getModelo() {
        return modelo;
    }

    public int getAno() {
        return ano;
    }

    public String getCor() {
        return cor;
    }

    public int getKilometragem() {
        return kilometragem;
    }

    public double getValor() {
        return valor;
    }

    @Override
    public String toString() {
        return "Modelo: " + modelo + "\n" +
                "Marca: " + marca + "\n" +
                "Placa: " + placa + "\n" +
                "Ano: " + ano + "\n" +
                "Cor: " + cor + "\n" +
                "Kilometragem: " + kilometragem + "\n" +
                "Valor: " + valor + "\n";
    }
}
