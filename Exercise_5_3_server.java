import java.net.*;
import java.util.*;
import java.io.*;

/*
 * Autor: Angelo Caravella 
 * Exercise 5.3
Write a Java version of exercise 3.2 (chat). 
Exploit Wireshark to capture the data exchanged between client and server.
 */
public class Exercise_5_3_server {
	public static final byte YOU = 0;
	public static final byte PEER = 1;
	public static final byte EXIT = 2;

	// costante per cedere il turno
	public static final byte PASS = '-';
	// costante per finire
	public static final byte END = '.';

	public static void main(String argv[]) {

		try {

			ServerSocket welcomeSocket = new ServerSocket(6789);
			// accetto la connessione
			Socket connectionSocket = welcomeSocket.accept();

			Scanner inFromUser = null, inFromPeer = null;
			PrintStream outToUser = null, outToPeer = null;
			try {
				inFromUser = new Scanner(System.in);
				outToUser = System.out;
				inFromPeer = new Scanner(connectionSocket.getInputStream());
				outToPeer = new PrintStream(connectionSocket.getOutputStream());
				String str = null;
				char lastChar = ' ';

				byte status = PEER; // YOU per il client PEER per il server
				while (status != EXIT) {
					switch (status) {

					case YOU:
						do {

							outToUser.print("You: ");
							str = inFromUser.nextLine();
							outToPeer.println(str);

							if (!str.equals(""))
								lastChar = str.charAt((str.length() - 1));
							if (lastChar == PASS)
								status = PEER;
							else if (lastChar == END)
								status = EXIT;

						} while (status == YOU);
						break;
					case PEER:
						do {
							outToUser.print("Peer: ");
							str = inFromPeer.nextLine();
							outToUser.println(str);

							if (!str.equals(""))
								lastChar = str.charAt((str.length() - 1));
							if (lastChar == PASS)
								status = YOU;
							else if (lastChar == END)
								status = EXIT;

						} while (status == PEER);
						break;
					}
				}
			} catch (IOException e) {
				e.printStackTrace();
			} finally {

				outToUser.println("Conversation terminated");
				if (inFromUser != null)
					inFromUser.close();
				if (outToUser != null)
					outToUser.close();
				if (inFromPeer != null)
					inFromPeer.close();
				if (outToPeer != null)
					outToPeer.close();
				if (connectionSocket != null)
					try {

						connectionSocket.close();
					} catch (IOException e) {
						System.out.println(e);
					}

			}

		} catch (Exception e) {
			System.out.println(e);
		}

	}
}
