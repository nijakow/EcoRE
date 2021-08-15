
(defstruct tokenizer
  stream pushbacks)

(defstruct token
  tokenizer type meta)

(defun eco-whitespace-char-p (c)
  (or (char= c #\space)
      (not (graphic-char-p c))))

(defun eco-special-char-p (c)
  (member c '(#\( #\)
              #\[ #\]
              #\{ #\}
              #\. #\, #\: #\;
              #\^ #\|)))

(defun tokenizer-peek (tokenizer)
  (peek-char (tokenizer-stream tokenizer)))

(defun tokenizer-read (tokenizer)
  (read-char (tokenizer-stream tokenizer)))

(defun tokenizer-slurp (tokenizer)
  (while (eco-whitespace-char-p (tokenizer-peek tokenizer))
    (tokenizer-read tokenizer)))

(defun tokenizer-read (tokenizer)
  (when (tokenizer-pushbacks tokenizer)
    (return (pop (tokenizer-pushbacks tokenizer))))
  (tokenizer-slurp tokenizer)
  (let ((c (tokenizer-read tokenizer)))
    (case (c)
      ((#\() (make-token :tokenizer tokenizer
                         :type :lparen))
      ((#\)) (make-token :tokenizer tokenizer
                         :type :rparen))
      ((#\[) (make-token :tokenizer tokenizer
                         :type :lbrack))
      ((#\]) (make-token :tokenizer tokenizer
                         :type :rbrack))
      ((#\{) (make-token :tokenizer tokenizer
                         :type :lcurly))
      ((#\}) (make-token :tokenizer tokenizer
                         :type :rcurly))
      ((#\:) (make-token :tokenizer tokenizer
                         :type :colon))
      ((#\. #\, #\;)  ; TODO: Ellipsis "..."
       (make-token :tokenizer tokenizer
                   :type :separator))
      ((#\^) (make-token :tokenizer tokenizer
                         :type :caret))
      ((#\|) (make-token :tokenizer tokenizer
                         :type :bar))
      ((#\#)
       ;; TODO
       (error "Unimplemented"))
      ;; TODO: #\' and #\"
      
      (let ((tok (coerce (cons c
                               (loop for c = (tokenizer-peek tokenizer)
                                     until (or (eco-special-char-p c)
                                               (eco-whitespace-char-p c)
                                               (not c))
                                     collecting c))
                         'string)))
        (case tok
          (("=") (make-token :tokenizer tokenizer
                             :type :equals))
          (("=>") (make-token :tokenizer tokenizer
                              :type :rarrow))
          (("self") (make-token :tokenizer tokenizer
                                :type :self))
          (("with") (make-token :tokenizer tokenizer
                                :type :with))
          (("private") (make-token :tokenizer tokenizer
                                   :type :private))
          (("<-") (make-token :tokenizer tokenizer
                              :type :assignment))
          (t (make-token :tokenizer tokenizer
                         :type :identifier
                         :meta tok)))))))
