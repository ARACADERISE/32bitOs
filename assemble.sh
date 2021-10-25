for f in *.s
do
	fasm $f ${f%%s}bin
done
