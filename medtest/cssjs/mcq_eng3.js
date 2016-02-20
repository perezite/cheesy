
// browser detection
var DOM = ( document.getElementById ) ? true : false ;

// general script variables
var answered = true ;
var answers = 'ABCDE' ;
var choiceNum = 0 ;
var commentNum = 0 ;
var commentsPassed = 0 ;
var i = 0 ;
var labels = new Array ( 'A', 'B', 'C', 'D', 'E') ;
var nextOne = 0 ;
var questionNum = 0 ;
var questions = new Array () ;
	questions.length = 0 ;
var right = 0 ;
var fr_right = 0;							// variables to store the results from the first run
var fr_length = 0;							// when a second run ist started
var rightanswered = false ;
var secondRun = false ;
var studentanswer = 0 ;
var tape = ' ' ;
var wrongOnes = new Array () ;				// array of questions which have been falsely answered
	wrongOnes.length = 0 ;

// question type 3 variables
var rightanswers = new Array () ;
	rightanswers.length = 0 ;
var choices = new Array ([false,0],[false,0],[false,0],[false,0],[false,0]) ;
var selectors = 5 ;

// variables to evaluate free text entry answers
var answer = ' ' ;       					// student answer to question
var wordlength = 0 ;      					// number of characters in the submitted word
var tryagain = 'true' ;    					// a right answer has not yet been found
var optcount = 0 ;       					// number of correct options
var sizelimit = 3 ;      					// smallest word which must be spelled correct
var strip = true ;       					// space stripping control
var caseInsensitive = true ;  				// accept case spelling errors
var byRules = true ;      					// accept common spelling errors
var allowMismatchDefault = true ; 			// accept additional spelling errors	
var allowMismatch = true ; 					// accept additional spelling errors	



function load_question ( questionData ) {
	questions[questions.length] = questionData ;
	if ( questionData[7] == 5 ) commentNum ++
} // end load_question


function shuffle () {
	var temp = new Array() ;				// temporary variable storage
	var a = 0 ;								// random number 1
	var b = 0 ;								// random number 2
		for ( i = 0 ; i < questions.length * 5; i ++ ) {
		a = Math.round(Math.random() * (questions.length - 1)) ;
		b = Math.round(Math.random() * (questions.length - 1)) ;
		temp = questions[a] ;
		questions[a] = questions[b] ;
		questions[b] = temp
	}		
} // end shuffle


function select_number () {
	var numberSelected = questions.length ;
	var useqs = 0 ;
	numberSelected = prompt( 'Zu diesem Thema gibt es ' + questions.length + ' Fragen. Wieviele Fragen moechten Sie davon bearbeiten?', questions.length ) ;
	useqs = numberSelected + 0 ; 			// this line and next line: variable type casting for NS6
	useqs = useqs/10 ;
	if ( useqs < questions.length ) questions.length = useqs
} // end select_number


