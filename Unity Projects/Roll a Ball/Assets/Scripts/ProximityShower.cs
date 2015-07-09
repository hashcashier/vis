using UnityEngine;
using System.Collections;

public class ProximityShower : MonoBehaviour {

	// Use this for initialization
	public GameObject[] target;
	public Vector3 translation;
	public float lowerLimit, upperLimit;
	private Vector3 initialPosition;

	void Start () {
		initialPosition = transform.position;
	}

	public void updateIP(Vector3 neu) {
		initialPosition = neu;
	}
	
	// Update is called once per frame
	void Update () {
		float mdist = -1, mdx = -1, mdy = -1, mdz = -1;
		for(int i = 0; i < target.Length; i++) {
			if(target[i] == null || !target[i].activeSelf || !target[i].activeInHierarchy) continue;
			Vector3 targetPos = target[i].transform.position;
			
			float dx = Mathf.Abs(targetPos.x - initialPosition.x);
			float dy = Mathf.Abs(targetPos.y - initialPosition.y);
			float dz = Mathf.Abs(targetPos.z - initialPosition.z);
			
			float distSqr = dx * dx + dy * dy + dz * dz;

			mdist = distSqr < mdist || mdist < 0 ? distSqr : mdist;
			mdx = dx < mdx || mdx < 0 ? dx : mdx;
			mdy = dy < mdy || mdy < 0 ? dy : mdy;
			mdz = dz < mdz || mdz < 0 ? dz : mdz;
		}
		if (mdist == -1)
			return;
		if (mdx > upperLimit || mdy > upperLimit || mdz > upperLimit || mdist > upperLimit * upperLimit) {
			transform.position = initialPosition;
		} else if (mdist < lowerLimit * lowerLimit) {
			transform.position = initialPosition + translation;
		} else {
			transform.position = initialPosition + ( (upperLimit - Mathf.Sqrt(mdist))/(upperLimit-lowerLimit) )*translation;
		}

		//Debug.Log (Mathf.Sqrt (distSqr));
	}
}
