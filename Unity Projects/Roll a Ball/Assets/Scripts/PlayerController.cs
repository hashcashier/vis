using UnityEngine;
using UnityEngine.UI;
using System.Collections;

public class PlayerController : MonoBehaviour {

    public float speed;
    private Rigidbody rb;

    private int count;
    public Text countText;

    public Text winText;

    // Use this for initialization
    void Start () {
        rb = GetComponent<Rigidbody>();
        count = 0;
        updateCountText();
        winText.text = "";
    }
	
    // Update is called once per frame
    void Update () {

    }

    void FixedUpdate () {
        float moveHorizontal = Input.GetAxis("Horizontal");
        float moveVertical = Input.GetAxis("Vertical");

        Vector3 movement = new Vector3(moveHorizontal, 0f, moveVertical);

        //rb.AddForce(movement * speed);
		rb.velocity = movement * speed;
    }

    void OnTriggerEnter(Collider other) {
        if (other.gameObject.CompareTag("Pick Up")) {
            other.gameObject.SetActive(false);
            count++;
            updateCountText();
        }
    }

    void updateCountText() {
        countText.text = "Count: " + count.ToString();
        if (count >= 8)
            winText.text = "You Win!";
    }

    int getCount() {
        return count;
    }
}
