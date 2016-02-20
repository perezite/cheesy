using UnityEngine;
using System.Collections;

public class Actor3Dialog : AbstractDialog {

	public override void LoadDialog() {

		DialogItem item = new DialogItem();
		item.setStatement ("Who are you?");
		item.addAnswer (new DialogAnswer ("I don’t want to play", "item1"));
		addDialogItem ("start", item);
		
		item = new DialogItem();
		item.setStatement("Play? Who said anything about play");
		item.addAnswer (new DialogAnswer("I DON’T WANT TO PLAY", "item2"));
		addDialogItem("item1", item);
		
		item = new DialogItem ();
		item.setStatement ("Ok…  what would you like to do?");
		item.addAnswer (new DialogAnswer("I want to hear some music", "item3"));
		addDialogItem ("item2", item);

		item = new DialogItem ();
		item.setStatement ("What music would you like?");
		item.addAnswer (new DialogAnswer("Jeremy liked the beatles", "item4"));
		addDialogItem ("item3", item);

		item = new DialogItem ();
		item.setStatement ("Who is jeremy?");
		item.addAnswer (new DialogAnswer("YOU ARE LIKE THE OTHERS, GO AWAY", null));
		addDialogItem ("item4", item);

		setCurrentDialogItemTag("start");
	}

}
