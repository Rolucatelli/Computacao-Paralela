package exercise;

import java.io.Serializable;


public class Vehicle implements Serializable {
    private int id;
    private String placa;
    private String marca;
    private String modelo;
    private Integer ano;
    private String cor;
    private Integer quilometragem;
    private Double valor;


    public Vehicle(int id, String placa, String marca, String modelo, Integer ano, String cor, Integer quilometragem, Double valor) {
        this.id = id;
        this.placa = placa;
        this.marca = marca;
        this.modelo = modelo;
        this.ano = ano;
        this.cor = cor;
        this.quilometragem = quilometragem;
        this.valor = valor;
    }

    public Vehicle(String placa, String marca, String modelo, Integer ano, String cor, Integer quilometragem, Double valor) {
        this.id = -1;
        this.placa = placa;
        this.marca = marca;
        this.modelo = modelo;
        this.ano = ano;
        this.cor = cor;
        this.quilometragem = quilometragem;
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

    public Integer getAno() {
        return ano;
    }

    public String getCor() {
        return cor;
    }

    public Integer getQuilometragem() {
        return quilometragem;
    }

    public Double getValor() {
        return valor;
    }

    public int getId() {
        return id;
    }



    @Override
    public String toString() {
        return "Modelo: " + modelo + "\n" +
                "Marca: " + marca + "\n" +
                "Placa: " + placa + "\n" +
                "Ano: " + ano + "\n" +
                "Cor: " + cor + "\n" +
                "Quilometragem: " + quilometragem + "\n" +
                "Valor: " + valor + "\n";
    }
}
