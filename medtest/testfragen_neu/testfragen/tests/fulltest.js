// Fragen Editor mytest

function get_questions() {

  load_question(['Welche Antwort ist richtig? (Typ a)', ['falsch', ' '], ['nochmal falsch', ' '], ['immer noch falsch', ' '], ['richtig', ' '], ['immer noch falsch', ' '], 'D', 0, 'none' ]) ;
  load_question(['Welches K&auml;stchen ist richtig? (Typ b)', ['nimm B', ' '], ['nimm B', ' '], ['nimm D', ' '], ['nimm E', ' '], ['nimm D', ' '], ['B', 'B', 'D', 'E', 'D'], 3, 'none' ]) ;
  load_question(['Welche Schlussfolgerung ist richtig? (Typ b)', ['falsche antwort', ' '], ['immer noch falsch', ' '], ['das ist richtig', ' '], ['wieder falsch', ' '], ['und nochmal falsch', ' '], 'C', 2, 'none' ]) ;
  load_question(['Welche Antwort ist richtig (typ k)', ['falsch', ' '], ['auch falsch', ' '], ['nochmal falsch', ' '], ['richtig', ' '], ['wieder falsch', ' '], 'D', 1, 'none' ]) ;
  load_question(['W&auml;hle aus richtig und falsch.. (typ kprim)', ['das ist falsch', ' '], ['das ist richtig', ' '], ['das ist richtig', ' '], ['das ist falsch', ' '], ['', ' '], ['0', '1', '1', '0'], 6, 'none' ]) ;
  
}