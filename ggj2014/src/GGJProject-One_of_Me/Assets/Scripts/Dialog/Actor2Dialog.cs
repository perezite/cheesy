using UnityEngine;
using System.Collections;

public class Actor2Dialog : AbstractDialog {

	public override void LoadDialog() {

		DialogItem item = new DialogItem();
		item.setStatement ("Hi, I'm actor 2");
		item.addAnswer (new DialogAnswer ("Good answer", "correctAnswer"));
		item.addAnswer (new DialogAnswer ("Bad answer", "wrongAnswer"));
		addDialogItem ("start", item);
		
		item = new DialogItem();
		item.setStatement("You are right!");
		item.addAnswer (new DialogAnswer("Goodbye!", null));
		addDialogItem("correctAnswer", item);
		
		item = new DialogItem ();
		item.setStatement ("You are wrong!");
		addDialogItem ("wrongAnswer", item);
		
		setCurrentDialogItemTag("start");
	}

}