function set_question () {

	selectors = 5 ;
	
	// test if all questions have been set
	if ( nextOne == questions.length && !secondRun ) 
	{
		eval_tape = '';
		eval_tape += 'Auswertung:\n\n';
		eval_tape += 'Fragen: ';
		eval_tape += questions.length - commentNum ;
		eval_tape += '\n';
		eval_tape += 'Davon richtig beantwortet: ';
		eval_tape += right;
		eval_tape += '\n';
		eval_tape += 'Gratulation zum perfekten Ergebnis'
		alert(eval_tape);	
		if ( DOM ) document.getElementById('Qinterface').innerHTML = '';
	}
	if ( nextOne == 0 && secondRun ) 
	{
		eval_tape = '';
		eval_tape += 'Auswertung:\n\n';
		eval_tape += 'Fragen: ';
		eval_tape += fr_length - commentNum ;
		eval_tape += '\n';
		eval_tape += 'Davon richtig beantwortet: ';
		eval_tape += fr_right;
		eval_tape += '\n';
		alert(eval_tape);
		alert('Sie können jetzt die falsch beantworteten Fragen noch einmal durchgehen?\r Die richtigen Antworten werden diesmal angezeigt.') ;
	}
	if ( nextOne == questions.length && secondRun ) 
	{
		eval_tape = '';
		eval_tape += 'Auswertung für den zweiten Lauf:\n\n';
		eval_tape += 'Fragen: ';
		eval_tape += questions.length - commentNum ;
		eval_tape += '\n';
		eval_tape += 'Davon richtig beantwortet: ';
		eval_tape += right;
		eval_tape += '\n';
		alert(eval_tape);
		if ( DOM ) document.getElementById('Qinterface').innerHTML = '';
	}
	else {
		// test if the last question was answered
		if ( !answered ) alert ( 'Bitte beantworten Sie die Frage.')

		else {
		
			tape = ' ' ;
			tape += '<table cellspacing="5" cellpadding="0" border="0"><tr><td><p>' + questions[nextOne][0] + '<br>&nbsp;</p></td>';

			// add an image if required
			if ( questions[nextOne][8] != 'none' ) tape += '<td rowspan="2" align="center"><img src="' + questions[nextOne][8] + '"></td>'
			else tape += '<td><td>' ;
			// alert (questions[nextOne][8]) ;

			tape += '</tr><tr><td><form name="stInput" action="javascript: void(0);"><table border="0" cellspacing="2" cellpadding="3">' ;
			
			// generate selection table for types 1 - 3
			if ( Math.abs(questions[nextOne][7]) < 3 )
				for ( i = 0 ; i < 5 ; i ++ ) {
					tape += '<tr><td class="tdgrau"><span class="fett">' + labels[i] + '</span></td>' ;
					tape += '<td class="tdgrau"><span class="normal">' + questions[nextOne][i+1][0] + '</span></td>' ;
					tape += '<td><input type="radio" name="answer" value="radiobutton" onclick="studentanswer = ' + i + '; test_answer();"></td></tr>'
				}
				
			// generate section table for type 3
			if ( Math.abs(questions[nextOne][7]) == 3 ) {
				// change type 3 question from a 5 x 5 to a 4 x 4 array if the questiontype is negative
				if ( questions[nextOne][7] == -3 ) selectors = 4 ;
				tape += '<tr><td></td><td></td>' ;
				for ( i = 0 ; i < selectors ; i ++ ) tape += '<td class="tdgrau"><span class="fett">' + labels[i] + '</span></td>' ;
				tape += '<tr>' ;
				for ( i = 0 ; i < selectors ; i ++ ) {
					tape += '<tr><td class="tdgrau"><span class="fett">' + (i+1) + '</span></td>' ;
					tape += '<td class="tdgrau"><span class="normal">' + questions[nextOne][i+1][0] + '</span></td>' ;
					for ( m = 0 ; m < selectors ; m ++ ) tape += '<td><input type="radio" name="ans' + i + '" value="radiobutton" onclick="test_answer_3 (' + i + ',' + m + ');"></td>' ;
					tape += '</tr>'
				}
			}
			
			// generate selection table for type 6 (kprim)
			if ( Math.abs(questions[nextOne][7]) == 6 )
			{
				selectors = 4 ;
				tape += '<tr><td></td><td></td>' ;
				tape += '<td class="tdgrau"><span class="fett">wahr</span></td>' ;
				tape += '<td class="tdgrau"><span class="fett">falsch</span></td>' ;				
				tape += '<tr>' ;
				for ( i = 0 ; i < selectors ; i ++ ) 
				{
					tape += '<tr><td class="tdgrau"><span class="fett">' + (i+1) + '</span></td>' ;
					tape += '<td class="tdgrau"><span class="normal">' + questions[nextOne][i+1][0] + '</span></td>' ;
	
					tape += '<td><input type="radio" name="ans' + i + '" value="radiobutton" onclick="test_answer_6 (' + i + ',1);"></td>' ;	// answer: 'richtig'
					tape += '<td><input type="radio" name="ans' + i + '" value="radiobutton" onclick="test_answer_6 (' + i + ',0);"></td>' ;	// answer: 'falsch'
					tape += '</tr>'
				}
			}
			
			// generate input field for free text answers
			if ( Math.abs(questions[nextOne][7]) == 4 ) {
				tape += '<tr><td><p class="smalltext">Geben Sie bitte die Antwort ein.<br><input type="text" name="studentInput" size="30">' ;
				tape += '<input type="button" name="test" value="Antwort testen" onclick="test_answer_4(forms[0].studentInput.value)"></td></tr>' ;
				// set allowmismatch for spellchecking
				if ( questions[nextOne][7] == -4 ) allowMismatch = false
				else allowMismatch = allowMismatchDefault
			}

			tape += '</table></form></td></tr><tr><td colspan="2"><table cellspacing="0" cellpadding="5" border="0"><tr>' ;
			tape += '<td class="tdgrau"><p class="textklein">Fragen in diesem Pool: ' ;
			tape += questions.length - commentNum ;

			if ( Math.abs(questions[nextOne][7]) == 5 ) commentsPassed ++ ;
			
			tape += '</p></td><td class="tdgrau"><span class="textklein">Fragen gestellt: ' + (nextOne+1-commentsPassed) +'</span></td>' ;
			tape += '<td class="tdgrau"><span class="textklein">davon bisher richtig beantwortet: ' + right +'</span></td>' ;
			tape += '<td><span class="normal"><a href="javascript:set_question();">n&auml;chste Frage</a></span></td></tr></table></td></tr></table>' ; 
	 
			if ( Math.abs(questions[nextOne][7]) != 5 ) answered = false ;
	 
			//reset variables for type 3 questions	
			choiceNum = 0 ;
			for ( i = 0 ; i < choices.length ; i ++ ) {
				choices[i][0] = false ;
				choices[i][1] = 0
			}
			
			// advance questioncounter if the entry is a comment
			if ( Math.abs(questions[nextOne][7]) == 5 ) nextOne ++ ;

			// diplay question			
			if ( DOM ) document.getElementById('Qinterface').innerHTML = tape
						
		}
	}
} // end set_question


