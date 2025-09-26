package text;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class TextServer {
    public static void main(String[] args) throws IOException {
        ServerSocket serverSocket = new ServerSocket(54321);
        System.out.println("Servidor iniciado na porta 54321");
        System.out.println("Servidor esperando receber mensagem de cliente...");

        Socket socket = serverSocket.accept();
        System.out.println("Cliente " + socket.getInetAddress().getHostAddress() + " conectado");

        DataInputStream in = new DataInputStream(socket.getInputStream());
        DataOutputStream out = new DataOutputStream(socket.getOutputStream());

        out.writeUTF(processMessage(in.readUTF()));

        out.close();
        in.close();
        socket.close();
        serverSocket.close();
    }

    private static String processMessage(String mensagem) {
        return mensagem.toUpperCase();
    }
}
