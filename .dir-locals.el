
((c++-mode . ((indent-tabs-mode . nil)
              (tab-width . 4)
              (c-basic-offset . 4)
              (fill-column . 80)
              (eval . (add-to-list 'auto-mode-alist '("\\.h\\'" . c++-mode)))
              (eval . (if (boundp 'c-offsets-alist)
                          (add-to-list 'c-offsets-alist '(innamespace . -)))))))