function test_answer_3 ( statement, choice ) {

	rightanswered = true ;

	if ( answered ) alert ( 'Bitte lassen Sie zuerst eine neue Frage stellen.' )
	else {

		// record the first answer of a student to part of the question
		if ( !choices[statement][0] ) {
			choices[statement][0] = true ;
			choiceNum ++ ;
			choices[statement][1] = choice
		}
		// record changes to the answer of a part of the questions
		if ( choices[statement][0] ) choices[statement][1] = choice ;

		// test answers when one choice was made for each term
		if ( choiceNum == selectors ) {

			for ( i = 0 ; i < selectors ; i ++ ) rightanswers[i] = eval ( '"' + answers + '"' + '.indexOf(questions[' + nextOne + '][6][' + i + '])' ) ;
			for ( i = 0 ; i < selectors ; i ++ ) if ( choices[i][1] != rightanswers[i] ) rightanswered = false ;
			if ( rightanswered ) right ++ ;

			// show the result
			feedback ( rightanswered ) ;
		}
	}
} // end test_answer_3

// test answer function for question type 6 (kprim)
function test_answer_6 ( statement, choice ) {

	rightanswered = true ;

	if ( answered ) alert ( 'Bitte lassen Sie zuerst eine neue Frage stellen.' )
	else {

		// record the first answer of a student to part of the question
		if ( !choices[statement][0] ) {
			choices[statement][0] = true ;
			choiceNum ++ ;
			choices[statement][1] = choice
		}
		// record changes to the answer of a part of the questions
		if ( choices[statement][0] ) choices[statement][1] = choice ;

		// test answers when one choice was made for each term
		if ( choiceNum == selectors ) {
		
			correct_answers = questions[nextOne][6];
			students_answers = ['', '', '', '', '', ''];
			// generate the array with students answers
			for ( i = 0; i < selectors; i++)
			{
				students_answers[i] = choices[i][1];
			}
			
			// check all the answers for correctness
			for (i = 0; i < selectors; i++)
			{
				if(students_answers[i] != correct_answers[i])
				{
					rightanswered = false;
				}
			}
	
			if ( rightanswered ) right ++ ;

			// show the result
			feedback ( rightanswered ) ;
		}
	}
} // end test_answer_5

