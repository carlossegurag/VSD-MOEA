for i in ../Code/POF/*
do
 tail -100 $i > LastFronts/$(basename ${i})
done

