;;Problem 1
(define firstNon0
  (lambda (expr1 expr2 expr3)
    (if (eq? expr1 0)
        (if (eq? expr2 0)
            (if (eq? expr3 0) 0
                expr3)
            expr2)
        expr1)))
;; end of Problem 1

;;problem 2
(define (tribonnaci n)
  (if (= n 0) 0
      (if (= n 1) 0
          (if (= n 2) 1
          (letrec
              ((tribonnaci2
                (lambda (n a b c d)
                  (cond ((= n 3) d)
                        (#t (tribonnaci2 (- n 1) b c d (+ b c d)))))))
    
            (tribonnaci2 n 0 0 1 (+ 0 0 1)))))))
;; end of problem 2
  
