using UnityEngine;
using System.Collections;

public class DudeController : MonoBehaviour {

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
		if (Input.GetKeyDown ("return")) {
			StoryManager storyManager = GameObject.Find ("PersistenceManager").GetComponent<StoryManager>();
			storyManager.state += 1;
			print ("incremented value");
		}

		if (Input.GetKeyDown ("a")) {
			Application.LoadLevel ("Game");
		}
	}

}
