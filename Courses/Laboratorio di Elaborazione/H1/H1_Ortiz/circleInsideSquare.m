function M = circleInsideSquare(size,radio)

s = fix(size/2)+1;
r = fix(radio/2);

M = ones(s,s);

for i=1:50
    for j=1:50
        if  j>=(sqrt(r^2-i^2)) 
            M(i,j) = 0;
        end
    end
end

M = [rot90(M); M];
M = [rot90(M,2) M];

end