
/*

Exercise 5.2
Autor: ANGELO CARAVELLA
Write the same application of exercise 5.1 by using the datagram oriented communication model (UDP). Exploit
Wireshark to capture the data exchanged between client and server.
*/

import java.io.*;
import java.net.*;


public class MulServerDgram_5_2_versione2 {
	public static void main(String args[]) throws Exception {
		DatagramSocket serverSocket = new DatagramSocket(1200);/* create socket datagram */
		byte[] receiveData = new byte[1024];
		byte[] sendData;
		while (true) {
			DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
			serverSocket.receive(receivePacket);

			ByteArrayInputStream bufFromClient = new ByteArrayInputStream(receivePacket.getData());
			DataInputStream shortStreamFromClient = new DataInputStream(bufFromClient);

			int a = shortStreamFromClient.readInt();
			int b = shortStreamFromClient.readInt();
			long prod = a * b;

			ByteArrayOutputStream bufToClient = new ByteArrayOutputStream();
			DataOutputStream shortStreamToClient = new DataOutputStream(bufToClient);

			shortStreamToClient.writeLong(prod);
			sendData = bufToClient.toByteArray();

			InetAddress IPAddress = receivePacket.getAddress();
			int port = receivePacket.getPort();
			DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, IPAddress, port);
			serverSocket.send(sendPacket);
		}
	}
}
