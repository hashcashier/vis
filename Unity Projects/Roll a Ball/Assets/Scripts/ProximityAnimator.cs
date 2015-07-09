using UnityEngine;
using System.Collections;

public class ProximityAnimator : MonoBehaviour {
	
	public GameObject target;
	public float triggerDistance;
	public string messageIn;
	public string messageOut;
	public Vector3 transitionPosition;
	public Vector3 transitionRotation;
	public GameObject[] WASD;

	private bool triggered;
	private Rigidbody[] body;
	private Animator animator;
	private Animation anim;

	// Use this for initialization
	void Start () {
		triggered = false;
		body = new Rigidbody[WASD.Length];
		for(int i = 0; i < WASD.Length; i++)
			body[i] = WASD[i].GetComponent<Rigidbody> ();
		animator = GetComponent<Animator> ();
	}

	float distTo(GameObject A, GameObject B) {
		Vector3 d = A.transform.position - B.transform.position;
		return Mathf.Sqrt (d.x * d.x + d.y * d.y + d.z * d.z);
	}

	void displayMessage(string mesasge) {
	}
	
	// Update is called once per frame
	void Update () {
		if (target == null)
			return;

		for (int i = 0; i < WASD.Length; i++) {
			if(!WASD[i].activeSelf || !WASD[i].activeInHierarchy)
				continue;

			float dist = distTo (WASD[i], target);

			if (dist > triggerDistance)
				continue;

			displayMessage (triggered ? messageIn : messageOut);
			if (Input.GetMouseButtonDown (0)) {
				if (triggered) {
					Seating ("Stand");
				} else {
					WASD[i].transform.position = target.transform.position + transitionPosition;
					WASD[i].transform.localEulerAngles = target.transform.localEulerAngles + transitionRotation;
					Seating ("Sit");
				}
				body[i].constraints ^= RigidbodyConstraints.FreezePositionX | RigidbodyConstraints.FreezePositionZ;
				triggered = !triggered;
			}
		}
	}

	void Seating(string action) {
		animator.SetTrigger (action);
	}
}
