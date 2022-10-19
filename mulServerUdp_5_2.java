
/*

Exercise 5.2
Autor: ANGELO CARAVELLA
Write the same application of exercise 5.1 by using the datagram oriented communication model (UDP). Exploit
Wireshark to capture the data exchanged between client and server.
*/
import java.net.*;

import java.nio.ByteBuffer;

public class mulServerUdp_5_2 {
	public static void main(String args[]) throws Exception {
		DatagramSocket serverSocket = new DatagramSocket(1200);/* create socket datagram */
		byte[] receiveData = new byte[1024];
		byte[] sendData;
		while (true) {
			DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
			serverSocket.receive(receivePacket);
			ByteBuffer buf = ByteBuffer.wrap(receivePacket.getData());

			int a = buf.getInt();
			int b = buf.getInt();
			long prod = a * b;

			buf = ByteBuffer.allocate(8);
			buf.putLong(prod);
			sendData = buf.array();
			InetAddress IPAddress = receivePacket.getAddress();
			int port = receivePacket.getPort();
			DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, IPAddress, port);
			serverSocket.send(sendPacket);
		}
	}
}
