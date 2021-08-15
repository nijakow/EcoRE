
(defstruct parser
  tokenizer)

(defun parser-check (parser type)
  (tokenizer-check (parser-tokenizer parser) type))

(defun parser-expect (parser type)
  (tokenizer-expect (parser-tokenizer parser) type))

(defun parser-parse-expression (parser &optional (allow-followups t))
  (let ((expr (parser-parse-simple-expression parser))
        (prev nil))
    (until (eq expr prev)
      (setf prev expr)
      ;; TODO: Check for "(" and assignments
      (setf expr (parser-parse-send parser allow-followups)))))

(defun parser-parse-expressions-until (parser eof-token)
  (unless (parser-check parser eof-token)
    (let ((tokens '()))
      (loop (push (parser-parse-expression parser) tokens)
            (when (parser-check parser eof-token)
              (return (reverse tokens)))
            (parser-expect parser :separator)))))

(defun parser-parse-file (parser)
  (parser-expressions-until parser :eof))
