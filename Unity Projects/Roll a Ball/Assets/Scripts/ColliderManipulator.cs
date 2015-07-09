using UnityEngine;
using System.Collections;

public class ColliderManipulator : MonoBehaviour {

	public GameObject[] watched;
	public GameObject[] affected;
	
	public Vector3 translation;
	public Vector3 rotation;

	private bool triggered;

	void OnTriggerEnter(Collider other) {
		if (triggered || !other.gameObject.activeSelf || !other.gameObject.activeInHierarchy)
			return;

		bool found = false;
		for (int i = 0; !found && i < watched.Length; i++)
			if(other.gameObject.Equals(watched[i]) || other.Equals(watched[i]))
				found = true;

		if (!found)
			return;

		for(int i = 0; i < affected.Length; i++) {
			Vector3 temp = affected[i].transform.position;
			affected[i].transform.position = new Vector3(0, 0, 0);
			affected[i].transform.eulerAngles += rotation;
			affected[i].transform.position = temp + translation;

			ProximityShower prox = affected[i].GetComponent<ProximityShower>();
			if(prox != null)
				prox.updateIP(temp + translation);
		}
		triggered = !triggered;
	}
}
