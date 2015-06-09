using UnityEngine;
using System.Collections;

public class ProximityShower : MonoBehaviour {

	// Use this for initialization
	public GameObject target;
	public Vector3 translation;
	public float lowerLimit, upperLimit;
	private Vector3 initialPosition;

	void Start () {
		initialPosition = transform.position;
	}
	
	// Update is called once per frame
	void Update () {
		Vector3 targetPos = target.transform.position;

		float dx = Mathf.Abs(targetPos.x - initialPosition.x);
		float dy = Mathf.Abs(targetPos.y - initialPosition.y);
		float dz = Mathf.Abs(targetPos.z - initialPosition.z);

		float distSqr = dx * dx + dy * dy + dz * dz;
		if (dx > upperLimit || dy > upperLimit || dz > upperLimit || distSqr > upperLimit * upperLimit) {
			transform.position = initialPosition;
		} else if (distSqr < lowerLimit * lowerLimit) {
			transform.position = initialPosition + translation;
		} else {
			transform.position = initialPosition + ( (upperLimit - Mathf.Sqrt(distSqr))/(upperLimit-lowerLimit) )*translation;
		}

		//Debug.Log (Mathf.Sqrt (distSqr));
	}
}
