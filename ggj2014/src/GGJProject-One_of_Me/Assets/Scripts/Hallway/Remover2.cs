using UnityEngine;
using System.Collections;

public class Remover2 : MonoBehaviour
{
	public GameObject splash;
	
	void OnTriggerEnter2D(Collider2D col)
	{
		print ("hit");
		if (Input.GetKeyDown (KeyCode.UpArrow)) {
			Application.LoadLevel("room2");
		}
	}


}
