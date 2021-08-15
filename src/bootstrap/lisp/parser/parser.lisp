
(defstruct parser
  tokenizer)

(defun parser-check (parser type)
  (tokenizer-check (parser-tokenizer parser) type))

(defun parser-expect (parser type)
  (tokenizer-expect (parser-tokenizer parser) type))
