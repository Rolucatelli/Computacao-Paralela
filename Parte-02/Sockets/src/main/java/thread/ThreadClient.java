package thread;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;

public class ThreadClient {
    public static void main(String[] args) throws IOException {

        Socket socket = new Socket("127.0.0.1", 54322);

        DataOutputStream out = new DataOutputStream(socket.getOutputStream());
        DataInputStream in = new DataInputStream(socket.getInputStream());

        out.writeUTF("rodrigo Lucatelli");

        String mensagem = in.readUTF();
        System.out.println(mensagem);

        out.close();
        in.close();
        socket.close();



    }
}
