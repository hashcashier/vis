using UnityEngine;
using System.Collections;

public class ProximityAnimator : MonoBehaviour {
	
	public GameObject target;
	public float triggerDistance;
	public string messageIn;
	public string messageOut;
	public Vector3 transitionPosition;
	public Vector3 transitionRotation;
	public GameObject WASD;

	private bool triggered;
	private Rigidbody body;
	private Animator animator;
	private Animation anim;

	// Use this for initialization
	void Start () {
		triggered = false;
		body = WASD.GetComponent<Rigidbody> ();
		animator = GetComponent<Animator> ();
	}

	float distTo(GameObject obj) {
		Vector3 d = WASD.transform.position - obj.transform.position;
		return Mathf.Sqrt (d.x * d.x + d.y * d.y + d.z * d.z);
	}

	void displayMessage(string mesasge) {
	}
	
	// Update is called once per frame
	void Update () {
		if (target == null)
			return;

		float dist = distTo (target);

		if (dist > triggerDistance)
			return;

		displayMessage (triggered ? messageIn : messageOut);
		if (Input.GetMouseButtonDown (0)) {
			if(triggered) {
				Seating ("Stand");
			} else {
				WASD.transform.position = target.transform.position + transitionPosition;
				WASD.transform.localEulerAngles = target.transform.localEulerAngles + transitionRotation;
				Seating ("Sit");
			}
			body.constraints ^= RigidbodyConstraints.FreezePositionX | RigidbodyConstraints.FreezePositionZ;
			triggered = !triggered;
		}
	}

	void Seating(string action) {
		animator.SetTrigger (action);
	}
}
