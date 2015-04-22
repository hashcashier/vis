using UnityEngine;
using System.Collections;
using System.Net.Sockets;
using System.Net;
using System.Text;
using System;

public class MarkerGPSSocketClient : MonoBehaviour {

	private UdpClient clientSocket;
	private IPEndPoint serverEndPoint;
	public string serverIP;
	public int serverPort;

	void Start () {
		clientSocket = new UdpClient ();
		serverEndPoint = new IPEndPoint (IPAddress.Any, serverPort);
		clientSocket.Connect (serverIP, serverPort);
		Debug.Log ("MarkerGPS: Started.");
	}
	
	void LateUpdate () {
		if (clientSocket == null) {
			Debug.LogWarning ("MarkerGPS: No connection.");
			return;
		}

		if (clientSocket.Available == 0) {
			byte[] message = new byte[1];
			message [0] = Convert.ToByte ('U');
			clientSocket.Send (message, 1);
		}

		if (clientSocket.Available > 0) {
			while(clientSocket.Available > 0) {
				Debug.Log ("MarkerGPS: " + clientSocket.Available.ToString() + " available.");
				try {
					byte[] receivedBytes = clientSocket.Receive (ref serverEndPoint);
					string response = Encoding.ASCII.GetString (receivedBytes);
					Debug.Log ("MarkerGPS: " + response);
				} catch(SocketException se) {
					Debug.LogWarning ("MarkerGPS: Socket Error, " + se.Message);
					break;
				}
			}
		} else {
			Debug.LogWarning ("MarkerGPS: No timely response!");
		}
	}
}
