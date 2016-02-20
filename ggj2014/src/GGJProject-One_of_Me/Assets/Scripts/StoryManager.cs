using UnityEngine;
using System.Collections;

public class StoryManager : MonoBehaviour {

	public int state = 42;

	void Awake() {
		DontDestroyOnLoad(transform.gameObject);
	}

	// Use this for initialization
	void Start () {
	}
	
	// Update is called once per frame
	void Update () {
		if (Input.GetKeyDown ("space")) {
			print("state is : " + state);
		}
	}


}
