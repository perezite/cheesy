using UnityEngine;
using System.Collections;

public class HUDOption {

	public string text;
	public delegate void CommandDelegate();
	public CommandDelegate command;

	public HUDOption (string text_, CommandDelegate command_) {
		text = text_;
		command = command_;
	}
}
