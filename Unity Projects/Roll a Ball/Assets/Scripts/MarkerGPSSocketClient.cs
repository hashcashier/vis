using UnityEngine;
using System.Collections;
using System.Net.Sockets;
using System.Net;
using System.Text;
using System;

public class MarkerGPSSocketClient : MonoBehaviour {

	private UdpClient clientSocket;
	private IPEndPoint serverEndPoint;
	public char[] mapping;
	public Vector3 worldFactors, worldOffset;
	public string serverIP;
	public int serverPort;

	void Start () {
		clientSocket = new UdpClient ();
		serverEndPoint = new IPEndPoint (IPAddress.Any, serverPort);
		clientSocket.Connect (serverIP, serverPort);
		Debug.Log ("MarkerGPS: Started.");
	}

	float getVal(Vector3 vec, char i) {
		if (i == 'x')
			return vec.x;
		else if (i == 'y')
			return vec.y;
		return vec.z;
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
			bool positionUpdate = false;
			Vector3 neuPos = new Vector3();
			while(clientSocket.Available > 0) {
				Debug.Log ("MarkerGPS: " + clientSocket.Available.ToString() + " available.");
				try {
					byte[] receivedBytes = clientSocket.Receive (ref serverEndPoint);
					string response = Encoding.ASCII.GetString (receivedBytes);
					Debug.Log ("MarkerGPS: " + response);
					string trimmedResponse = "";
					for(int i = 0, j = 0; i < response.Length; i++) {
						if(response[i] == ' ' && j == 1) {
							continue;
						}
						j = response[i] == ' ' ? 1 : 0;
						trimmedResponse += response[i];
					}
					trimmedResponse = trimmedResponse.Trim();
					string[] tokens = trimmedResponse.Split (' ');
					if(tokens.Length == 3) {
						neuPos.x = float.Parse(tokens[0]);
						neuPos.y = float.Parse(tokens[1]);
						neuPos.z = float.Parse(tokens[2]);
						positionUpdate = true;
					} else {
						Debug.LogWarning ("MarkerGPS: Malformed Token Array of " + tokens.Length.ToString());
					}

				} catch(SocketException se) {
					Debug.LogWarning ("MarkerGPS: Socket Error, " + se.Message);
					break;
				}
			}

			if(positionUpdate) {
				Vector3 neuTransform = transform.position;
				if(worldFactors.x != 0)
					neuTransform.x = getVal(neuPos, mapping[0]) * worldFactors.x + worldOffset.x;
				if(worldFactors.y != 0)
					neuTransform.y = getVal(neuPos, mapping[1]) * worldFactors.y + worldOffset.y;
				if(worldFactors.z != 0)
					neuTransform.z = getVal(neuPos, mapping[2]) * worldFactors.z + worldOffset.z;
				Debug.Log ("Updating Position To: " + neuTransform.ToString());
				transform.position = neuTransform;
			}
		} else {
			Debug.LogWarning ("MarkerGPS: No timely response!");
		}
	}
}
