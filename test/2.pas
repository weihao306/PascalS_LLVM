program sort(input,output);

var i,x:integer;
    a:array[0..100] of integer;

procedure quiksort(low,high:integer);
    var i,j,mid,temp:integer;
    begin
        i:=low;
        j:=high;
        mid:=a[(i+j)div 2];
        repeat
            while a[i]<mid do i:=i+1;
            while mid<a[j] do j:=j-1;
            if i<=j then
            begin
                temp:=a[i];
                a[i]:=a[j];
                a[j]:=temp;
            end;
        until (i>j);
        if i<high then quiksort(i,high);
        if j>low then quiksort(low,j);
    end;

begin
    read(n);
    for i:=1 to n do
        read(a[i]);
    quiksort(i,n);
    for i:=1 to n do
        write(a[i], ' ');
end.