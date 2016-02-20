#pragma strict

var state : int;
var door1Active : boolean;
var door2Active : boolean;
var door3Active : boolean;
var door4Active : boolean;
var door5Active : boolean;
var door6Active : boolean;


function Start () {

}

function Update (){

	if(Input.GetKey("left")){
		state = 1;
	}
	if(Input.GetKey("right")){
		state = 2;
	}else{
		state = 0;
	}
	if(Input.GetKey("space")){
		if(door1Active == true){
			//Exec Room 1 Scene
		}
		if(door2Active == true){
			//Exec Room 1 Scene
		}
		if(door3Active == true){
			//Exec Room 1 Scene
		}
		if(door4Active == true){
			//Exec Room 1 Scene
		}
		if(door5Active == true){
			//Exec Room 1 Scene
		}
		if(door6Active == true){
			//Exec Room 1 Scene
		}
		
	}
	
	if(state == 0){
		//Playanimation Idle
	}
	if(state == 1){
		//Playanimation LeftWalking
		transform.Translate(-(Time.deltaTime/2), 0, 0);
	}
	if(state == 2){
		//Playanimation RightWalking
		transform.Translate((Time.deltaTime/2), 0, 0);
	}
	
}