function test_answer () {

	rightanswered = false ;
 	// test if the question has been answered already
	if ( answered ) alert ( 'Bitte lassen Sie zuerst eine neue Frage stellen.' )
	else {
		rightanswers.length = 0 ;
		// determine the numerical value of the right answer
	 	rightanswers[0] = eval ( '"' + answers + '"' + '.indexOf(questions[' + nextOne + '][6])' )
	  // compared checked radiobox with the numerical value of the right answer
		rightanswered = studentanswer == rightanswers[0] ;
	  if ( rightanswered ) right ++ ;
	  // show the result
		feedback(rightanswered)
  }
} // end test_answer



function feedback ( isitright ) {

	var showComment = ' ' ;

	if ( isitright ) {
		// show a comment if present
	  if ( questions[nextOne][studentanswer + 1].length > 1  && questions[nextOne][7] != 6)
			if ( questions[nextOne][studentanswer + 1][1] != ' ') showComment += '- ' + questions[nextOne][studentanswer + 1][1] ;
		alert ('richtig' + showComment )
	}
	else {
		// show a coment if present
		if ( questions[nextOne][studentanswer + 1].length > 1 && questions[nextOne][7] != 6)
			if ( questions[nextOne][studentanswer + 1][1] != ' ') showComment += '- ' + questions[nextOne][studentanswer + 1][1] ;
		// show the correct answer in the second run
		if ( secondRun ) {
			if ( Math.abs(questions[nextOne][7]) != 3 &&  Math.abs(questions[nextOne][7]) != 6) 
			{
				showComment += ' - die richtige Antwort ist ' + questions[nextOne][6] ;
			}
			if ( Math.abs(questions[nextOne][7]) == 3 ) showComment += ' - die richtigen Antworten sind ' + questions[nextOne][6].concat()
			if ( Math.abs(questions[nextOne][7]) == 6 )	// kprim 
			{
				// transform array of correct answers to a string
				/*correct_answers = questions[nextOne][6];
				correct_answers_string = ['', '', '', ''];
				for( i = 0; i < selectors; i ++)
				{
					if(correct_answers[i] == 0)
						correct_answers_string[i] = 'falsch';
					if(correct_answers[i] == 1)
						correct_answers_string[i] = 'richtig';
				}
				showComment += ' - die korrekten Antworten sind:' + correct_answers_string.concat()*/
				// generate the array with students answers
				students_answers = ['', '', '', '', '', ''];
				for ( i = 0; i < selectors; i++)
				{
					students_answers[i] = choices[i][1];
				}
				// show comments for falsely answered questions, if there are any
				for ( i = 0; i < selectors; i++)
				{
					if(students_answers[i] != correct_answers[i])
					{
						showComment += '\nDie korrekte Antwort auf Frage ' + (i+1) + ' wäre ';
						if(correct_answers[i] == 0)
							showComment += '\"falsch\"';
						else
							showComment += '\"wahr\"';
						// now show the comment, if present
						curr_comment = questions[nextOne][i+1][1]; 
						if(curr_comment != ' ' && curr_comment != '')
							showComment += ": " + curr_comment;
					}
				}
			}
		}
		alert ('leider nicht richtig' + showComment ) ;
		if ( !secondRun ) wrongOnes[wrongOnes.length] = questions[nextOne]
	}

	// register a falsely answered question and reset variables
  nextOne ++ ;
	answered = true ;
	second_run ()

} // end feedback



