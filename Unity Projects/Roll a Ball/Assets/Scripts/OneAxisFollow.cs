using UnityEngine;
using System.Collections;

public class OneAxisFollow : MonoBehaviour {
	public GameObject followee;
	public string axis;

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void LateUpdate () {
		Vector3 tmp = transform.position;
		if (axis.ToLower().Equals ("x"))
			tmp.x = followee.transform.position.x;
		else if (axis.ToLower().Equals ("y"))
			tmp.y = followee.transform.position.y;
		else if (axis.ToLower().Equals ("z"))
			tmp.z = followee.transform.position.z;
		transform.position = tmp;
	}
}
