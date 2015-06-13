using UnityEngine;
using System.Collections;

public class Oscillate : MonoBehaviour {

	private float start;
	public float range;
	public float speedFactor;
	public string axis;

	// Use this for initialization
	void Start () {
		start = 0.5f - Random.value;
	}
	
	// Update is called once per frame
	void Update () {
		float trans = range * Mathf.Sin (speedFactor * ( Mathf.PI * start + Time.time ) );
		if (axis.ToLower ().Equals ("x"))
			transform.Translate (new Vector3 (trans, 0, 0));
		else if (axis.ToLower().Equals ("y"))
			transform.Translate (new Vector3 (0, trans, 0));
		else if (axis.ToLower().Equals ("z"))
			transform.Translate (new Vector3 (0, 0, trans));
	}
}