function second_run () {
	if ( nextOne == questions.length && wrongOnes.length > 0 && !secondRun ) {
		secondRun = true ;
		for ( i = 0; i < wrongOnes.length; i++ ) questions[i] = wrongOnes[i] ;
		fr_length = questions.length;
		fr_right = right;
		questions.length = wrongOnes.length ;
		commentsPassed = 0 ;
		right = 0 ;
		nextOne = 0
	}
} // end second_run


// The following function stripps leading and tailing spaces and replaces multiple spaces bewteen words by a single space

function strippSpaces ( word ) {

	var y = 0 ;										// counter variable
	var stripped = new Array () ;	// word stripping array
	strip = true ;	

	// replace all double spaces by single spaces
	while ( strip ) {
		if ( word.search('  ') != -1 ) word = word.replace('  ',' ') ;
		if ( word.search('  ') == -1 ) strip = false
	}	

	// determine the number of remaining characters in the submitted word
	wordlength = word.length ;

	// convert the word string to an array
	for ( y = 0 ; y < wordlength ; y ++ ) stripped[y] = word.charAt(y) ;

	// remove tailing space from students answer
	if ( stripped[stripped.length-1] == ' ' ) stripped.length = stripped.length - 1 ;

	// remove leading space
	stripped.reverse() ;
	if ( stripped[stripped.length-1] == ' ' ) stripped.length = stripped.length - 1 ;
	stripped.reverse() ;
 
	// return the array as a string
	return stripped.join('')
} // end strippSpaces

/* The following function condenses the students answer to the minimum of characters
considered to represent the correct answer */

function condense ( word, justLast ) {

	var condensed = new Array () ;				// word condensation array
	var m = 0 ;														// counter variable
	var n = 0 ;														// counter variable
	var endings = new Array () ;        	// word endings to strip
			endings.length = 0 ;
	var excludeList = [ ' ', 'h', ',', '-', '.' ] ; // characters to be excluded from the student answer
	
	for ( i = 0 ; i < endings.length ; i ++ ) word = word.replace(endings[i]," ") ;
		
	// determine the number of characters in the submitted word
	wordlength = word.length ;
	
	// condense answer (strip double letters, final e, final es, middle h, spaces and dashes)
	for ( i = 0; i < wordlength; i++ ) {
		if ( i == 0 ) {
			includeLetter = true ;
			for ( n = 0; n < excludeList.length; n ++ ) if ( word.charAt(i) == excludeList[n] ) includeLetter = false ;
			if ( includeLetter || word.charAt(i) == "h" ) {
				condensed[m] = word.charAt(i);
				m ++
			}
		}
			
		if ( i != 0 ) {
			includeLetter = true ; 
			for ( n = 0; n < excludeList.length; n ++ ) {
				if ( excludeList[n] == ' ' ) lateSpaceDrop = true ;
				if ( word.charAt(i) == excludeList[n] ) includeLetter = false ;
				if ( word.charAt(i) == ' ' && justLast ) includeLetter = true
			}
			if ( includeLetter ) {
				if ( m == 0 ) {
					condensed[m] = word.charAt(i) ;
					m ++
				}	
				if ( m != 0 && word.charAt(i) != condensed[m-1] ) {
					condensed[m] = word.charAt(i) ;
					m ++
				}
			}
		}
	}

	// join the remaining characters into the condensed word and return it */
	word = condensed.join('') ;
	return word 
} // end condense

// The following function compares two strings and returns true if there is only one mismatch or one transposition of characters

