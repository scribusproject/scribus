all:
	doxygen
	echo "*** See error.log for errors and warnings"
	echo "*** Applying templates"
	cat templates/header.html > index.html
	cat templates/index.html >> index.html
	cat templates/footer.html >> index.html

clean:
	rm *.ttf *.dot *.html *.png *.css error.log *.map *.md5

tgz:
	tar -cf Scribus-devel-docs.tar *
	gzip Scribus-devel-docs.tar

tbz:
	tar -cf Scribus-devel-docs.tar *
	bzip2 Scribus-devel-docs.tar

codecheck:
	flawfinder --html -c ../scribus > code-check.html

