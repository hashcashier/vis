using UnityEngine;
using System.Collections;

public class SceneSwitch : MonoBehaviour {

	public GameObject[] cameras;

	void disableAllBut(GameObject[] objs, int but) {
		for (int i = 0; i < objs.Length; i++)
			objs [i].SetActive (i == but);
	}

	void Update () {
		if (Input.GetKey (KeyCode.LeftControl)) {
			if (Input.GetKeyDown (KeyCode.Alpha1)) {
				Application.LoadLevel (0);
			} else if (Input.GetKeyDown (KeyCode.Alpha2)) {
				Application.LoadLevel (1);
			} else if (Input.GetKeyDown (KeyCode.Alpha3)) {
				Application.LoadLevel (2);
			} else if (Input.GetKeyDown (KeyCode.Alpha4)) {
				Application.LoadLevel (3);
			} else if (Input.GetKeyDown (KeyCode.Alpha5)) {
				Application.LoadLevel (4);
			} else if (Input.GetKeyDown (KeyCode.Alpha6)) {
				Application.LoadLevel (5);
			} else if (Input.GetKeyDown (KeyCode.Alpha7)) {
				Application.LoadLevel (6);
			} else if (Input.GetKeyDown (KeyCode.Alpha8)) {
				Application.LoadLevel (7);
			} else if (Input.GetKeyDown (KeyCode.Alpha9)) {
				Application.LoadLevel (8);
			} else if (Input.GetKeyDown (KeyCode.Alpha0)) {
				Application.LoadLevel (9);
			} else if (Input.GetKeyDown (KeyCode.Plus)) {
				if (Application.loadedLevel < Application.levelCount)
					Application.LoadLevel (Application.loadedLevel + 1);
			} else if (Input.GetKeyDown (KeyCode.Minus)) {
				if (Application.loadedLevel > 0)
					Application.LoadLevel (Application.loadedLevel - 1);
			}
		} else if(Input.GetKey (KeyCode.LeftAlt)) {
			if (Input.GetKeyDown (KeyCode.Alpha1)) {
				disableAllBut(cameras, 0);
			} else if (Input.GetKeyDown (KeyCode.Alpha2)) {
				disableAllBut(cameras, 1);
			} else if (Input.GetKeyDown (KeyCode.Alpha3)) {
				disableAllBut(cameras, 2);
			} else if (Input.GetKeyDown (KeyCode.Alpha4)) {
				disableAllBut(cameras, 3);
			} else if (Input.GetKeyDown (KeyCode.Alpha5)) {
				disableAllBut(cameras, 4);
			} else if (Input.GetKeyDown (KeyCode.Alpha6)) {
				disableAllBut(cameras, 5);
			} else if (Input.GetKeyDown (KeyCode.Alpha7)) {
				disableAllBut(cameras, 6);
			} else if (Input.GetKeyDown (KeyCode.Alpha8)) {
				disableAllBut(cameras, 7);
			} else if (Input.GetKeyDown (KeyCode.Alpha9)) {
				disableAllBut(cameras, 8);
			} else if (Input.GetKeyDown (KeyCode.Alpha0)) {
				disableAllBut(cameras, 9);
			}
		}
	}
}
