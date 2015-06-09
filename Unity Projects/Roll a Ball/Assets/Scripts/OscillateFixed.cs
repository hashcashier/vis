using UnityEngine;
using System.Collections;

public class OscillateFixed : MonoBehaviour {

	public float start;
	public float amplitude;
	public float period;
	public string axis;
	private Vector3 initial;

	// Use this for initialization
	void Start () {
		initial = transform.position;
	}
	
	// Update is called once per frame
	void Update () {
		float trans = amplitude * Mathf.Sin (period * ( Mathf.PI * start + Time.time ) );
		Vector3 curr = transform.position;
		if (axis.ToLower ().Equals ("x"))
			transform.position = (new Vector3 (initial.x + trans, curr.y, curr.z));
		else if (axis.ToLower().Equals ("y"))
			transform.position = (new Vector3 (curr.x, initial.y + trans, curr.z));
		else if (axis.ToLower().Equals ("z"))
			transform.position = (new Vector3 (curr.x, curr.y, initial.z + trans));
	}
}