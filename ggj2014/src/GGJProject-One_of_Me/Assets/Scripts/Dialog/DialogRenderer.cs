using UnityEngine;
using System.Collections;

public class DialogRenderer : MonoBehaviour {

	//GameObject gameManager;
	DialogManager dialogManager;
	public GUISkin guiSkin;

	// Use this for initialization
	void Start () {
		dialogManager = GameObject.Find ("GameManager").GetComponent<DialogManager>();
	}
	
	void OnGUI() {

		GUI.skin = guiSkin;

		// print the statement
		GUI.Label (new Rect(100, 100, 100, 50), dialogManager.getCurrentDialogItem().statement);

		// print dialog answers 
		for (int i = 0; i < dialogManager.getCurrentDialogItem().getNumAnswers(); i++) {
			string buttonText = dialogManager.getCurrentDialogItem().getAnswer(i).text;
			if (GUI.Button(new Rect(100, 150 +  i*50, 100, 50), buttonText)) {
				dialogManager.setCurrentDialogItem(dialogManager.getCurrentDialogItem().getAnswer (i).targetTag);
			}
		}

	}
	
	// Update is called once per frame
	void Update () {
	}
}
