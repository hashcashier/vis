using UnityEngine;
using System.Collections;
using System.Diagnostics;

public class MarkerGPSClient : MonoBehaviour {

	public Vector3 worldFactors;
	public string positionerDIR;
	public string positionerEXE;
	private Process positioner;

	// Use this for initialization
	void Start () {
		positioner = new Process ();
		positioner.StartInfo.FileName = Application.dataPath + "/" + positionerDIR + "/" + positionerEXE;
		//positioner.StartInfo.CreateNoWindow = true;
		positioner.StartInfo.WorkingDirectory = Application.dataPath + "/" + positionerDIR;
		positioner.StartInfo.UseShellExecute = false;
		positioner.StartInfo.RedirectStandardInput = true;
		positioner.StartInfo.RedirectStandardOutput = true;

		positioner.Start ();
		positioner.StandardInput.WriteLine ("p");

	}
	
	// Update is called once per frame
	void Update () {
		if (positioner != null && positioner.StandardOutput != null) {
			string message = positioner.StandardOutput.ReadLine ();
			if(message != null)
				UnityEngine.Debug.LogWarning (message);
		}
		//transform.position
	}
}
