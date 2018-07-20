FILES=./images/tomatos/*
for f in $FILES
do
    echo "Processing $f file..."
    ./build/TomatoClassification $f >> pc.txt
done
echo "Finished"
