package object;

import java.io.Serializable;

public class Person implements Serializable {

    private String nome;
    private int idade;

    public Person(String nome, int idade) {
        this.nome = nome;
        this.idade = idade;
    }

    public void setNome(String nome) { this.nome = nome; }
    public void setIdade(int idade) { this.idade = idade; }
    public String getNome() { return this.nome; }
    public int getIdade() { return this.idade; }
}
