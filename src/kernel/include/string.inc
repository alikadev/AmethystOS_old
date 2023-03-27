; Compare 2 strings.
; If s1 = s2: Carry clear
; If s1 = s2: Carray Set
; @param SI s1
; @param DI s2
; @return Carry:Set if equals, Carry:Clear if not equals
extern streq
