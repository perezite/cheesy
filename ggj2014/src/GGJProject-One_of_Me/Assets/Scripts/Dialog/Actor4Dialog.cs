using UnityEngine;
using System.Collections;

public class Actor4Dialog : AbstractDialog {

	public override void LoadDialog() {

		// start
		DialogItem item = new DialogItem();
		item.setStatement("");
		item.addAnswer(new DialogAnswer("Who are you?", "item1"));
		addDialogItem ("start", item);

		// item1
		item = new DialogItem();
		item.setStatement("I don't want to play");
		item.addAnswer (new DialogAnswer("Play? Who said anything about play", "item2"));
		addDialogItem("item1", item);

		// item2
		item = new DialogItem();
		item.setStatement("I DON’T WANT TO PLAY");
		item.addAnswer (new DialogAnswer("Ok…  what would you like to do?", "item3"));
		addDialogItem("item2", item);

		/// item3
		item = new DialogItem();
		item.setStatement("I want to hear some music");
		item.addAnswer (new DialogAnswer("What music would you like?", "item4"));
		addDialogItem("item3", item);

		/// item4
		item = new DialogItem();
		item.setStatement("Jeremy liked the beatles");
		item.addAnswer (new DialogAnswer("Who is jeremy?", "item4.1"));
		item.addAnswer (new DialogAnswer("I also like the beatles?", "item5"));
		addDialogItem("item4", item);

		// item 4.1
		item = new DialogItem();
		item.setStatement("You are like the others, go away!");
		item.addAnswer (new DialogAnswer("Leave", null));
		addDialogItem("item4.1", item);

		// item5
		item = new DialogItem();
		item.setStatement("He was happy yesterday, I don’t understand");
		item.addAnswer (new DialogAnswer("Who was happy?", "item6"));
		item.addAnswer (new DialogAnswer("What made him happy?", "item7"));
		addDialogItem("item5", item);

		// item6
		item = new DialogItem();
		item.setStatement("You are like the others, go away!");
		item.addAnswer (new DialogAnswer("Leave", null));
		addDialogItem("item6", item);

		// item 7
		item = new DialogItem();
		item.setStatement("I don’t know, he was supposed to be sad. It was a school day!");
		item.addAnswer (new DialogAnswer("You don’t like school?", "item8"));
		item.addAnswer (new DialogAnswer("I also hated school", "item9"));
		item.addAnswer (new DialogAnswer("What happened to him at school?", "item10"));
		item.addAnswer (new DialogAnswer("School is a difficult place for some", "item11"));
		addDialogItem("item7", item);

		// item8 
		item = new DialogItem();
		item.setStatement("It's alright");
		item.addAnswer (new DialogAnswer("Leave", null));
		addDialogItem("item8", item);

		// item9
		item = new DialogItem();
		item.setStatement("No, you didn't");
		item.addAnswer (new DialogAnswer("Leave", null));
		addDialogItem("item9", item);

		// item10
		item = new DialogItem();
		item.setStatement("NOBODY EVEN NOTICED, YOU NEVER HELPED HIM, NOBODY DID");
		item.addAnswer (new DialogAnswer("Leave", null));
		addDialogItem("item10", item);

		// item11
		item = new DialogItem();
		item.setStatement("He hated it, the other kids never stopped.  Every time he tried to do something about it, it just got worse.  I told father to do something but he never had time. ");
		item.addAnswer (new DialogAnswer("What is your father's job", "item12"));
		item.addAnswer (new DialogAnswer("Were is your father?", "item12"));
		item.addAnswer (new DialogAnswer("Do you love your father?", "item12"));
		addDialogItem("item11", item);

		// item12
		item = new DialogItem ();
		item.setStatement ("FUCK DAD");
		item.addAnswer (new DialogAnswer("...", "item13"));
		addDialogItem ("item12", item);

		/*-----------------------*/

		// start
		item = new DialogItem();
		item.setStatement("");
		item.addAnswer(new DialogAnswer("Do you like comics?", "item14"));
		addDialogItem ("item13", item);
		
		// item1
		item = new DialogItem();
		item.setStatement("I guess….    those are jeremy's he liked the fantastic four.  He wanted to be invisible, I liked batman more but I didn’t want to tell him");
		item.addAnswer (new DialogAnswer("You sound like a good friend.", "item15"));
		addDialogItem("item14", item);
		
		// item2
		item = new DialogItem();
		item.setStatement("He was always there for me, I was his only friend and he was my best friend. I never needed anyone else.");
		item.addAnswer (new DialogAnswer("Ok…  what would you like to do?", "item16"));
		addDialogItem("item15", item);
		
		/// item3
		item = new DialogItem();
		item.setStatement("No, I just liked to listen.");
		item.addAnswer (new DialogAnswer("Was this Jeremy's room?", "item17"));
		addDialogItem("item16", item);
		
		/// item4
		item = new DialogItem();
		item.setStatement("No, I just liked to listen.");
		item.addAnswer (new DialogAnswer("Was this Jeremy's room?", "item18"));
		addDialogItem("item17", item);
		
		/// item5
		item = new DialogItem();
		item.setStatement("Father said I could move to the attic when we felt this was too small for us. But we liked sharing the room.");
		item.addAnswer (new DialogAnswer("Who wrote that letter?", "item19"));
		addDialogItem("item18", item);
		
		/// item6
		item = new DialogItem();
		item.setStatement("Stop it!");
		item.addAnswer (new DialogAnswer("Who wrote that letter?", "item20"));
		addDialogItem("item19", item);
		/// item7
		item = new DialogItem();
		item.setStatement("STOP IT!");
		item.addAnswer (new DialogAnswer("Leave", null));
		addDialogItem("item20", item);	

		setCurrentDialogItemTag("start");
	}

}
