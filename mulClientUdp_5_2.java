
/*

Exercise 5.2
Autor: ANGELO CARAVELLA
Write the same application of exercise 5.1 by using the datagram oriented communication model (UDP). Exploit
Wireshark to capture the data exchanged between client and server.
*/
import java.net.*;
import java.nio.ByteBuffer;
import java.util.Scanner;

public class mulClientUdp_5_2 {
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

		System.out.println("Insert the first integer: \n");
		a = inFromUser.nextInt();

		System.out.println("Insert the second integer: \n");
		b = inFromUser.nextInt();
		ByteBuffer buf = ByteBuffer.allocate(8);
		buf.putInt(a);
		buf.putInt(b);

		sendData = buf.array();
		DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, IPAddress, 1200);
		clientSocket.send(sendPacket);
		DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
		clientSocket.receive(receivePacket);
		ByteBuffer buf1 = ByteBuffer.wrap(receivePacket.getData());
		prod = buf1.getLong();
		System.out.println("FROM SERVER :" + prod);
		clientSocket.close();
	}
}
