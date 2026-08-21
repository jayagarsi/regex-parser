grammar regex;

expr
    : LPAR expr RPAR                # Group
    | expr op=(STAR | PLUS | QMARK) # Repeat
    | expr expr                     # Concat
    | expr PIPE expr                # Alt
    | CHAR                          # Literal
    ;

// LEXER RULES
STAR  : '*';
PLUS  : '+';
QMARK : '?';
PIPE  : '|';
LPAR  : '(';
RPAR  : ')';
CHAR  : ('a'..'z'|'A'..'Z'|'0'..'9');//~[*+?|()] ;   // any single char that isn't a metachar


//expr    := term ('|' term)*
//term    := factor*
//factor  := atom postfix?
//postfix := '*' | '+' | '?'
//atom    := CHAR | '(' expr ')'

/*
expr -> '('expr ')'
      | expr '*'
      | expr '+'
      | expr '?' 
      | expr expr
      | expr '|' expr
      | CHAR


 */