package exercise;

import java.io.IOException;
import java.io.ObjectOutputStream;
import java.net.Socket;

public class Client {
    public static void main(String[] args) throws IOException {
        Socket socket = new Socket("127.0.0.1", 4769);

        ObjectOutputStream saida = new ObjectOutputStream(socket.getOutputStream());
        // Enviar object
        Vehicle v = new Vehicle("EIJ-8926", "Peugeout", "Passion 208", 2012, "Preto", 100000, 30000);
        saida.writeObject(v);

        saida.close();
        socket.close();
    }
}
