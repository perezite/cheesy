using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public abstract class AbstractHUD : MonoBehaviour {

	public GUISkin guiSkin;
	public List<HUDOption> options;

	GameObject actor2;

	// Use this for initialization
	void Start () {
		options = new List<HUDOption> ();

		LoadHUDOptions ();
	}

	public abstract void LoadHUDOptions ();


	// Update is called once per frame
	void Update () {
	}

	void OnGUI() {
		GUI.skin = guiSkin;

		int buttonWidth = (int)(Screen.width * 0.2f);
		int buttonHeight = (int)(Screen.height * 0.1f);
		int buttonLeft = (int)(Screen.width - buttonWidth - Screen.width * 0.05f);
		int buttonTop = (int)(Screen.height * 0.1f);
		
		// show the option bar to the right
		for (int i = 0; i < options.Count; i++) {
			string buttonText = options[i].text;
			if (GUI.Button(new Rect(buttonLeft, buttonTop + i*buttonHeight, buttonWidth, buttonHeight), buttonText)) {
				options[i].command();
			}
		}
	}
}
