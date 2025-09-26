package exercise;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class Server {
    public static void main(String[] args) throws IOException {
        ServerSocket serverSocket = new ServerSocket(4769);

        System.out.println("A porta 4769 foi aberta!");
        System.out.println("Servidor esperando receber objeto de cliente...");

        Socket socket = serverSocket.accept();
        System.out.println("Cliente " + socket.getInetAddress().getHostAddress() + " conectado");

        ObjectInputStream in = new ObjectInputStream(socket.getInputStream());
        // Ler object

    }
}
