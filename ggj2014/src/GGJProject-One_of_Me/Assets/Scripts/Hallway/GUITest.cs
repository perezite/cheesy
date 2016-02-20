using UnityEngine;
using System.Collections;

public class GUITest : MonoBehaviour {
	private string stringToMod= "Loader MenuDSADSADSADSADSADSADSADSADSA DSADSADSADSADSADASDsadsadsadsadsa dsadslkadsdsalkdsalklkdalklklkdlklklklklklkdsalksalks Loader MenuDSADSADSADSADSADSADSADSADSA DSADSADSADSADSADASDsadsadsadsadsa dsadslkadsdsalkdsalklkdalklklkdlklklklklklkdsalksalks Loader MenuDSADSADSADSADSADSADSADSADSA DSADSADSADSADSADASDsadsadsadsadsa dsadslkadsdsalkdsalklkdalklklkdlklklklklklkdsalksalks Loader MenuDSADSADSADSADSADSADSADSADSA DSADSADSADSADSADASDsadsadsadsadsa dsadslkadsdsalkdsalklkdalklklkdlklklklklklkdsalksalks";

	public BoxCollider2D collider1;

	public GUISkin guiskin;
	void OnGUI () {
		print ("fuck you3times");
		GUIStyle style1= guiskin.GetStyle("Box");
		// Make a background box
		GUI.Box(new Rect(10,10,100,90), "Loader MenuDSADSADSADSADSADSADSADSADSA DSADSADSADSADSADASDsadsadsadsadsa dsadslkadsdsalkdsalklkdalklklkdlklklklklklkdsalksalks",style1);
		GUI.TextArea (new Rect (655, 35, 500, 100), stringToMod);
		// Make the first button. If it is pressed, Application.Loadlevel (1) will be executed
		if(GUI.Button(new Rect(20,40,80,20), "Level 1")) {
			stringToMod = "mutate!";

		}

		// Make the second button.
		if(GUI.Button(new Rect(20,70,80,20), "Level 2")) {
			stringToMod = "mutate2";
		}
	}




}