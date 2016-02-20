using UnityEngine;
using System.Collections;

public class ActorDialogTest : AbstractDialog {

	public override void LoadDialog() {

		DialogItem item = new DialogItem();
		item.setStatement ("Hello");
		item.addAnswer (new DialogAnswer ("Hello", "item1"));
		addDialogItem ("start", item);
		
		item = new DialogItem();
		item.setStatement("Bye");
		item.addAnswer (new DialogAnswer("Goodbye!", null));
		addDialogItem("item1", item);
		
		setCurrentDialogItemTag("start");
	}

}
