using UnityEngine;
using System.Collections;

public class Remover4 : MonoBehaviour
{
	public GameObject splash;
	
	void OnTriggerEnter2D(Collider2D col)
	{
		print ("hit");
		if (Input.GetKeyDown (KeyCode.UpArrow)) {
			Application.LoadLevel("room4");
		}
	}


}
