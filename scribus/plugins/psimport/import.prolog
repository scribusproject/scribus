%
% psi.prolog
%
% Copyright (C) 1996-2000 by vhf computer GmbH + vhf interservice GmbH
% Author:   Georg Fleischmann
%
% created:  2000-09-09
% modified: 
%
% this PostScript code writes a file with the structure below.
% # # # # l		(x0 y0 x1 y1) line
% # # # # # # # #c	(x0 y0 x1 y1 x2 y2 x3 y3) curve
% # w			(width) width
% # # # # # co		(c m y k a) color
% n			new list
% f			list is a filled polygon
% s			list is a group
% cl			list is a clip list (clip with old clip list and use it)
% gs			save current clip list and width to top of stack
% gr			use last clip list (on top of stack) and width
% # # # # # # # # # #	(x y a b c d e f text font) text
%
% Adapted for Scribus by Franz Schmid 15.05.2004
% Also removed the hardcoded Output Filename
% and changed it in a way the -sOutputFile Option of Ghostscript can be used
% Speeded up the flattening of Text by removing unneeded calculations.
% Changed the Output slightly to ease parsing
% m                 moveto  
% l # # # #         (x0 y0 x1 y1) line
% c # # # # # # # # (x0 y0 x1 y1 x2 y2 x3 y3) curve
% w #               (width) Linewidth
% co # # # # #      (c m y k a) color
% n                 new list
% f                 list is a filled polygon
% s                 list is a stroke path
% cp                close current subpath
% ci                list is a clip list (clip with old clip list and use it)
%
% 15.05.2004 Added the Glyphshow Operator.
% 17.05.2004 Made clipping working.
% 20.05.2004 kshow is working now.

/cfile OutputFile (w) file def
/print { cfile exch writestring } bind def

% whether we have to flatten the text
/flattenText 1 def

% is a Clipping there
/clipCnt 0 def

% remember the current point
/currentX 0 def
/currentY 0 def

% 1st point of path to close the path
/beginX 0 def
/beginY 0 def

% dummy for converting strings
/str 50 string def

% 0 = mirror at
/mirror 0 def

% mirror a at 0
/mir
{
	mirror 0 ne
	{	0 exch sub
	}if
} bind def

% scale
/m_a 1 def
/m_b 0 def
/m_c 0 def
/m_d 1 def
/m_x 0 def
/m_y 0 def
/matrix_x	% x y
{
	% ax + cy + tx
	m_c mul exch m_a mul add m_x add
} bind def
/matrix_y	% x y
{
	% bx + dy + ty
	m_d mul exch m_b mul add m_y add
} bind def

/writecurrentcolor
{
	currentcolorspace 0 get /CIEBasedABC eq
	{	currentcolor setrgbcolor
	} if

	currentcmykcolor	% -> c m y k
	(co )print
	3 index str cvs print
	( ) print
	2 index str cvs print
	( ) print
	1 index str cvs print
	( ) print
	str cvs print
	( ) print
	pop pop pop
	.currentopacityalpha	% a
	str cvs print
	(\n) print
} bind def

/writecurrentlinewidth
{
	userdict begin
	currentlinewidth	% w
	storeMatrix

	% (wb + wd + wa + wc) / 2
	dup dup dup m_b abs mul exch m_d abs mul add  exch m_a abs mul add  exch m_c abs mul add  2 div  abs
	(w ) print
	str cvs print
	(\n) print
	end
} bind def

/_move	% x y
{
	userdict begin
	(m\n) print
	/currentY exch def
	/currentX exch def
	/beginX currentX def
	/beginY currentY def
	end
} bind def

/_line
{
	userdict begin
	/y1 exch def
	/x1 exch def

	% x x1 ne y y1 ne or
	currentX x1 sub abs 0.001 gt  currentY y1 sub abs 0.001 gt or
	{
		(l ) print
		currentX currentY matrix_x str cvs print
		( ) print
		currentX currentY matrix_y str cvs print
		( ) print

		x1 y1 matrix_x str cvs print
		( ) print
		x1 y1 matrix_y str cvs print
		(\n) print
		/currentX x1 def
		/currentY y1 def
	}if
	end
} bind def

/_curve
{
	userdict begin
	% x1 y1 x2 y2 x3 y3
	(c ) print
	currentX currentY matrix_x str cvs print
	( ) print
	currentX currentY matrix_y str cvs print
	( ) print
	5 index 5 index matrix_x str cvs print
	( ) print
	5 index 5 index matrix_y str cvs print
	( ) print
	3 index 3 index matrix_x str cvs print
	( ) print
	3 index 3 index matrix_y str cvs print
	( ) print

	/currentY exch def
	/currentX exch def

	currentX currentY matrix_x str cvs print
	( ) print
	currentX currentY matrix_y str cvs print
	(\n)print
	pop pop pop pop
	end
} bind def

% modified: 18.10.96
/_close
{
	(cp\n) print
	beginX beginY _line
} bind def

/storeMatrix
{
	userdict begin
	matrix currentmatrix
	0 get /m_a exch def
	matrix currentmatrix
	1 get /m_b exch def
	matrix currentmatrix
	2 get /m_c exch def
	matrix currentmatrix
	3 get /m_d exch def
	matrix currentmatrix
	4 get /m_x exch def
	matrix currentmatrix
	5 get /m_y exch def
	end
} bind def

