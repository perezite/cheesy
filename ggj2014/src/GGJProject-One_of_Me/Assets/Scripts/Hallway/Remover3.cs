using UnityEngine;
using System.Collections;

public class Remover3 : MonoBehaviour
{
	public GameObject splash;
	
	void OnTriggerEnter2D(Collider2D col)
	{
		print ("hit");
		if (Input.GetKeyDown (KeyCode.UpArrow)) {
			Application.LoadLevel("room3");
		}
	}


}
