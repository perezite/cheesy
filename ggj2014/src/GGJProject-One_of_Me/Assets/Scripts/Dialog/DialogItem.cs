using UnityEngine;
using System.Collections;
using System.Collections.Generic;

/* Contains one frame of dialog shown on the screen 
   (statement and answers)*/
public class DialogItem {
	
	public string statement;
	//public DialogAnswer answer;
	public List<DialogAnswer> answers;

	public DialogItem() {
		answers = new List<DialogAnswer> ();
		//answer = new DialogAnswer("correctAnswer", "This is true");
	}

	public void addAnswer(DialogAnswer answer_) {
		answers.Add(answer_);
	}

	public void setStatement(string statement_) {
		statement = statement_;
	}

	public int getNumAnswers () {
		return answers.Count;
	}

	public DialogAnswer getAnswer(int i ) {
		return answers[i];
	}

}
