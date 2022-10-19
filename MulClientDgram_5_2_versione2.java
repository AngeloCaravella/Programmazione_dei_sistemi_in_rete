
/*

Exercise 5.2
Autor: ANGELO CARAVELLA
Write the same application of exercise 5.1 by using the datagram oriented communication model (UDP). Exploit
Wireshark to capture the data exchanged between client and server.
*/

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.nio.ByteBuffer;
import java.util.Scanner;

public class MulClientDgram_5_2_versione2 {
	public static void main(String args[]) throws Exception {
		int a, b;
		long prod;
		/* create object type scanner */
		Scanner inFromUser = new Scanner(System.in);
		/* create socket connection */
		DatagramSocket clientSocket = new DatagramSocket();
		InetAddress IPAddress = InetAddress.getByName("127.0.0.1");
		byte[] sendData;/* byte data */
		byte[] receiveData = new byte[1024];

		ByteArrayOutputStream bufToClient = new ByteArrayOutputStream();
		DataOutputStream shortStreamToClient = new DataOutputStream(bufToClient);

		System.out.println("Insert the first integer: \n");
		a = inFromUser.nextInt();

		System.out.println("Insert the second integer: \n");
		b = inFromUser.nextInt();

		shortStreamToClient.writeInt(a);
		shortStreamToClient.writeInt(b);

		sendData = bufToClient.toByteArray();

		DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, IPAddress, 1200);
		clientSocket.send(sendPacket);
		DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
		clientSocket.receive(receivePacket);
		/* receive packet */
		ByteArrayInputStream bufFromClient = new ByteArrayInputStream(receivePacket.getData());
		DataInputStream shortStreamFromClient = new DataInputStream(bufFromClient);

		prod = shortStreamFromClient.readLong();

		System.out.println("FROM SERVER :" + prod);
		clientSocket.close();
	}
}
