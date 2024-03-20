%define next 0

%macro colon 2
    %ifstr %1
        %ifid %2
            %2:
            dq next
            db %1, 0

            %define next %2
        %else
            %error "second argument must be id"
        %endif
    %else
        %error "first argument must be string"
    %endif
%endmacro