
/*
 * Autor: ANGELO CARAVELLA
 * 
 * Exercise 5.1
Write a client/server application in Java for the remote multiplication of two integers. Use the stream-oriented
communication model (TCP). Exploit Wireshark to capture the data exchanged between client and server.
*/
import java.io.*;
import java.net.*;
import java.util.*;

public class MulClientTcp_5_1 {
	public static void main(String argv[]) throws Exception {
		int a, b;
		long prod = 0;

		Scanner inFromUser = new Scanner(System.in);
		Socket clientSocket = new Socket("127.0.0.1", 6789);
		DataOutputStream outToServer = new DataOutputStream(clientSocket.getOutputStream());
		DataInputStream inFromServer = new DataInputStream(clientSocket.getInputStream());

		System.out.println("Insert the first integer: ");
		a = inFromUser.nextInt();
		System.out.println("Insert the second integer: ");
		b = inFromUser.nextInt();

		outToServer.writeInt(a);
		outToServer.writeInt(b);

		prod = inFromServer.readLong();
		System.out.println("product is: " + prod);
		clientSocket.close();

	}
}
