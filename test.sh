FILES=test/*
for f in $FILES
do
    echo "Processing $f file..."
    ./TomatoClassification $f
done
echo "Finished"
