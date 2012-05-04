function y = blurrySqr( size, std_hor, std_ver)  
    x=0:size;
    y_ver=gaussmf(x,[std_ver fix(size/2)])';
    y_v=repmat(y_ver,1,101);
    y_hor=gaussmf(x,[std_hor fix(size/2)]);
    y_h=repmat(y_hor,101,1);
    y = y_h.*y_v;
end