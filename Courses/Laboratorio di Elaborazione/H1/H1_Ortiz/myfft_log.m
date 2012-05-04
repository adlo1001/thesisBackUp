function M = myfft_log(M) 
    M = log( 1 + myfft(M) );
end