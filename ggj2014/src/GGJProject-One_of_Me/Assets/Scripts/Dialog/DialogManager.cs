using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class DialogManager : MonoBehaviour {

	private string currentDialogItem;
	public Dictionary<string, DialogItem> items = new Dictionary<string, DialogItem>();

	// Use this for initialization
	void Start () {
		InitDialogs ();
		currentDialogItem = "start";
	}

	public void addDialogItem(string tag, DialogItem item_) {
		items.Add (tag, item_);
	}

	public DialogItem getDialogItem(string tag) {
		return items[tag];
	}

	public DialogItem getCurrentDialogItem() {
		return items [currentDialogItem];
	}

	public void setCurrentDialogItem(string tag) {
		currentDialogItem = tag;
	}
	
	// Update is called once per frame
	void Update () {
	}

	void InitDialogs() {
		DialogItem item = new DialogItem();
		//item.statement = "An apple is a fruit";
		item.setStatement ("Give a bad or a good answer");
		item.addAnswer (new DialogAnswer ("Good answer", "correctAnswer"));
		item.addAnswer (new DialogAnswer ("Bad answer", "wrongAnswer"));
		addDialogItem ("start", item);

		item = new DialogItem();
		item.setStatement("You are right!");
		addDialogItem("correctAnswer", item);

		item = new DialogItem ();
		item.setStatement ("You are wrong!");
		addDialogItem ("wrongAnswer", item);
	}

}
