% parent(X, Y):  Y is X's parent
parent(bob, ann).
parent(chuck, bob).
parent(bob, al).
parent(chuck, barbara).
parent(barbara, arthur).
parent(barbara, abby).

% anc_bad(X, Y): Y is X's ancestor.  Gives infinite recursion
anc_bad(X, Y):- anc_bad(X, Z), parent(Z, Y).
anc_bad(X, Y):- parent(X, Y).

% anc_good(X, Y):  Y is X's ancestor.  This order works.
anc_good(X, Y):- parent(Z, Y), anc_good(X, Z).
anc_good(X, Y):- parent(X, Y).

onemore([],[]).
onemore([H1 | T1], [H2|T2]) :- onemore(T1,T2), H2 is H1 +1.

lst([a,b,c,d], d).
lst([a],a).
lst([X|T],Y) :- last(T,Y).

lngth([],0).
lngth([X|T], N) :-lngth(T,N1), N is N1+1.