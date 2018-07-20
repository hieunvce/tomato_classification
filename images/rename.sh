index=38;
for name in *.png
do
    mv "${name}" "${index}.png"
    index=$((index+1))
done
