
/*
 * Autor: ANGELO CARAVELLA
 * 
 * Exercise 5.1
Write a client/server application in Java for the remote multiplication of two integers. Use the stream-oriented
communication model (TCP). Exploit Wireshark to capture the data exchanged between client and server.
*/

import java.io.*;
import java.net.*;

public class MulServerTcp_5_1 {
	public static void main(String argv[]) throws Exception {
		int a, b;
		long prod;
		String clientSentence;
		String capitalizedSentence;
		ServerSocket welcomeSocket = new ServerSocket(6789);
		while (true) {
			Socket connectionSocket = welcomeSocket.accept();
			DataOutputStream outToServer = new DataOutputStream(connectionSocket.getOutputStream());
			DataInputStream inFromServer = new DataInputStream(connectionSocket.getInputStream());

			a = inFromServer.readInt();
			b = inFromServer.readInt();

			prod = a * b;

			outToServer.writeLong(prod);
			connectionSocket.close();
		}
	}

}
