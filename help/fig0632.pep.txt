;File: fig0632.pep
;Computer Systems
;Figure 6.32, Fourth edition
;
         BR      main
true:    .EQUATE 1
false:   .EQUATE 0
;
LOWER:   .EQUATE 21         ;const int
UPPER:   .EQUATE 65         ;const int
;
;******* bool inRange (int a)
retVal:  .EQUATE 2          ;returned value #2d
a:       .EQUATE 0          ;formal parameter #2d
inRange: LDA     LOWER,i    ;if ((LOWER <= a)
if:      CPA     a,s
         BRGT    else
         LDA     a,s        ;   && (a <= UPPER))
         CPA     UPPER,i
         BRGT    else
then:    LDA     true,i     ;   return true
         STA     retVal,s
         RET0
else:    LDA     false,i    ;   return false
         STA     retVal,s
         RET0
         ;
;******* main ()
age:     .EQUATE 0          ;local variable #2d
main:    SUBSP   2,i        ;allocate #age
         DECI    age,s      ;cin >> age
if2:     LDA     age,s      ;if (
         STA     -4,s       ;store the value of age
         SUBSP   4,i        ;push #retVal #a
         CALL    inRange    ;   (inRange (age))
         ADDSP   4,i        ;pop #a #retVal
         LDA     -2,s       ;load retVal
         BREQ    else2      ;branch if retVal == false (i.e. 0)
then2:   STRO    msg1,d     ;   cout << "Qualified\n"
         BR      endif2
else2:   STRO    msg2,d     ;   cout << "Unqualified\n"
endif2:  ADDSP   2,i        ;deallocate #age
         STOP
msg1:    .ASCII  "Qualified\n\x00"
msg2:    .ASCII  "Unqualified\n\x00"
         .END   