/rectfill
{
	userdict begin
	(n\n)print			% start polygon
	writecurrentcolor
	writecurrentlinewidth
	storeMatrix

	% x y width height
	dup type /arraytype ne
	{
		/hr exch def
		/wr exch def
		/yr exch def
		/xr exch def
		xr yr _move
		xr wr add yr _line
		xr wr add yr hr add _line
		xr yr hr add _line
		xr yr _line
	}
	% numarray
	% numstring
	{
		/ar exch def
		0 4 ar length 1 sub
		{
			/n exch def
			ar n get /xr exch def
			ar n 1 add get /yr exch def
			ar n 2 add get /wr exch def
			ar n 3 add get /hr exch def
			xr yr _move
			xr wr add yr _line
			xr wr add yr hr add _line
			xr yr hr add _line
			xr yr _line
		} for
	}ifelse

	(f\n)print			% close polygon
	end
} bind def

/rectstroke
{
	userdict begin
	(n\n)print			% start rect
	writecurrentcolor
	writecurrentlinewidth
	storeMatrix

	% x y width height
	dup type /arraytype ne
	{
		/hr exch def
		/wr exch def
		/yr exch def
		/xr exch def
		xr yr _move
		xr wr add yr _line
		xr wr add yr hr add _line
		xr yr hr add _line
		xr yr _line
	}
	% numarray
	% numstring
	{
		/ar exch def
		0 4 ar length 1 sub
		{
			/n exch def
			ar n get /xr exch def
			ar n 1 add get /yr exch def
			ar n 2 add get /wr exch def
			ar n 3 add get /hr exch def
			xr yr _move
			xr wr add yr _line
			xr wr add yr hr add _line
			xr yr hr add _line
			xr yr _line
		} for
	}ifelse
	(n\n)print			% stroke rect
	end
} bind def

/stroke
{
	(n\n) print
	writecurrentcolor
	writecurrentlinewidth
	clipCnt 1 eq
		{ clipsave clip newpath clippath cliprestore } if
	storeMatrix
	{_move} {_line} {_curve} {_close} pathforall
	(s\n)print			% stroke path
	newpath
} bind def

/eofill
{
	(n\n) print			% start polygon
	writecurrentcolor	% write color
	writecurrentlinewidth
	clipCnt 1 eq
		{ clipsave clip newpath clippath cliprestore } if
	storeMatrix			% take transformation, scaling, rotation from PostScript
	{_move} {_line} {_curve} {_close} pathforall
	(f\n)print			% close polygon

	newpath				% clear stack
} bind def

/fill
{
	eofill
} bind def

/clip
{
	userdict begin
	(n\n)print			% start clip polygon

	storeMatrix			% take transformation, scaling, rotation from PostScript
	{_move} {_line} {_curve} {_close} pathforall

	(ci\n)print			% close clip polygon begin path
						% we have to close the path!!
	clip
	/clipCnt 1 def
	newpath				% clear stack
	end
} bind def

% we don't clip
% because this doesn't work for flattening text (show, charpath) with NeXT PostScript Code
/rectclip
{
	pop pop pop pop
} bind def

/stateArray 500 array def
/stateTop 0 def
/gsave
{
	userdict begin
%	(gs\n) print
	stateArray stateTop gstate currentgstate put
	/stateTop stateTop 1 add def
	end
} bind def

/grestore
{
	userdict begin
	stateTop 1 lt
	{
	}
	{
%		(gr\n) print
		stateArray stateTop 1 sub get setgstate
		/stateTop stateTop 1 sub def
		stateArray stateTop 0 put
	}ifelse
	end
} bind def

% a bind def of the show operator doesn't work,
% so this is our way to get a charpath entry for flattening text
/root_charpath
{
	charpath
} bind def

/show
{
	userdict begin
	storeMatrix
	currentfont /FontName known
	% stack: string
	{
		currentpoint	% x y
		newpath
		/clipCnt 0 def
		moveto
		% we process each char separately to get smaller paths
		0 1 2 index length 1 sub
		{
			(n\n)print			% start polygon
			writecurrentcolor	% write color
			storeMatrix
			currentpoint	% x y
			newpath			% clear graphic stack
			moveto
			1 index exch 1 getinterval false root_charpath
			{_move} {_line} {_curve} {_close} pathforall
			(f\n)print			% close polygon
		} for
		currentpoint	% x y
		newpath				% clear graphic stack (and current point)
		moveto
	} if
	pop
	end
} def

/ashow
{
	% ax ay string
	exch pop
	exch pop
	show
} bind def

/widthshow	% cx cy char string
{
	exch pop
	exch pop
	exch pop
	show
} bind def

/awidthshow		% cx cy char ax ay string
{
	exch pop
	exch pop
	exch pop
	exch pop
	exch pop
	show
} bind def

/cshow	% proc string
{
	exch pop
	show
} bind def

/kshow	% proc string
{
	dup length 1 sub 
	dup 0 ne
	{
		1 index 0 1 getinterval
		show
		1 sub
		dup 0 ne
		{
			1 add
			1 exch 1 exch
			{
				dup 1 sub
				2 index exch get
				2 index 2 index get
				4 index exec
				1 index exch 1 getinterval
				show
			} for
		} if
	}
	{
		pop dup show
	} ifelse
	pop pop
} bind def

/xshow	% string array
{
	pop
	show
} bind def

/xyshow	% string array
{
	pop
	show
} bind def

/yshow	% string array
{
	pop
	show
} bind def

/ReEnc % newfontname reencodevector origfontdict -> ReEncode -> newfontdict
{
	userdict begin
 dup begin dup maxlength dict begin
  { 1 index /FID ne {def} {pop pop} ifelse
  } forall
  /Encoding exch def
  currentdict
  end end
  definefont
	end
} bind def

/glyphshow {
    save % So can reclaim VM from reencoding
    currentfont /Encoding get dup length array copy dup 0 5 -1 roll put
    /GlyphShowTempFont exch currentfont ReEnc
    setfont
    (\000) show 
	restore
} bind def

