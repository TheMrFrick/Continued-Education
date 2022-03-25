%Problem III
%creating people.
person(joe).
person(sam).

%creating departments
department(toy).

%stating joe and sam in toy department
inside(joe, toy).
inside(sam, toy).

%stating head of toy department
head(sam, toy).

%stating that joe reports to sam
report(X, Y, department) :- inside(x, department), head(y, department).

%rule for paid less.
paid_less(X,Y):-report(X,Y,Z).

%stating that joe is paid less
paid_less(joe,sam).

is_head(H):-report(H,_).
%END OF PROBLEM III

%Problem IV
trib(0,0).
trib(1,0).
trib(2,1).
add(0, X,_Y,_Z,X). 
add(1,_X, Y,_Z,Y).
add(2,_X,_Y, Z,Z). 
add(N,X,Y,Z,Next):-N > 2,NN is N-1,W is X+Y+Z,add(NN,Y,Z,W,Next).
trib(N,T):-add(N,0,0,1,T).
%END OF PROBLEM IV

%Problem V
double([], []).
double(A,B) :-  double_help(A,A).

double_help([], []).
double_help([A | X], [A, A | Y]) :- double_help(X, Y).
%end of Problem V 

%problem VI
suppressEchos([], []).
suppressEchos(A,B):- list_to_set(A, B).