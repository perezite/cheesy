using UnityEngine;
using System.Collections;

public class MinimalHUD : AbstractHUD {

	public override void LoadHUDOptions() {
		//options.Add (new HUDOption ("Talk", testCallback4));
		//options.Add (new HUDOption ("Talk", testCallback6));
		//options.Add (new HUDOption ("Talk to guy 2", testCallback2));
		//options.Add (new HUDOption ("Talk to guy 3", testCallback3));
		//options.Add (new HUDOption ("Talk to guy 4", testCallback4));
		options.Add (new HUDOption ("Leave", testCallback5));
	}
	
	void testCallback1() {
		GameObject actor1 = GameObject.FindGameObjectWithTag("actor1");
		actor1.GetComponent<AbstractDialog>().enabled = true;
	}
	
	void testCallback2() {
		GameObject actor2 = GameObject.FindGameObjectWithTag("actor2");
		actor2.GetComponent<AbstractDialog>().enabled = true;
	}

	void testCallback3() {
		GameObject actor3 = GameObject.FindGameObjectWithTag("actor3");
		actor3.GetComponent<AbstractDialog>().enabled = true;
	}
	
	void testCallback4() {
		GameObject actor4 = GameObject.FindGameObjectWithTag("actor4");
		actor4.GetComponent<AbstractDialog>().enabled = true;
	}

	void testCallback5() {
		Application.LoadLevel ("Hallway");
	}

	
	void testCallback6() {
		GameObject actor4 = GameObject.FindGameObjectWithTag("conversation");
		actor4.GetComponent<AbstractDialog>().enabled = true;
	}
}
