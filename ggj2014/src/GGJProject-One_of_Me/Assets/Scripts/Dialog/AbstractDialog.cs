using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public abstract class AbstractDialog : MonoBehaviour {
	
	public GUISkin guiSkin;
	private string startDialogItem;
	private string currentDialogItem;
	public Dictionary<string, DialogItem> items = new Dictionary<string, DialogItem>();
	bool isActive;
	float textBuildupIntervalSeconds = 1.0f;
	float textBuildupElapsedSeconds = 0.0f;
	
	public abstract void LoadDialog ();

	// Use this for initialization
	void Start () {
		isActive = true;
		LoadDialog ();
	}
	
	void OnGUI() {
		GUI.skin = guiSkin;
		if (isActive) {
			GUI.skin = guiSkin;
		
			// dimensions for bounding box
			int boundingBoxLeft = (int)(Screen.width * 0.1f);
			int boundingBoxTop = (int)(Screen.height * 0.1f);
			int boundingBoxWidth = (int)(Screen.width * 0.6f);
			int boundingBoxHeight = (int)(Screen.height * 0.8f);

			// draw bounding box
			GUI.Box(new Rect(boundingBoxLeft, boundingBoxTop, boundingBoxWidth, boundingBoxHeight), "");

			// determine the characters actually shown in the statement
			string fullStatementText = getCurrentDialogItem().statement;
			int fullStatementLength = fullStatementText.Length;
			float percentageShown = textBuildupElapsedSeconds/textBuildupIntervalSeconds;
			int charactersShown = (int)(percentageShown*(float)fullStatementLength);
			string statementText = fullStatementText.Substring(0, charactersShown);

			// print the statement
			int dialogLeft = boundingBoxLeft + (int)(Screen.width*0.1f);
			int dialogWidth = (int)(Screen.width*0.4f);
			GUI.Label (new Rect(Screen.width*0.2f, Screen.height*0.2f, dialogWidth, Screen.height*0.3f), statementText);

			// print dialog answers 
			int dialogAnswersTop = (int)(Screen.height*0.5f);
			int dialogAnswerHeight = (int)(Screen.height*0.1f);
			for (int i = 0; i < getCurrentDialogItem().getNumAnswers(); i++) {
				string buttonText = getCurrentDialogItem().getAnswer(i).text;
				if (GUI.Button(new Rect(dialogLeft, dialogAnswersTop + i*dialogAnswerHeight, dialogWidth, dialogAnswerHeight), buttonText)) {
					setCurrentDialogItemTag(getCurrentDialogItem().getAnswer (i).targetTag);
				}
			}
		}
	}
	
	// Update is called once per frame
	void Update () {
		textBuildupElapsedSeconds += Time.deltaTime;
		textBuildupElapsedSeconds = Mathf.Min(textBuildupElapsedSeconds, textBuildupIntervalSeconds);
		//textBuildupElapsedSeconds = textBuildupElapsedSeconds % textBuildupIntervalSeconds;
	}

	public void addDialogItem(string tag, DialogItem item_) {
		// add a default 'goodbye' answer if there is no answer at all
		if (item_.getNumAnswers() == 0) {
			item_.addAnswer(new DialogAnswer("Leave", null));
		}
		items.Add (tag, item_);
	}

	public DialogItem getDialogItem(string tag) {
		return items[tag];
	}
	
	public DialogItem getCurrentDialogItem() {
			return items [currentDialogItem];
	}

	public void setStartDialogItem(string tag) {
		startDialogItem = tag;
		setCurrentDialogItemTag (tag);
	}

	public void setCurrentDialogItemTag(string tag) {
		if (tag != null) { 
			textBuildupElapsedSeconds = 0.0f;
			currentDialogItem = tag; 
		} else {
			isActive = false;
		}
	}
}