function justOneWrong ( word, correct ) {

	var lengthDifference = 0 ;
	var stillOK = true ;
	var mismatches = 0 ;
	var mismatchlimit = 1 ;

	lengthDifference = word.length - correct.length ;
	if ( Math.abs(lengthDifference) > 1 ) stillOK = false ;
 
	if ( lengthDifference == 0 )
		for ( l = 0 ; l < word.length ; l ++ )
			if ( word.charAt(l) != correct.charAt(l) ) 
				if ( word.charAt(l) == correct.charAt(l+1) && word.charAt(l+1) == correct.charAt(l) ) l ++ 
				else mismatches ++ ;
	if ( mismatches > mismatchlimit ) stillOK = false ;	

	if ( stillOK && lengthDifference == 1 )
		for ( l = 0 ; l < correct.length ; l ++ )
			if ( word.charAt(l) != correct.charAt(l) )
				if ( word.substr( l + 1) != correct.substr(l) ) stillOK = false ;
      
	if ( stillOK && lengthDifference == -1 )
		for ( l = 0 ; l < word.length ; l ++ )
			if ( word.charAt(l) != correct.charAt(l) )
				if ( correct.substr( l + 1) != word.substr(l) ) stillOK = false ;

	return stillOK
} // end justOneWrong


// The following functions analyse the student answer against the possible correct answers.

function analyse ( answer, correct ) {

	var i = 0 ;		// counter variable
	var x = 0 ;		// counter variable
	var k = 0 ;		// counter variable
	var tempcorrect = ' ' ;
		
	// check for a perfect match
	if ( answer == correct ) {
		tryagain = false ;
		alert ( "Richtig. Unsere herzlichsten Glückwünsche!" )
	}
	
	// check for a case-insensitive match
	if ( tryagain && caseInsensitive )
		if ( answer.toLowerCase() == correct.toLowerCase() && tryagain ) {
			tryagain = false ;
			alert ( "Tja, die Gross- und Kleinschreibung, aber sonst richtig. Weiter so!" )
		}
					
	// check for a case-insensitive match allowing for common spelling errors
	if ( tryagain && byRules ) {
		if ( correct.toLowerCase().length > sizelimit ) {
			if ( condense(answer.toLowerCase()) == condense(correct.toLowerCase()) ) {
				tryagain = false ;
				alert ( 'Die richtige Antwort ist schon "' + questions[nextOne][6][optcount] + '", aber es schreibt sich doch etwas anders als Sie es vorschlagen. Naja ... richtig.')
			}
		}
	}
  
	// check for a case-insensitive imperfect match accepting one missing, added or incorrect character
	if ( tryagain && allowMismatch ) {
		if ( answer.toLowerCase().length > sizelimit ) {
			if ( justOneWrong( condense( answer.toLowerCase()), condense ( correct.toLowerCase()) ) || justOneWrong( answer, correct.toLowerCase() ) ) {
				tryagain = false ;
				alert ('Der Computer erkennt noch, dass Sie wohl "' + questions[nextOne][6][optcount] + '" meinen. Ihre Rechtschreibung ist allerdings verbesserungsbedürftig. Aber wenn wir ein Auge fest zudrücken .... richtig.') ;
			}
		}
	}
		
}	// end analyse


function test_answer_4 (answer) {
	if ( answered ) alert ('Bitte lassen Sie zuerst eine neue Frage stellen.')
	else {
		tryagain = true ;
		answer = strippSpaces(answer) ;
		for ( optcount = 0 ; optcount < questions[nextOne][6].length ; optcount ++ ) if ( tryagain ) analyse ( answer, questions[nextOne][6][optcount] ) ;
		if ( !tryagain ) right ++ ;
		if ( tryagain ) {
			alert (' Leider falsch - auch mit gutem Willen koennen wir aus Ihrem Vorschlag "' + answer + '" kein "' + questions[nextOne][6][0] + '" machen.') ;
			if ( !secondRun ) wrongOnes[wrongOnes.length] = questions[nextOne]
		}
  	nextOne ++ ;
		answered = true ;
		second_run ()
	}
} // end test_answer4