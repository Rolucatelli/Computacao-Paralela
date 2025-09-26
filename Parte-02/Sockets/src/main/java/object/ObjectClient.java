package object;

import java.io.IOException;
import java.io.ObjectOutputStream;
import java.net.Socket;

public class ObjectClient {
    public static void main(String[] args) throws IOException {
        //1 - Abrir conexão
        Socket socket = new Socket("127.0.0.1", 54323);

        //2 - Definir stream de saída de dados do cliente
        ObjectOutputStream saida = new ObjectOutputStream(socket.getOutputStream());
        Person p = new Person("Rodrigo Lucatelli", 20);
        saida.writeObject(p);

        //4 - Fechar streams de saída de dados
        saida.close();

        //5 - Fechar o socket
        socket.close();
    }
}
