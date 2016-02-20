#pragma strict

var LookAtTrgt : GameObject;
var RotTrgt : GameObject;

function Start () {

	RotTrgt = GameObject.Find("RotationTrgt");

}
 
function Update(){

	
	transform.RotateAround(RotTrgt.transform.position, Vector3.forward, 50*Time.deltaTime);
	transform.LookAt(LookAtTrgt.transform.position);
	//transform.RotateAround(
	//transform.rotation.z = 0;
}
 
function Shake(){
   
}