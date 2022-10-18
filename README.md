# matchr

match(value, pat ~ body, ...)

```
<pat> ::= (<pat>)                         match (<expr>) when <expr> matches <pat>
        | quote(<expr>)                   match <expr> literally
        | <id>                            match anything, bind identifier
        | .                               match anything
        | any(<pat> ...)                  match when any <pat> matches
        | all(<pat> ...)                  match when all <pat> match
        | none(<pat> ...)                 match when none of the <pat> match
        | sat(<expr>)                     match if <expr>(val) is TRUE
        | app(<expr>, <pat>)              match if <pat> matches result of <expr>(val)
        | <literal>
        | <vec-pat>
        | function(<pat-seq> ...) <pat>
        | while(<pat>) <pat>
        | repeat <pat>
        | for(<pat> in <pat>) <pat>
        | if(<pat>) <pat>
        | if(<pat>) <pat> else <pat>
        | lang(<pat-seq>)
        | {<pat-seq> ...}
        | pat(<pat>)


<vec-pat> ::= dbl(<pat-seq> ...)
            | int(<pat-seq> ...)
            | lgl(<pat-seq> ...)
            | str(<pat-seq> ...)
            | cpx(<pat-seq> ...)
            | raw(<pat-seq> ...)
            | vec(<pat-seq> ...)

<pat-seq> ::= <pat>
            | <pat>, ...
            | <pat>, ...<num>
            | <pat>, ..._<num>
            | <pat>, ...<num>_<num>

<literal> ::= NULL
            | <lgl>
            | <int>
            | <dbl>
            | <str>
            | <dbl>i | reim(<dbl>, <dbl>)
            | raw(0-255)
```
