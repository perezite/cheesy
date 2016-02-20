using UnityEngine;
using System.Collections;

public class InputManager : MonoBehaviour {

	public GameObject actor1;
	public GameObject actor2;

	// Use this for initialization
	void Start () {
	}
	
	// Update is called once per frame
	void Update () {
		if (Input.GetMouseButtonDown (0)) {
			Ray ray = Camera.main.ScreenPointToRay (Input.mousePosition);
			RaycastHit hit;
			if (Physics.Raycast (ray, out hit)) {
				if (hit.collider.tag == "actor1") {
					GameObject gameObject = GameObject.FindGameObjectWithTag("actor1");
					print ("hit actor1");
					gameObject.GetComponent<AbstractDialog>().enabled = true;
				}
				if(hit.collider.tag == "actor2") {
					print ("hit actor2");
					GameObject gameObject = GameObject.FindGameObjectWithTag("actor2");
					gameObject.GetComponent<AbstractDialog>().enabled = true;
				}
			}
		}
	}
}
