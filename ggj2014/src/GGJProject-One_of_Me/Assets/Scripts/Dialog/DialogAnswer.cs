using UnityEngine;
using System.Collections;

public class DialogAnswer {

	public string targetTag;
	public string text;

	public DialogAnswer(string text_, string targetTag_) 
	{
		targetTag = targetTag_;
		text = text_;
	}

}
