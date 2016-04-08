for h in $(pgrep hailhydra)
do 
    kill -9 $h; 
done;
