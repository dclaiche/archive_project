f cmp --silent -- "$FILE1" "$FILE2"; 
then
  echo "files contents are identical"
else
    echo "files differ"
fi
